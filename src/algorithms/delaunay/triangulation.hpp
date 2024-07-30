#ifndef TRIANGULATION_HPP_INCLUDED
#define TRIANGULATION_HPP_INCLUDED

#include "primitives/point.hpp"
#include "primitives/triangle.hpp"

#include <vector>
#include <map>
#include <set>

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

    // Functions for initial testing - delete later
    void addEdge(size_t v1, size_t v2);

protected:
    // Use with care; will throw if used on exterior edges, i.e., edges which
    // have only one opposing vertex.
    std::pair<size_t, size_t> getOpposingVerticesToEdge(Edge edge) const;
    void flipEdge(Edge edge);
    int legalizeEdges();

private:
    std::vector<primitives::Point> m_vertices;
    std::multimap<size_t, size_t> m_edges;
};

} // namespace algorithms


#endif // TRIANGULATION_HPP_INCLUDED