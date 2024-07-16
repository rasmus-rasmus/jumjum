#include "planesweep.hpp"

#include <set>
#include <algorithm>

#include <iostream>

#include "algorithms/planesweep/events.hpp"

namespace algorithms
{

double getComparisonValue(const primitives::LineLineIntersection& intersection)
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
    ComparableLineSegment(const primitives::LineSegment& line, double currY) : primitives::LineSegment(line), compareLine(currY) 
    {
        // Horizontal lines are difficult to compare because they will not intersect a given comparison ray
        // in a unique point. So they are slightly fudged to make them comparable.
        if (std::abs(getStartPoint().y() - getEndPoint().y()) < 2 * 1e-5)
        {
            fudgeEndPoints();
        }
    }

    bool operator<(const ComparableLineSegment& otherLine) const
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
        if (hasBeenFudged) return;
        setStartPoint(primitives::Point(getStartPoint().x(), getStartPoint().y() + 1e-5));
        setEndPoint(primitives::Point(getEndPoint().x(), getEndPoint().y() - 1e-5));
        hasBeenFudged = true;
    }

    void defudgeEndPoints()
    {
        if (!hasBeenFudged) return;

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
struct EventComparator {
    bool operator()(Event* lhs, Event* rhs) const {
        return *lhs < *rhs;
    }
};

Planesweep::Planesweep(const std::vector<primitives::LineSegment>& lines)
{
    for (auto line : lines)
    {
        m_lines.push_back(line);   
    }
}

// Allocates memory for IntersectionEvent on the heap and inserts pointer to new event into eventQueue.
// NB: Remember to deallocate when removing events from queue. Use eraseEventFromQueue below for memory safe removal. 
void insertIntersectionEvent(std::multiset<Event*, EventComparator>& eventQueue, 
                             ComparableLineSegment firstLine, 
                             ComparableLineSegment secondLine)
{
    // Ensure that the actual unfundged lines are recorded.
    firstLine.defudgeEndPoints();
    secondLine.defudgeEndPoints();

    auto newIntersectionEvent = new IntersectionEvent(firstLine, secondLine);
    Event* newEvent = newIntersectionEvent;
    eventQueue.insert(newEvent);
}

// Memory safe removal of events from eventQueue.
void eraseEventFromQueue(std::multiset<Event*, EventComparator>& eventQueue, std::multiset<Event*, EventComparator>::iterator it)
{
    auto eventPtr = *it;
    eventQueue.erase(it);
    delete eventPtr;
}

// Process intersection event: 
//  1. Report Intersection to intersectionsOut.
//  2. Swap the positions of the two intersecting lines in the status line.
//  3. Check if the two lines intersect their new neighbors to the left resp. right
//     in the status line and add intersection event to the event queue if so.
void processIntersectionEvent(std::multiset<Event*, EventComparator>& eventQueue, 
                              std::set<ComparableLineSegment>& statusLine, 
                              std::vector<LinePair>& intersectionsOut, 
                              const IntersectionEvent* event)
{
    intersectionsOut.push_back(
        {event->getFirstLine(), event->getSecondLine()}
    );

    double currY = event->getPosition().y();

    double firstLineMaxY = std::max(event->getFirstLine().getStartPoint().y(), event->getFirstLine().getEndPoint().y());
    double firstLineMinY = std::min(event->getFirstLine().getStartPoint().y(), event->getFirstLine().getEndPoint().y());
    double secondLineMaxY = std::max(event->getSecondLine().getStartPoint().y(), event->getSecondLine().getEndPoint().y());
    double secondLineMinY = std::min(event->getSecondLine().getStartPoint().y(), event->getSecondLine().getEndPoint().y());

    if (firstLineMaxY < currY + 1e-5 || secondLineMaxY < currY + 1e-5
        || firstLineMinY > currY - 1e-5 || secondLineMinY > currY - 1e-5)
    {
        // The intersection happens in an (upper or lower) end point of one or both lines,
        // so we don't need to swap their positions in the status line.
        return;
    }

    auto firstLinePos = statusLine.find(ComparableLineSegment(event->getFirstLine(), currY + 1e-5));
    auto secondLinePos = statusLine.find(ComparableLineSegment(event->getSecondLine(), currY + 1e-5));

    if (firstLinePos == statusLine.end())
    {
        throw std::logic_error("Intersection event with first line not in status line.");
    }

    if (secondLinePos == statusLine.end())
    {
        throw std::logic_error("Intersection event with second line not in status line.");
    }

    if (std::distance(firstLinePos, secondLinePos) != 1)
    {
        throw std::logic_error("Intersection event with lines not adjacent in status line.");
    }

    // To swap the positions of firstLine and secondLine in the status line we have to remove
    // firstLine and then insert it again compared along a ray just *below* the intersection point.
    // To this end, we subtract a small offset from the comparison ray.
    statusLine.erase(firstLinePos);
    ComparableLineSegment toInsert(event->getFirstLine(), currY-1e-5);
    statusLine.insert(toInsert);

    auto minPos = *firstLinePos < *secondLinePos ? firstLinePos : secondLinePos;

    // minNeighbor will be decremented below once it has been verified that this is valid,
    // i. e., that minPos is not first element.
    auto minNeighbor = minPos;
    if (minPos != statusLine.begin() && minPos->intersects(*(--minNeighbor)))
    {
        insertIntersectionEvent(eventQueue, *minNeighbor, *minPos);
    }

    auto maxPos = minPos;
    maxPos++;
    // As above, maxNeighbor will be incremented once it has been verified that this is valid.
    auto maxNeighbor = maxPos;
    if (++maxNeighbor != statusLine.end() && maxPos->intersects(*maxNeighbor))
    {
        insertIntersectionEvent(eventQueue, *maxPos, *maxNeighbor);
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
void processEndPointEvent(std::multiset<Event*, EventComparator>& eventQueue, 
                          std::set<ComparableLineSegment>& statusLine, 
                          std::vector<LinePair>& intersectionsOut,
                          const EndPointEvent* event)
{
    if (event->isUpperEndPoint())
    {
        double currY = event->getPosition().y();
        ComparableLineSegment toInsert(event->getLine(), currY);
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
            insertIntersectionEvent(eventQueue, *leftNbr, *insertedIt);
        }

        if (++rightNbr != statusLine.end() && insertedIt->intersects(*rightNbr))
        {
            insertIntersectionEvent(eventQueue, *insertedIt, *rightNbr);
        }

        return;
    }
    
    // Lower end point event.
    auto currY = event->getPosition().y();
    auto itToDelete = statusLine.find(ComparableLineSegment(event->getLine(), currY));
    auto rightNbr = statusLine.erase(itToDelete);
    auto leftNbr = rightNbr;
    if (rightNbr != statusLine.end() && rightNbr != statusLine.begin() && (--leftNbr)->intersects(*rightNbr))
    {
        insertIntersectionEvent(eventQueue, *leftNbr, *rightNbr);
    }
}

void processNextEvent(std::multiset<Event*, EventComparator>& eventQueue, std::set<ComparableLineSegment>& statusLine, std::vector<LinePair>& intersectionsOut)
{
    auto nextEventIt = eventQueue.begin();
    auto nextEvent = *nextEventIt;

    if (nextEvent->getType() == EventType::Intersection)
    {
        auto intersectionEvent = dynamic_cast<const IntersectionEvent*>(nextEvent);
        processIntersectionEvent(eventQueue, statusLine, intersectionsOut, intersectionEvent);

        eraseEventFromQueue(eventQueue, nextEventIt);

        return;
    }

    auto endPointEvent = dynamic_cast<EndPointEvent*>(nextEvent);
    processEndPointEvent(eventQueue, statusLine, intersectionsOut, endPointEvent);

    eraseEventFromQueue(eventQueue, nextEventIt);
}

std::vector<LinePair> Planesweep::perform()
{
    std::multiset<Event*, EventComparator> eventQueue;

    std::set<ComparableLineSegment> statusLine;
    std::vector<LinePair> intersectionsOut;

    for (size_t i = 0; i < m_lines.size(); ++i)
    {
        // Insert EndPointEvent once for each end point with isUpper set resp not set. 
        // The EndPointEvent will take care of returning the relevant end point for comparison
        // based on whether it is upper or not.
        eventQueue.insert(new EndPointEvent(m_lines[i], true));
        eventQueue.insert(new EndPointEvent(m_lines[i], false));
    }

    while (!eventQueue.empty())
    {
        processNextEvent(eventQueue, statusLine, intersectionsOut);
    }

    return intersectionsOut;
}

} // namespace algorithms
