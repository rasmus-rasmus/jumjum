#include "planesweep.hpp"

#include <algorithm>
#include <set>
#include <memory>

#include <iostream>

#include "algorithms/planesweep/events.hpp"

namespace algorithms
{

    double getComparisonValue(const primitives::LineLineIntersection &intersection)
    {
        if (std::holds_alternative<std::monostate>(intersection))
        {
            throw std::logic_error("Trying to get comparison point from empty intersection.");
        }

        if (std::holds_alternative<primitives::Point>(intersection))
        {
            return std::get<primitives::Point>(intersection).x();
        }

        throw std::logic_error("Non-fudged horizontal lines in comparison, which shouldn't happen.");
    }

    // Wrapper class around LineSegment to implement comparison of lines when inserting
    // in status line. Lines are compared by comparing the x-values of their intersections
    // with the status line. Any LineSegment will hold the position of the status line
    // at the time when they were inserted into it in their compareLine variable.
    // Since the status line moved from top to bottom the min of the two lines compareLine
    // values is used to compare them, as this is presumably the current position of the
    // status line.
    struct ComparableLineSegment : public primitives::LineSegment
    {
        ComparableLineSegment(const primitives::LineSegment &line, double currY) : primitives::LineSegment(line), compareLine(currY)
        {
            // Horizontal lines are difficult to compare because they will not intersect a given comparison ray
            // in a unique point. So they are slightly fudged to make them comparable.
            if (std::abs(getStartPoint().y() - getEndPoint().y()) < 2 * 1e-5)
            {
                fudgeEndPoints();
            }
        }

        bool operator<(const ComparableLineSegment &otherLine) const
        {
            double minCompareLine = std::min(compareLine, otherLine.compareLine);
            std::vector<double> xValues = {getStartPoint().x(), getEndPoint().x(), otherLine.getStartPoint().x(), otherLine.getEndPoint().x()};
            double xmin = *std::min_element(xValues.begin(), xValues.end());
            double xmax = *std::max_element(xValues.begin(), xValues.end());
            primitives::LineSegment horizontalLineAtCurrY(primitives::Point(xmin - 1., minCompareLine), primitives::Point(xmax + 1., minCompareLine));

            auto thisComparisonValue = getComparisonValue(computeIntersection(horizontalLineAtCurrY));
            auto otherComparisonValue = getComparisonValue(otherLine.computeIntersection(horizontalLineAtCurrY));

            if (std::abs(thisComparisonValue - otherComparisonValue) < 1e-8)
            {
                // If the two lines intersect the comparison line at the same point, they should
                // be considered equivalent.
                return false;
            }

            return thisComparisonValue < otherComparisonValue;
        }

        void fudgeEndPoints()
        {
            if (hasBeenFudged)
                return;
            setStartPoint(primitives::Point(getStartPoint().x(), getStartPoint().y() + 1e-5));
            setEndPoint(primitives::Point(getEndPoint().x(), getEndPoint().y() - 1e-5));
            hasBeenFudged = true;
        }

        void defudgeEndPoints()
        {
            if (!hasBeenFudged)
                return;

            setStartPoint(primitives::Point(getStartPoint().x(), getStartPoint().y() - 1e-5));
            setEndPoint(primitives::Point(getEndPoint().x(), getEndPoint().y() + 1e-5));
            hasBeenFudged = false;
        }

        double compareLine;

    private:
        bool hasBeenFudged = false;
    };

    // Wrapper comparator to be able to use the Event comparison operator to
    // compare Events pointers when inserting them into a sorted container.
    struct EventComparator
    {
        bool operator()(const std::unique_ptr<Event>& lhs, const std::unique_ptr<Event>& rhs) const
        {
            return *lhs < *rhs;
        }
    };

    Planesweep::Planesweep(const std::vector<primitives::LineSegment> &lines)
    {
        for (auto line : lines)
        {
            m_lines.push_back(line);
        }
    }

    // Allocates memory for IntersectionEvent and inserts pointer to new event into eventQueue.
    // NB: Remember to deallocate when removing events from queue. Use eraseEventFromQueue below for memory safe removal.
    void insertIntersectionEvent(std::multiset<std::unique_ptr<Event>, EventComparator> &eventQueue,
                                 ComparableLineSegment firstLine,
                                 ComparableLineSegment secondLine,
                                 double currSweepLinePosition)
    {
        // Ensure that the actual unfudged lines are recorded.
        firstLine.defudgeEndPoints();
        secondLine.defudgeEndPoints();

        std::unique_ptr<Event> newIntersectionEvent(new IntersectionEvent(firstLine, secondLine));

        if (newIntersectionEvent->getPosition().y() > currSweepLinePosition)
        {
            // Intersection is above current sweep line, so has already been processed
            return;
        }

        auto [it0, it1] = eventQueue.equal_range(newIntersectionEvent);
        for (auto foundIt = it0; foundIt != it1; ++foundIt)
        {
            if (*dynamic_cast<IntersectionEvent *>(foundIt->get()) == *dynamic_cast<IntersectionEvent *>(newIntersectionEvent.get()))
            {
                // Intersection event is alreay in event queue
                return;
            }
        }

        eventQueue.insert(std::move(newIntersectionEvent));
    }

