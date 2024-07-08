#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include <optional>
#include <variant>

#include "primitives/linesegment.hpp"

using LineLineIntersection = std::variant<Point, LineSegment>;

struct IntersectionEvent
{
    IntersectionEvent(LineSegment* line, LineSegment* otherLine);

    IntersectionEvent() = delete;
    IntersectionEvent& operator=(const IntersectionEvent& ref) = delete;
    IntersectionEvent& operator=(IntersectionEvent&& ref) = delete;

    const LineSegment& getFirstLine() const { return *m_firstLine; }
    const LineSegment& getSecondLine() const { return *m_secondLine; }

    LineLineIntersection getIntersection();

private:
    LineSegment* m_firstLine;
    LineSegment* m_secondLine;
    std::optional<LineLineIntersection> m_intersection = std::nullopt;
};

struct EndPointEvent
{
    EndPointEvent(LineSegment* line, bool isUpper) : m_line(line), m_isUpper(isUpper) {}

    EndPointEvent() = delete;
    EndPointEvent& operator=(const EndPointEvent& ref) = delete;
    EndPointEvent& operator=(EndPointEvent&& ref) = delete;

    Point getPoint();

private:
    bool m_isUpper;
    LineSegment* m_line;
};

#endif