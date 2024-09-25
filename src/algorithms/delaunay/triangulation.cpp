#include "triangulation.hpp"
#include "utility/geomutils.hpp"

#include <glm/vec2.hpp>

#include <fstream>
#include <algorithm>

namespace algorithms
{

void DelaunayTriangulator::addEdge(size_t v1, size_t v2, bool legalizeAfterInsertion)
{
    if (v1 == v2)
    {
        throw std::invalid_argument("No degenerate edges allowed.");
    }

    auto v1Nbrs = m_edges.equal_range(v1);
    auto v2Nbrs = m_edges.equal_range(v2);

    // Add undirected edge
    if (std::find_if(v1Nbrs.first, v1Nbrs.second, 
                     [v2](const std::pair<size_t, size_t>& edge) { return edge.second == v2; }) 
        == v1Nbrs.second)
    {
        m_edges.insert({v1, v2});
    }
    if (std::find_if(v2Nbrs.first, v2Nbrs.second, 
                     [v1](const std::pair<size_t, size_t>& edge) { return edge.second == v1; }) 
        == v2Nbrs.second)
    {
        m_edges.insert({v2, v1});
    }

    if (legalizeAfterInsertion)
    {
        legalizeEdges();
    }
}

std::pair<size_t, std::optional<size_t>> DelaunayTriangulator::getOpposingVerticesToEdge(Edge edge) const
{
    auto [startIt, endIt] = m_edges.equal_range(edge.second);
    if (startIt->first != edge.second)
    {
        throw std::logic_error("Edge not found in edge map.");
    }

    glm::dvec2 edgeVector = utility::toGLM(m_vertices[edge.first]) - utility::toGLM(m_vertices[edge.second]);

    std::vector<size_t> rightOrientedCandidates;
    std::vector<size_t> leftOrientedCandidates;
    for (auto it = startIt; it != endIt; ++it)
    {
        auto [currStartIt, currEndIt] = m_edges.equal_range(it->second);
        for (auto currIt = currStartIt; currIt != currEndIt; ++currIt)
        {
            if (currIt->second == edge.first)
            {
                // Figure out whether candidate edge is to the left or right of the input edge.
                glm::dvec2 candEdgeVector = utility::toGLM(m_vertices[currIt->first]) - utility::toGLM(m_vertices[edge.second]);
                double orientation = edgeVector.x * candEdgeVector.y - edgeVector.y * candEdgeVector.x;
                if (orientation > 0)
                {
                    rightOrientedCandidates.push_back(currIt->first);
                }
                else if (orientation < 0)
                {
                    leftOrientedCandidates.push_back(currIt->first);
                }
                else
                {
                    throw std::logic_error("Degenerate triangle found.");
                }    
            }
        }
    }

    if (rightOrientedCandidates.size() < 1 && leftOrientedCandidates.size() < 1)
    {
        throw std::logic_error("No opposing vertices found.");
    }

    auto opposingLeft = leftOrientedCandidates.size() > 0 
                      ? *std::min_element(leftOrientedCandidates.begin(), leftOrientedCandidates.end(),
                        [this, edge, edgeVector](size_t v1, size_t v2) 
                        { 
                            return utility::getAngle(edgeVector, utility::toGLM(m_vertices[v1]) - utility::toGLM(m_vertices[edge.second])) 
                                    < utility::getAngle(edgeVector, utility::toGLM(m_vertices[v2]) - utility::toGLM(m_vertices[edge.second]));
                        })
                      : -1;
    auto opposingRight = rightOrientedCandidates.size() > 0
                       ? *std::min_element(rightOrientedCandidates.begin(), rightOrientedCandidates.end(),
                          [this, edge, edgeVector](size_t v1, size_t v2) 
                          { 
                              return utility::getAngle(edgeVector, utility::toGLM(m_vertices[v1]) - utility::toGLM(m_vertices[edge.second])) 
                                     < utility::getAngle(edgeVector, utility::toGLM(m_vertices[v2]) - utility::toGLM(m_vertices[edge.second]));
                          })
                        : -1;

    if (leftOrientedCandidates.size() < 1)
    {
        return {opposingRight, std::nullopt};
    }
    if (rightOrientedCandidates.size() < 1)
    {
        return {opposingLeft, std::nullopt};
    }

    return {opposingLeft, opposingRight};
}

Edge DelaunayTriangulator::flipEdge(Edge edge)
{
    auto [newEndPoint0, newEndPoint1] = getOpposingVerticesToEdge(edge);

    if (!newEndPoint1.has_value())
    {
        throw std::invalid_argument("Cannot flip exterior edge.");
    }

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
    addEdge(newEndPoint0, *newEndPoint1, false);

    return {newEndPoint0, *newEndPoint1};
}

int DelaunayTriangulator::legalizeEdges()
{
    int numLegalizedEdges = 0;
    std::set<Edge> edgesToLegalize;
    auto insertEdgeIfNotLegal = [this, &edgesToLegalize](Edge edge)
    {
        auto [opposingV1, opposingV2] = getOpposingVerticesToEdge(edge);
        if (opposingV2.has_value() 
            && primitives::Triangle(m_vertices[edge.first], m_vertices[edge.second], m_vertices[opposingV1]).getCircumcircle().contains(m_vertices[*opposingV2]))
        {
            edgesToLegalize.insert({std::min(edge.first, edge.second), std::max(edge.first, edge.second)});
        }
    };

    for (const auto& [v1, v2] : m_edges)
    {
        insertEdgeIfNotLegal({v1, v2});
    }

    while (!edgesToLegalize.empty())
    {
        ++numLegalizedEdges;
        auto edgeToFlip = *edgesToLegalize.begin();
        edgesToLegalize.erase(edgesToLegalize.begin());

        auto flippedEdge = flipEdge(edgeToFlip);

        insertEdgeIfNotLegal({flippedEdge.first, edgeToFlip.first});
        insertEdgeIfNotLegal({flippedEdge.first, edgeToFlip.second});
        insertEdgeIfNotLegal({flippedEdge.second, edgeToFlip.first});
        insertEdgeIfNotLegal({flippedEdge.second, edgeToFlip.second});
    }

    return numLegalizedEdges;
}

bool DelaunayTriangulator::isDelaunay() const
{
    for (const auto& [v1, v2] : m_edges)
    {
        auto [opposingV1, opposingV2] = getOpposingVerticesToEdge({v1, v2});
        if (opposingV2.has_value() 
            && primitives::Triangle(m_vertices[v1], m_vertices[v2], m_vertices[opposingV1]).getCircumcircle().contains(m_vertices[*opposingV2]))
        {
            return false;
        }
    }

    return true;
}



} // namespace algorithms