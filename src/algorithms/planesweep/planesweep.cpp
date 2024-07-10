#include "planesweep.hpp"

namespace algorithms
{

Planesweep::Planesweep(const std::vector<primitives::LineSegment>& lines)
{
    for (auto line : lines)
    {
        addLine(line);   
    }
}

void Planesweep::addLine(const primitives::LineSegment& line)
{
    m_lines.push_back(line);

    // Insert EndPointEvent once for each end point with isUpper set resp not set. 
    // The EndPointEvent will take care of returning the relevant end point for comparison
    // based on whether it is upper or not.
    m_eventQueue.insert(EndPointEvent(&m_lines.back(), true));
    m_eventQueue.insert(EndPointEvent(&m_lines.back(), false));

    m_isDone = false;
}

IntersectionEvent Planesweep::getIntersection(size_t idx) const
{
    if (idx >= intersectionCount())
    {
        throw std::logic_error("Intersection index out of range.");
    }

    return m_intersections[idx];
}

} // namespace algorithms