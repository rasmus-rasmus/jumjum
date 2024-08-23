#include <set>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include "primitives/point.hpp"
#include "primitives/triangle.hpp"
#include "algorithms/delaunay/triangulation.hpp"
#include "utility/io.hpp"

// #include "executables/unittests/triangulation.test.cpp"

using namespace algorithms;

struct DelaunayTriangulatorTest : DelaunayTriangulator
{

    DelaunayTriangulatorTest() : DelaunayTriangulator(std::vector<primitives::Point>()) {}
    DelaunayTriangulatorTest(std::vector<primitives::Point> points) : DelaunayTriangulator(points) {}
    std::pair<size_t, std::optional<size_t>> getOpposingVerticesToEdge(Edge edge)
    {
        return DelaunayTriangulator::getOpposingVerticesToEdge(edge);
    }

    void addEdge(size_t v1, size_t v2)
    {
        return DelaunayTriangulator::addEdge(v1, v2);
    }

    Edge flipEdge(Edge edge)
    {
        return DelaunayTriangulator::flipEdge(edge);
    }

    int legalizeEdges()
    {
        return DelaunayTriangulator::legalizeEdges();
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

int main()
{
    // Set working dir to project root.
    auto currPath = std::filesystem::current_path();
    int loopGuard = 10;
    while((!std::filesystem::exists(currPath / "README.md") || !std::filesystem::exists(currPath / "LICENSE") || !std::filesystem::exists(currPath / "Makefile"))
           && loopGuard)
    {
        currPath = currPath.parent_path();
        --loopGuard;
    }

    for (int i = 1; i <= 3; ++i)
    {
        std::string filename = "inputTriangulation" + std::to_string(i) + ".txt";
        DelaunayTriangulatorTest triangulator;
        utility::loadTriangulationFromFile(currPath / "src/executables/correctnesstests/testdata" / filename, triangulator);
        std::cout << "isLegal: " << triangulator.isDelaunay() << std::endl;
        auto numFlips = triangulator.legalizeEdges();
        std::cout << "Number of flips: " << numFlips << std::endl;
        std::cout << "isLegal: " << triangulator.isDelaunay() << std::endl;
    }
    
    return 0;
}
