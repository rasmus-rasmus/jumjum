#include "triangulation.hpp"

namespace algorithms
{

void DelaunayTriangulator::addEdge(size_t v1, size_t v2)
{
    if (v1 == v2)
    {
        throw std::invalid_argument("No degenerate edges allowed.");
    }

    // Add undirected edge
    m_edges.insert({v1, v2});
    m_edges.insert({v2, v1});
}

std::pair<size_t, size_t> DelaunayTriangulator::getOpposingVerticesToEdge(Edge edge) const
{
    std::vector<size_t> opposingVerticesFound;
    auto [startIt, endIt] = m_edges.equal_range(edge.second);
    if (startIt->first != edge.second)
    {
        throw std::logic_error("Edge not found in edge map.");
    }

    for (auto it = startIt; it != endIt; ++it)
    {
        auto [currStartIt, currEndIt] = m_edges.equal_range(it->second);
        for (auto currIt = currStartIt; currIt != currEndIt; ++currIt)
        {
            if (currIt->second == edge.first)
            {
                opposingVerticesFound.push_back(it->second);
                break;
            }
        }
    }

    if (opposingVerticesFound.size() != 2)
    {
        std::string errorMessage = "Expected two opposing vertices. Found: " + std::to_string(opposingVerticesFound.size());
        throw std::logic_error(errorMessage);
    }

    return {opposingVerticesFound[0], opposingVerticesFound[1]};
}

void DelaunayTriangulator::flipEdge(Edge edge)
{
    auto [newEndPoint0, newEndPoint1] = getOpposingVerticesToEdge(edge);

    // Remove old edge. Since the edges are undirected we need to remove its 
    // reversed counterpart as well.
    for (size_t oldEndPoint : {edge.first, edge.second})
    {
        auto [startIt, endIt] = m_edges.equal_range(oldEndPoint);
        for (auto it = startIt; it != endIt; ++it)
        {
            if (it->second == edge.first || it->second == edge.second)
            {
                m_edges.erase(it);
                break;
            }
        }
    }
    
    // Add new edge
    addEdge(newEndPoint0, newEndPoint1);
}

} // namespace algorithms