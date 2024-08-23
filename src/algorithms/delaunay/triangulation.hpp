#ifndef TRIANGULATION_HPP_INCLUDED
#define TRIANGULATION_HPP_INCLUDED

#include "primitives/point.hpp"
#include "primitives/triangle.hpp"

#include <vector>
#include <map>
#include <set>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

namespace algorithms
{

struct Triangulation 
{
    std::vector<primitives::Point> vertices;
    std::vector<size_t> indices;
};

using Edge = std::pair<size_t, size_t>;

struct DelaunayTriangulator
{
    DelaunayTriangulator(std::vector<primitives::Point> points) : m_vertices(points) {}

    bool isDelaunay() const;

    // Writes triangulation to file in the following format:
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
    void writeTriangulationToFile(fs::path outPath) const;

    void addEdge(size_t v1, size_t v2, bool legalizeAfterInsertion = true);
protected:
    // Use with care; will throw if used on exterior edges, i.e., edges which
    // have only one opposing vertex.
    std::pair<size_t, std::optional<size_t>> getOpposingVerticesToEdge(Edge edge) const;
    Edge flipEdge(Edge edge);
    int legalizeEdges();

    std::vector<primitives::Point> m_vertices;
    std::multimap<size_t, size_t> m_edges;
};

} // namespace algorithms


#endif // TRIANGULATION_HPP_INCLUDED