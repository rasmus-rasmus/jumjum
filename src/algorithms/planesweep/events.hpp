#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include "primitives/linesegment.hpp"

namespace algorithms
{

struct Event
{
    virtual primitives::Point getPoint() const = 0;
};

bool operator<(const Event& lhs, const Event& rhs);

struct IntersectionEvent: public Event
{
    IntersectionEvent(primitives::LineSegment* line, primitives::LineSegment* otherLine);

    IntersectionEvent() = delete;
    IntersectionEvent& operator=(const IntersectionEvent& ref) = delete;
    IntersectionEvent& operator=(IntersectionEvent&& ref) = delete;

    const primitives::LineSegment& getFirstLine() const { return *m_firstLine; }
    const primitives::LineSegment& getSecondLine() const { return *m_secondLine; }

    primitives::LineLineIntersection getIntersection() const { return m_intersection; };

    primitives::Point getPoint() const override;

private:
    primitives::LineSegment* m_firstLine;
    primitives::LineSegment* m_secondLine;
    primitives::LineLineIntersection m_intersection;
};

struct EndPointEvent: public Event
{
    EndPointEvent(primitives::LineSegment* line, bool isUpper) : m_line(line), m_isUpper(isUpper) {}

    EndPointEvent() = delete;
    EndPointEvent& operator=(const EndPointEvent& ref) = delete;
    EndPointEvent& operator=(EndPointEvent&& ref) = delete;

    primitives::Point getPoint() const override;

private:
    bool m_isUpper;
    primitives::LineSegment* m_line;
};

} // namespace algoithms

#endif