#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include "primitives/linesegment.hpp"

struct IntersectionEvent
{
    IntersectionEvent(LineSegment line, LineSegment otherLine);

    IntersectionEvent() = delete;
    IntersectionEvent& operator=(const IntersectionEvent& ref) = delete;
    IntersectionEvent& operator=(IntersectionEvent&& ref) = delete;

    LineSegment firstLine;
    LineSegment secondLine;

    Point getIntersectionPoint();

private:
    Point* m_intersectionPoint = nullptr;
};

#endif