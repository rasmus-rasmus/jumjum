#include "algorithms/planesweep/events.hpp"

#include <cmath>
#include <glm/geometric.hpp>

namespace algorithms
{

bool Event::operator<(const Event& otherEvent)
{
    auto thisPosition = this->getPosition();
    auto otherPosition = otherEvent.getPosition();

    if (thisPosition.squareDistance(otherPosition) < 1e-6)
    {
        // In case of two events happening at the same position, e. g., an intersection
        // in a lower end point event, we want to make sure the intersection event is processed first.
        // Otherwise the line whose end point is intersected will have been removed from the status line
        // by the time the intersection event is processed.
        return this->getType() < otherEvent.getType();
    }

    return thisPosition < otherPosition;
}

IntersectionEvent::IntersectionEvent(primitives::LineSegment line, primitives::LineSegment otherLine)
{
    if (!line.intersects(otherLine))
    {
        throw std::logic_error("Trying to create intersection event with non-intersecting lines.");
    }

    m_firstLine = line;
    m_secondLine = otherLine;
    m_intersection = m_firstLine.computeIntersection(m_secondLine);
    m_eventType = EventType::Intersection;
}

primitives::Point IntersectionEvent::getPosition() const
{
    if (std::holds_alternative<std::monostate>(m_intersection))
    {
        throw std::logic_error("IntersectionEvent contains empty intersection. This shouldn't happen.");
    }

    if (std::holds_alternative<primitives::Point>(m_intersection))
    {
        return std::get<primitives::Point>(m_intersection);
    }

    auto intersectionLine = std::get<primitives::LineSegment>(m_intersection);

    // Probably not correct. TODO: Figure out better solution.
    return intersectionLine.getStartPoint() < intersectionLine.getEndPoint() 
           ? intersectionLine.getStartPoint() 
           : intersectionLine.getEndPoint();
}

primitives::Point EndPointEvent::getPosition() const
{
    if (m_isUpper)
    {
        return m_line.getStartPoint() < m_line.getEndPoint() ? m_line.getStartPoint() : m_line.getEndPoint();
    }

    return m_line.getStartPoint() < m_line.getEndPoint() ? m_line.getEndPoint() : m_line.getStartPoint();
}

} //namespace algorithms