    // Process intersection event:
    //  1. Report Intersection to intersectionsOut.
    //  2. Swap the positions of the two intersecting lines in the status line.
    //  3. Check if the two lines intersect their new neighbors to the left resp. right
    //     in the status line and add intersection event to the event queue if so.
    void processIntersectionEvent(std::multiset<std::unique_ptr<Event>, EventComparator> &eventQueue,
                                  std::set<ComparableLineSegment> &statusLine,
                                  std::vector<LinePair> &intersectionsOut,
                                  const IntersectionEvent *event)
    {
        intersectionsOut.push_back(
            {event->getFirstLine(), event->getSecondLine()});

        double currSweepLinePosition = event->getPosition().y();

        double firstLineMaxY = std::max(event->getFirstLine().getStartPoint().y(), event->getFirstLine().getEndPoint().y());
        double firstLineMinY = std::min(event->getFirstLine().getStartPoint().y(), event->getFirstLine().getEndPoint().y());
        double secondLineMaxY = std::max(event->getSecondLine().getStartPoint().y(), event->getSecondLine().getEndPoint().y());
        double secondLineMinY = std::min(event->getSecondLine().getStartPoint().y(), event->getSecondLine().getEndPoint().y());

        if (firstLineMaxY < currSweepLinePosition + 1e-5 || secondLineMaxY < currSweepLinePosition + 1e-5 || firstLineMinY > currSweepLinePosition - 1e-5 || secondLineMinY > currSweepLinePosition - 1e-5)
        {
            // The intersection happens in an (upper or lower) end point of one or both lines,
            // so we don't need to swap their positions in the status line.
            return;
        }

        auto firstLinePos = statusLine.find(ComparableLineSegment(event->getFirstLine(), currSweepLinePosition + 1e-5));
        auto secondLinePos = statusLine.find(ComparableLineSegment(event->getSecondLine(), currSweepLinePosition + 1e-5));

        if (firstLinePos == statusLine.end())
        {
            throw std::logic_error("Intersection event with first line not in status line.");
        }

        if (secondLinePos == statusLine.end())
        {
            throw std::logic_error("Intersection event with second line not in status line.");
        }

        if (std::abs(std::distance(firstLinePos, secondLinePos)) != 1)
        {
            throw std::logic_error("Intersection event with lines not adjacent in status line.");
        }
        auto minPos = std::distance(firstLinePos, secondLinePos) > 0 ? firstLinePos : secondLinePos;
        auto maxPos = std::next(minPos);

        // To swap the positions of firstLine and secondLine in the status line we have to remove
        // one (we choose the lowest ordered, i. e., minPos) and then insert it again compared along
        // a ray just *below* the intersection point. To this end, we fudge the comparison ray slightly down.
        // For minPos and maxPos to still correctly point to the lowest, resp. hightest, ordered line,
        // they must also be correctly reassigned.
        ComparableLineSegment toInsert(*minPos, currSweepLinePosition - 1e-5);
        minPos = statusLine.erase(minPos);
        maxPos = statusLine.insert(toInsert).first;

        // minNeighbor will be decremented below once it has been verified that this is valid,
        // i. e., that minPos is not first element.
        auto minNeighbor = minPos;
        if (minPos != statusLine.begin() && minPos->intersects(*(--minNeighbor)))
        {
            insertIntersectionEvent(eventQueue, *minNeighbor, *minPos, currSweepLinePosition);
        }

        // As above, maxNeighbor will be incremented once it has been verified that this is valid.
        auto maxNeighbor = maxPos;
        if (++maxNeighbor != statusLine.end() && maxPos->intersects(*maxNeighbor))
        {
            insertIntersectionEvent(eventQueue, *maxPos, *maxNeighbor, currSweepLinePosition);
        }
    }

