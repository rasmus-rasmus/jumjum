#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include <optional>

#include "primitives/linesegment.hpp"

struct IntersectionEvent
{
    IntersectionEvent(LineSegment line, LineSegment otherLine);

    IntersectionEvent() = delete;
    IntersectionEvent& operator=(const IntersectionEvent& ref) = delete;
    IntersectionEvent& operator=(IntersectionEvent&& ref) = delete;

    LineSegment getFirstLine() const { return m_firstLine; }
    LineSegment getSecondLine() const { return m_secondLine; }

    Point getIntersectionPoint();

private:
    LineSegment m_firstLine;
    LineSegment m_secondLine;
    std::optional<Point> m_intersectionPoint = std::nullopt;
};

#endif