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

    auto intersectionLine = std::get<primitives::LineSegment>(intersection);
    return std::min(intersectionLine.getStartPoint().x(), intersectionLine.getEndPoint().x());
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
    ComparableLineSegment(const primitives::LineSegment& line, double currY) : primitives::LineSegment(line), compareLine(currY) {}

    bool operator<(const ComparableLineSegment& otherLine) const
    {
        double minCompareLine = std::min(compareLine, otherLine.compareLine);
        std::vector<double> xValues = {getStartPoint().x(), getEndPoint().x(), otherLine.getStartPoint().x(), otherLine.getEndPoint().x()};
        double xmin = *std::min_element(xValues.begin(), xValues.end());
        double xmax = *std::max_element(xValues.begin(), xValues.end());
        primitives::LineSegment horizontalLineAtCurrY(primitives::Point(xmin - 1., minCompareLine), primitives::Point(xmax + 1., minCompareLine));

        auto thisIntersection = computeIntersection(horizontalLineAtCurrY);
        auto otherIntersection = otherLine.computeIntersection(horizontalLineAtCurrY);

        return getComparisonValue(thisIntersection) < getComparisonValue(otherIntersection);
    }

    double compareLine;
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
void insertIntersectionEvent(std::set<Event*, EventComparator>& eventQueue, const primitives::LineSegment& firstLine, const primitives::LineSegment& secondLine)
{
    auto newIntersection = new IntersectionEvent(firstLine, secondLine);
    Event* newEvent = newIntersection;
    eventQueue.insert(newEvent);
}

// Memory safe removal of events from eventQueue.
void eraseEventFromQueue(std::set<Event*, EventComparator>& eventQueue, std::set<Event*, EventComparator>::iterator it)
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
void processIntersectionEvent(std::set<Event*, EventComparator>& eventQueue, 
                              std::set<ComparableLineSegment>& statusLine, 
                              std::vector<LinePair>& intersectionsOut, 
                              const IntersectionEvent* event)
{
    intersectionsOut.push_back(
        {event->getFirstLine(), event->getSecondLine()}
    );

    double currY = event->getPosition().y();

    // Add small offset to the comparison ray, since firstLine and secondLine will compare equal 
    // if compared exactly along the y value of the intersection point.
    auto firstLinePos = statusLine.find(ComparableLineSegment(event->getFirstLine(), currY + 1e-5));
    auto secondLinePos = statusLine.find(ComparableLineSegment(event->getSecondLine(), currY + 1e-5));

    if (firstLinePos == statusLine.end() || secondLinePos == statusLine.end() || std::distance(firstLinePos, secondLinePos) != 1)
    {
        throw std::logic_error("Intersection event with lines not in status line or not adjacent.");
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
        insertIntersectionEvent(eventQueue, (primitives::LineSegment)*minNeighbor, (primitives::LineSegment)*minPos);
    }

    auto maxPos = minPos;
    maxPos++;
    // As above, maxNeighbor will be incremented once it has been verified that this is valid.
    auto maxNeighbor = maxPos;
    if (++maxNeighbor != statusLine.end() && maxPos->intersects(*maxNeighbor))
    {
        insertIntersectionEvent(eventQueue, (primitives::LineSegment)*maxPos, (primitives::LineSegment)*maxNeighbor);
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
void processEndPointEvent(std::set<Event*, EventComparator>& eventQueue, 
                          std::set<ComparableLineSegment>& statusLine, 
                          std::vector<LinePair>& intersectionsOut,
                          const EndPointEvent* event)
{
    if (event->isUpperEndPoint())
    {
        double currY = event->getPosition().y();
        ComparableLineSegment toInsert(event->getLine(), currY);
        auto insertedIt = statusLine.insert(toInsert).first;

        // Will be properly incremented/decremented once verified that this is valid.
        auto leftNbr = insertedIt;
        auto rightNbr = insertedIt;

        if (insertedIt != statusLine.begin() && insertedIt->intersects(*(--leftNbr)))
        {
            insertIntersectionEvent(eventQueue, (primitives::LineSegment)*leftNbr, (primitives::LineSegment)*insertedIt);
        }

        if (++rightNbr != statusLine.end() && insertedIt->intersects(*rightNbr))
        {
            insertIntersectionEvent(eventQueue, (primitives::LineSegment)*insertedIt, (primitives::LineSegment)*rightNbr);
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
        insertIntersectionEvent(eventQueue, (primitives::LineSegment)*leftNbr, (primitives::LineSegment)*rightNbr);
    }
}

void processNextEvent(std::set<Event*, EventComparator>& eventQueue, std::set<ComparableLineSegment>& statusLine, std::vector<LinePair>& intersectionsOut)
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
    std::set<Event*, EventComparator> eventQueue;

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
