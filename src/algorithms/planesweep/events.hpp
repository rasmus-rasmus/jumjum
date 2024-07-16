#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include "primitives/linesegment.hpp"

namespace algorithms
{

enum EventType
{
    Base,
    Intersection,
    EndPoint
};

// Abstract base class to be implemented by IntersectionEvent and EndPointEvent.
// We need this base class to have a common comparison operator for all types of events
// and to be able to store all types of events in a single sorted container; the status line.
struct Event
{
    virtual ~Event() {};

    // We compare events by comparing their positions.
    // For end point events their position is simply the position of the end point in question
    // (based on whether it's an upper or lower end point event),
    // for intersection events it is of course the position of the intersection point.
    // If the lines intersect in another line, it is not immediately obvious what its position is.
    // for the time being we use the smallest end point.
    virtual primitives::Point getPosition() const = 0;

    EventType getType() const { return m_eventType; }

    bool operator<(const Event& otherEvent);

protected:
    EventType m_eventType = EventType::Base;
};

struct IntersectionEvent: public Event
{
    IntersectionEvent(primitives::LineSegment line, primitives::LineSegment otherLine);

    const primitives::LineSegment& getFirstLine() const { return m_firstLine; }
    const primitives::LineSegment& getSecondLine() const { return m_secondLine; }

    primitives::LineLineIntersection getIntersection() const { return m_intersection; };

    primitives::Point getPosition() const override;

private:
    primitives::LineSegment m_firstLine;
    primitives::LineSegment m_secondLine;
    primitives::LineLineIntersection m_intersection;
};

struct EndPointEvent: public Event
{
    EndPointEvent(primitives::LineSegment line, bool isUpper) : m_line(line), m_isUpper(isUpper) { m_eventType = EventType::EndPoint; }

    const primitives::LineSegment& getLine() const { return m_line; }

    primitives::Point getPosition() const override;

    bool isUpperEndPoint() const { return m_isUpper; }

private:
    bool m_isUpper;
    primitives::LineSegment m_line;
};

} // namespace algorithms

#endif
