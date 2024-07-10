#ifndef PLANESWEEP_HPP_INCLUDED
#define PLANESWEEP_HPP_INCLUDED

#include <set>
#include <vector>

#include "algorithms/planesweep/events.hpp"
#include "primitives/linesegment.hpp"

namespace algorithms
{

struct Planesweep
{
    Planesweep(const std::vector<primitives::LineSegment>& lines);

    void addLine(const primitives::LineSegment& line);

    bool perform();

    size_t intersectionCount() const { return m_intersections.size(); }
    IntersectionEvent getIntersection(size_t idx) const;

private:
    std::set<Event> m_statusLine;
    std::set<Event> m_eventQueue;
    std::vector<primitives::LineSegment> m_lines;
    std::vector<IntersectionEvent> m_intersections;
    bool m_isDone = false;

    void processNextEvent(); // TODO: Determine best return type
};

} // namespace algorithms

#endif 