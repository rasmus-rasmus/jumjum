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

    /// @brief Performs a Delaunay triangulation of the current set of vertices.
    ///        If the triangulation already has edges, they are cleared first.
    ///        If the triangulation fails before finishing, edges are cleared and <searchHierarchy>
    ///        is set to nullopt so as to not leave the triangulator in an invalid state.
    /// @return Whether the triangulation was successful.
    bool performTriangulation();

    /// @brief Checks if all edges are Delaunay by checking whether the triangles formed by their opposing
    ///        vertices have circumcircles that contain no other vertices.
    /// @return Whether all edges are Delaunay or not.
    bool isDelaunay() const;

    /// @brief Legalizes edges in the triangulation, and also "recursively" (though not actually implemented
    /// as a recursive function) legalizes edges that are created as a result of any edge flips on the way.
    /// @param legalizationCandidates Edge candidates to consider first. If empty, all edges are considered.
    /// @return The number of edge flips made during the legalization process.
    int legalizeEdges(std::vector<Edge> legalizationCandidates = {});

    const std::vector<primitives::Point>& getVertices() const { return m_vertices; };
    const std::multimap<size_t, size_t>& getEdges() const { return m_edges; };
    
protected:
    std::vector<primitives::Point> m_vertices;
    std::multimap<size_t, size_t> m_edges;

    // Utility stuff
    std::pair<size_t, std::optional<size_t>> getOpposingVerticesToEdge(Edge edge, bool throwOnDegenerateTris = true) const;
    void addEdge(size_t v1, size_t v2, bool legalizeAfterInsertion = true);
    // NB: throws if called on exterior edge, i. e., edge, which does not have two opposing vertices.
    Edge flipEdge(Edge edge);
    // Only added as a data member to not have to pass it around as a parameter to every function.
    // Should be set to nullopt whenever <performTriangulation> is not running.
    std::optional<TriangleSearchHierarchy> searchHierarchy = std::nullopt;
};

} // namespace algorithms


#endif // TRIANGULATION_HPP_INCLUDED
