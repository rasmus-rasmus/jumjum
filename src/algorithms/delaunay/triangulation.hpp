#ifndef TRIANGULATION_HPP_INCLUDED
#define TRIANGULATION_HPP_INCLUDED

#include "primitives/point.hpp"
#include "primitives/triangle.hpp"
#include "trianglesearch.hpp"

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
    DelaunayTriangulator() = default;
    DelaunayTriangulator(std::vector<primitives::Point> points) : m_vertices(points) {}

    bool performTriangulation();

    bool isDelaunay() const;
    int legalizeEdges(std::vector<Edge> legalizationCandidates = {});

    const std::vector<primitives::Point>& getVertices() const { return m_vertices; };
    const std::multimap<size_t, size_t>& getEdges() const { return m_edges; };
    
protected:
    // Utility stuff
    std::pair<size_t, std::optional<size_t>> getOpposingVerticesToEdge(Edge edge, bool throwOnDegenerateTris = true) const;
    void addEdge(size_t v1, size_t v2, bool legalizeAfterInsertion = true);
    Edge flipEdge(Edge edge);
    std::optional<TriangleSearchHierarchy> searchHierarchy;

    std::vector<primitives::Point> m_vertices;
    std::multimap<size_t, size_t> m_edges;
};

} // namespace algorithms


#endif // TRIANGULATION_HPP_INCLUDED
