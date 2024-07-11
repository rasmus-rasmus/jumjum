#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include "primitives/linesegment.hpp"

namespace algorithms
{

struct Event
{
    virtual ~Event() {};
    virtual primitives::Point getPoint() const = 0;
    virtual primitives::LineLineIntersection getIntersection() const = 0;
};

bool operator<(const Event& lhs, const Event& rhs);

struct IntersectionEvent: public Event
{
    IntersectionEvent(primitives::LineSegment* line, primitives::LineSegment* otherLine);

    IntersectionEvent() = delete;

    const primitives::LineSegment& getFirstLine() const { return *m_firstLine; }
    const primitives::LineSegment& getSecondLine() const { return *m_secondLine; }

    primitives::LineLineIntersection getIntersection() const override { return m_intersection; };

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

    primitives::Point getPoint() const override;

    primitives::LineLineIntersection getIntersection() const override
    {
        throw std::logic_error("getIntersection called on non-IntersectionEvent.");
    }

    bool isUpperEndPoint() const { return m_isUpper; }

private:
    bool m_isUpper;
    primitives::LineSegment* m_line;
};

} // namespace algorithms

#endif