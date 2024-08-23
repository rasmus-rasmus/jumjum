#ifndef IO_HPP_INCLUDED
#define IO_HPP_INCLUDED

#include "algorithms/delaunay/triangulation.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace utility
{

fs::path getProjectRootPath(int loopGuard = 10);


// Note: Assumes input file has the following format:
// <numPoints> <numTris>
// 0 <point0.x> <point0.y>
// 1 <point1.x> <point1.y>
// ...
// <numPoints-1> <point(numPoints-1).x> <point(numPoints-1).y>
// <tri0.v0> <tri0.v1> <tri0.v2>
// <tri1.v0> <tri1.v1> <tri1.v2>
// ...
// <tri(numTris-1).v0> <tri(numTris-1).v1> <tri(numTris-1).v2>
//
// where v0, v1 and v2 are indices of vertices in the point list.
void loadTriangulationFromFile(fs::path inPath, algorithms::DelaunayTriangulator& triangulation);

} // namespace utility

#endif // IO_HPP_INCLUDED