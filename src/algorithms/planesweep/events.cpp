#include "algorithms/planesweep/events.hpp"

#include <cmath>
#include <glm/geometric.hpp>

namespace algorithms
{

bool operator<(const Event& lhs, const Event& rhs)
{
    return lhs.getPoint() < rhs.getPoint();
}


IntersectionEvent::IntersectionEvent(primitives::LineSegment* line, primitives::LineSegment* otherLine)
{
    if (!line->intersects(*otherLine))
    {
        throw std::logic_error("Trying to create intersection event with non-intersecting lines.");
    }

    m_firstLine = line;
    m_secondLine = otherLine;
    m_intersection = m_firstLine->computeIntersection(*m_secondLine);
}

primitives::Point IntersectionEvent::getPoint() const
{
    if (std::holds_alternative<primitives::Point>(m_intersection))
    {
        return std::get<primitives::Point>(m_intersection);
    }

    auto intersectionLine = std::get<primitives::LineSegment>(m_intersection);

    return intersectionLine.getStartPoint() < intersectionLine.getEndPoint() 
           ? intersectionLine.getStartPoint() 
           : intersectionLine.getEndPoint();
}

primitives::Point EndPointEvent::getPoint() const
{
    if (m_isUpper)
    {
        return m_line->getStartPoint() < m_line->getEndPoint() ? m_line->getStartPoint() : m_line->getEndPoint();
    }

    return m_line->getStartPoint() < m_line->getEndPoint() ? m_line->getEndPoint() : m_line->getStartPoint();
}

} //namespace algorithms
