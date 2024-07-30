#include "executables/doctest.h"

#include "algorithms/delaunay/triangulation.hpp"
#include "primitives/point.hpp"


using namespace algorithms;

// Wrapper to expose protected methods for testing
struct DelaunayTriangulatorPublic : DelaunayTriangulator
{
    std::pair<size_t, size_t> getOpposingVerticesToEdgePublic(Edge edge)
    {
        return getOpposingVerticesToEdge(edge);
    }
};

TEST_CASE("DelaunayTriangulator")
{
    DelaunayTriangulatorPublic triangulator(std::vector<primitives::Point>{primitives::Point(0, 0), 
                                                                           primitives::Point(1, 0), 
                                                                           primitives::Point(1, 1),
                                                                           primitives::Point(0, 1)});

    // A square
    triangulator.addEdge(0, 1);
    triangulator.addEdge(1, 2);
    triangulator.addEdge(2, 3);
    triangulator.addEdge(3, 0);

    // And its diagonal
    triangulator.addEdge(0, 2);

    auto opposingVertices = triangulator.getOpposingVerticesToEdgePublic({0, 2});

    CHECK(std::min(opposingVertices.first, opposingVertices.second) == 1);
    CHECK(std::max(opposingVertices.first, opposingVertices.second) == 3);

    CHECK_THROWS(triangulator.getOpposingVerticesToEdgePublic({0, 1}));
}