#ifndef PLANESWEEP_HPP_INCLUDED
#define PLANESWEEP_HPP_INCLUDED

#include <vector>

#include "primitives/linesegment.hpp"

namespace algorithms
{

using LinePair = std::pair<primitives::LineSegment, primitives::LineSegment>;

// Simple wrapper class around implementation of planesweep algorithm.
// LineSegments can be added/remoed from the m_lines vector and running
// perform will run a planesweep on the LineSegments currently in m_lines.
struct Planesweep
{
    Planesweep(const std::vector<primitives::LineSegment>& lines);

    std::vector<LinePair> perform();

    std::vector<primitives::LineSegment> m_lines;
};

} // namespace algorithms

#endif 