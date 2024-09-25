#include "executables/doctest.h"

#include "algorithms/delaunay/triangulation.hpp"
#include "primitives/point.hpp"
#include "utility/io.hpp"

#include <algorithm>


using namespace algorithms;

// Wrapper class to expose protected methods for testing
struct DelaunayTriangulatorTest : DelaunayTriangulator
{

    DelaunayTriangulatorTest() : DelaunayTriangulator(std::vector<primitives::Point>()) {}
    DelaunayTriangulatorTest(std::vector<primitives::Point> points) : DelaunayTriangulator(points) {}
    std::pair<size_t, std::optional<size_t>> getOpposingVerticesToEdge(Edge edge)
    {
        return DelaunayTriangulator::getOpposingVerticesToEdge(edge);
    }

    Edge flipEdge(Edge edge)
    {
        return DelaunayTriangulator::flipEdge(edge);
    }

    void addEdge(size_t v1, size_t v2, bool legalizeAfterInsertion = true)
    {
        DelaunayTriangulator::addEdge(v1, v2, legalizeAfterInsertion);
    }

    // Helper function for test purposes.
    bool hasEdge(size_t v1, size_t v2) const
    {
        auto v1Nbrs = m_edges.equal_range(v1);
        return std::find_if(v1Nbrs.first, v1Nbrs.second, 
                            [v2](const std::pair<size_t, size_t>& edge) { return edge.second == v2; }) 
               != v1Nbrs.second;
    }

    std::pair<size_t, std::optional<size_t>> getOpposingVerticesToEdge(Edge edge) const
    {
        return DelaunayTriangulator::getOpposingVerticesToEdge(edge);
    }

    int legalizeEdges()
    {
        return DelaunayTriangulator::legalizeEdges();
    }
};

TEST_CASE("DelaunayTriangulator::addEdge")
{
    DelaunayTriangulatorTest triangulator(std::vector<primitives::Point>{primitives::Point(0, 0), 
                                                                           primitives::Point(1, 0),
                                                                           primitives::Point(1, 1)});

    triangulator.addEdge(0, 1, false);
    triangulator.addEdge(1, 2, false);

    CHECK(triangulator.hasEdge(0, 1));
    CHECK(triangulator.hasEdge(1, 0));

    CHECK(triangulator.hasEdge(1, 2));
    CHECK(triangulator.hasEdge(2, 1));

    CHECK(!triangulator.hasEdge(0, 2));
    CHECK(!triangulator.hasEdge(2, 0));

    CHECK_THROWS(triangulator.addEdge(0, 0, false));
    CHECK_THROWS(triangulator.addEdge(1, 1, false));
    CHECK_THROWS(triangulator.addEdge(2, 2, false));
}

TEST_CASE("DelaunayTriangulator::getOpposingVerticesToEdge")
{
    DelaunayTriangulatorTest triangulator(std::vector<primitives::Point>{primitives::Point(0, 0), 
                                                                           primitives::Point(1, 0), 
                                                                           primitives::Point(1, 1),
                                                                           primitives::Point(0, 1)});

    // A square
    triangulator.addEdge(0, 1, false);
    triangulator.addEdge(1, 2, false);
    triangulator.addEdge(2, 3, false);
    triangulator.addEdge(3, 0, false);

    // And its diagonal
    triangulator.addEdge(0, 2, false);

    auto opposingVertices = triangulator.getOpposingVerticesToEdge({0, 2});

    CHECK(std::min(opposingVertices.first, *opposingVertices.second) == 1);
    CHECK(std::max(opposingVertices.first, *opposingVertices.second) == 3);

    CHECK(triangulator.getOpposingVerticesToEdge({0, 1}).second == std::nullopt);
}

TEST_CASE("DelaunayTriangulator::getOpposingVerticesToEdge 2")
{
    DelaunayTriangulatorTest triangulator;
    utility::loadTriangulationFromFile(utility::getProjectRootPath() / "src/executables/testdata/inputTriangulation1.txt", triangulator);

    auto [opposing0, opposing1] = triangulator.getOpposingVerticesToEdge({0, 7});
    auto [opposing2, opposing3] = triangulator.getOpposingVerticesToEdge({1, 7});
    auto [opposing4, opposing5] = triangulator.getOpposingVerticesToEdge({2, 6});
    auto [opposing6, opposing7] = triangulator.getOpposingVerticesToEdge({6, 7});
    auto [opposing8, opposing9] = triangulator.getOpposingVerticesToEdge({0, 8});

    CHECK(opposing1.has_value());
    CHECK(std::min(opposing0, *opposing1) == 1);
    CHECK(std::max(opposing0, *opposing1) == 9);

    CHECK(opposing3.has_value());
    CHECK(std::min(opposing2, *opposing3) == 0);
    CHECK(std::max(opposing2, *opposing3) == 3);

    CHECK(opposing5.has_value());
    CHECK(std::min(opposing4, *opposing5) == 3);
    CHECK(std::max(opposing4, *opposing5) == 5);

    CHECK(!opposing7.has_value());
    CHECK(opposing6 == 3);

    CHECK(!opposing9.has_value());
    CHECK(opposing8 == 2);
}

TEST_CASE("DelaunayTriangulator::flipEdge")
{
    DelaunayTriangulatorTest triangulator(std::vector<primitives::Point>{primitives::Point(0, 0), 
                                                                           primitives::Point(1, 0), 
                                                                           primitives::Point(1, 1),
                                                                           primitives::Point(0, 1)});

    // A square
    triangulator.addEdge(0, 1, false);
    triangulator.addEdge(1, 2, false);
    triangulator.addEdge(2, 3, false);
    triangulator.addEdge(3, 0, false);

    // And its diagonal
    triangulator.addEdge(0, 2, false);

    triangulator.flipEdge({0, 2});

    CHECK(triangulator.hasEdge(1, 3));
    CHECK(triangulator.hasEdge(3, 1));
    CHECK(!triangulator.hasEdge(0, 2));
    CHECK(!triangulator.hasEdge(2, 0));
}

TEST_CASE("DelaunayTriangulator::legalizeEdges")
{
    DelaunayTriangulatorTest triangulator1;
    utility::loadTriangulationFromFile(utility::getProjectRootPath() / "src/executables/testdata/inputTriangulation1.txt", triangulator1);
    auto numLegalizedEdges1 = triangulator1.legalizeEdges();

    DelaunayTriangulatorTest triangulator2;
    utility::loadTriangulationFromFile(utility::getProjectRootPath() / "src/executables/testdata/inputTriangulation2.txt", triangulator2);
    auto numLegalizedEdges2 = triangulator2.legalizeEdges();

    DelaunayTriangulatorTest triangulator3;
    utility::loadTriangulationFromFile(utility::getProjectRootPath() / "src/executables/testdata/inputTriangulation3.txt", triangulator3);
    auto numLegalizedEdges3 = triangulator3.legalizeEdges();

    CHECK(numLegalizedEdges1 == 6);
    CHECK(triangulator1.isDelaunay());

    CHECK(numLegalizedEdges2 == 5);
    CHECK(triangulator2.isDelaunay());

    CHECK(numLegalizedEdges3 == 4);
    CHECK(triangulator3.isDelaunay());
}