    // Process end point event:
    //  If Upper:
    //      1. Add line to status line.
    //      2. Check if line intersect its neighbors to the left resp. right and add
    //         intersection event(s) if this is the case.
    //  If lower:
    //      1. Remove line from status line.
    //      2. Check whether the recently removed line's neighbors to the left resp. right
    //         (which are now adjacent) intersect and add intersection event to the event queue
    //         if this is the case.
    void processEndPointEvent(std::multiset<std::unique_ptr<Event>, EventComparator> &eventQueue,
                              std::set<ComparableLineSegment> &statusLine,
                              std::vector<LinePair> &intersectionsOut,
                              const EndPointEvent *event)
    {
        if (event->isUpperEndPoint())
        {
            double currSweepLinePosition = event->getPosition().y();
            ComparableLineSegment toInsert(event->getLine(), currSweepLinePosition);
            if (statusLine.find(toInsert) != statusLine.end())
            {
                // If the line is intersected in its upper end point by
                // a line already present in the status line, we cannot insert it, since
                // it will compare equal to that line along currY, so we fudge currY a bit.
                // This way the two lines will (hopefully) end up adjacent in the status line,
                // so that their intersection will be detected below.
                // NB: This will not handle the case where the two lines in question are
                // parallel and overlapping, i. e., the intersection is another line.
                // TODO: Handle this!
                toInsert.compareLine -= 1e-5;
            }
            auto insertedIt = statusLine.insert(toInsert).first;

            // Will be properly incremented/decremented once verified that this is valid.
            auto leftNbr = insertedIt;
            auto rightNbr = insertedIt;

            if (insertedIt != statusLine.begin() && insertedIt->intersects(*(--leftNbr)))
            {
                insertIntersectionEvent(eventQueue, *leftNbr, *insertedIt, currSweepLinePosition);
            }

            if (++rightNbr != statusLine.end() && insertedIt->intersects(*rightNbr))
            {
                insertIntersectionEvent(eventQueue, *insertedIt, *rightNbr, currSweepLinePosition);
            }

            return;
        }

        // Lower end point event.
        auto currSweepLinePosition = event->getPosition().y();
        auto itToDelete = statusLine.find(ComparableLineSegment(event->getLine(), currSweepLinePosition));
        auto rightNbr = statusLine.erase(itToDelete);
        auto leftNbr = rightNbr;
        if (rightNbr != statusLine.end() && rightNbr != statusLine.begin() && (--leftNbr)->intersects(*rightNbr))
        {
            insertIntersectionEvent(eventQueue, *leftNbr, *rightNbr, currSweepLinePosition);
        }
    }

    void processNextEvent(std::multiset<std::unique_ptr<Event>, EventComparator> &eventQueue,
                          std::set<ComparableLineSegment> &statusLine,
                          std::vector<LinePair> &intersectionsOut)
    {
        auto nextEvent = eventQueue.begin()->get();

        if (nextEvent->getType() == EventType::Intersection)
        {
            auto intersectionEvent = dynamic_cast<const IntersectionEvent *>(nextEvent);
            processIntersectionEvent(eventQueue, statusLine, intersectionsOut, intersectionEvent);

            return;
        }

        auto endPointEvent = dynamic_cast<EndPointEvent *>(nextEvent);
        processEndPointEvent(eventQueue, statusLine, intersectionsOut, endPointEvent);
    }

    void processAndPopNextEvent(std::multiset<std::unique_ptr<Event>, EventComparator> &eventQueue,
                                std::set<ComparableLineSegment> &statusLine,
                                std::vector<LinePair> &intersectionsOut)
    {
        auto nextEventIt = eventQueue.begin();
        processNextEvent(eventQueue, statusLine, intersectionsOut);
        eventQueue.erase(nextEventIt);
    }

    std::vector<LinePair> Planesweep::perform(bool printDebugInfo)
    {
        std::multiset<std::unique_ptr<Event>, EventComparator> eventQueue;

        std::set<ComparableLineSegment> statusLine;
        std::vector<LinePair> intersectionsOut;

        for (size_t i = 0; i < m_lines.size(); ++i)
        {
            // Insert EndPointEvent once for each end point with isUpper set resp. not set.
            // The EndPointEvent will take care of returning the relevant end point for comparison
            // based on whether it is upper or not.
            eventQueue.insert(std::unique_ptr<Event>(new EndPointEvent(m_lines[i], true)));
            eventQueue.insert(std::unique_ptr<Event>(new EndPointEvent(m_lines[i], false)));
        }

        while (!eventQueue.empty())
        {
            if (printDebugInfo)
            {
                std::cout << "Processing event of type: " << (*eventQueue.begin())->getType() << std::endl;
                std::cout << "Status line: " << std::endl;
                for (auto line : statusLine)
                {
                    std::cout << line << std::endl;
                }
                std::cout << "\nEvent Queue: " << std::endl;
                for (auto &&event : eventQueue)
                {
                    std::cout << "Type: " << event->getType() << " -- " << "Position: " << event->getPosition() << std::endl;
                }
                std::cout << "-----------------------------------" << std::endl;
            }

            processAndPopNextEvent(eventQueue, statusLine, intersectionsOut);
        }

        return intersectionsOut;
    }

} // namespace algorithms
