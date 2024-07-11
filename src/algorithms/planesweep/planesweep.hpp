#ifndef PLANESWEEP_HPP_INCLUDED
#define PLANESWEEP_HPP_INCLUDED

#include <vector>
#include <map>

#include "primitives/linesegment.hpp"

namespace algorithms
{

using LinePair = std::pair<primitives::LineSegment, primitives::LineSegment>;

struct Planesweep
{
    Planesweep(const std::vector<primitives::LineSegment>& lines);

    std::multimap<primitives::LineLineIntersection, LinePair> perform();

    std::vector<primitives::LineSegment> m_lines;
};

} // namespace algorithms

#endif 