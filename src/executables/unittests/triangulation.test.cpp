#include "executables/doctest.h"

#include "algorithms/delaunay/triangulation.hpp"
#include "primitives/point.hpp"


using namespace algorithms;

// Wrapper class to expose protected methods for testing
struct DelaunayTriangulatorTest : DelaunayTriangulator
{

    DelaunayTriangulatorTest() : DelaunayTriangulator(std::vector<primitives::Point>()) {}
    DelaunayTriangulatorTest(std::vector<primitives::Point> points) : DelaunayTriangulator(points) {}
    std::pair<size_t, size_t> getOpposingVerticesToEdge(Edge edge)
    {
        return DelaunayTriangulator::getOpposingVerticesToEdge(edge);
    }

    void addEdge(size_t v1, size_t v2)
    {
        return DelaunayTriangulator::addEdge(v1, v2);
    }

    void flipEdge(Edge edge)
    {
        return DelaunayTriangulator::flipEdge(edge);
    }

    // Helper function for test purposes.
    bool hasEdge(size_t v1, size_t v2)
    {
        auto v1Nbrs = m_edges.equal_range(v1);
        return std::find_if(v1Nbrs.first, v1Nbrs.second, 
                            [v2](const std::pair<size_t, size_t>& edge) { return edge.second == v2; }) 
               != v1Nbrs.second;
    }
};

TEST_CASE("DelaunayTriangulator::addEdge")
{
    DelaunayTriangulatorTest triangulator(std::vector<primitives::Point>{primitives::Point(0, 0), 
                                                                           primitives::Point(1, 0),
                                                                           primitives::Point(1, 1)});

    triangulator.addEdge(0, 1);
    triangulator.addEdge(1, 2);

    CHECK(triangulator.hasEdge(0, 1));
    CHECK(triangulator.hasEdge(1, 0));

    CHECK(triangulator.hasEdge(1, 2));
    CHECK(triangulator.hasEdge(2, 1));

    CHECK(!triangulator.hasEdge(0, 2));
    CHECK(!triangulator.hasEdge(2, 0));

    CHECK_THROWS(triangulator.addEdge(0, 0));
    CHECK_THROWS(triangulator.addEdge(1, 1));
    CHECK_THROWS(triangulator.addEdge(2, 2));
}

TEST_CASE("DelaunayTriangulator::getOpposingVerticesToEdge")
{
    DelaunayTriangulatorTest triangulator(std::vector<primitives::Point>{primitives::Point(0, 0), 
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

    auto opposingVertices = triangulator.getOpposingVerticesToEdge({0, 2});

    CHECK(std::min(opposingVertices.first, opposingVertices.second) == 1);
    CHECK(std::max(opposingVertices.first, opposingVertices.second) == 3);

    CHECK_THROWS(triangulator.getOpposingVerticesToEdge({0, 1}));
}

TEST_CASE("DelaunayTriangulator::flipEdge")
{
    DelaunayTriangulatorTest triangulator(std::vector<primitives::Point>{primitives::Point(0, 0), 
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

    triangulator.flipEdge({0, 2});

    CHECK(triangulator.hasEdge(1, 3));
    CHECK(triangulator.hasEdge(3, 1));
    CHECK(!triangulator.hasEdge(0, 2));
    CHECK(!triangulator.hasEdge(2, 0));
}