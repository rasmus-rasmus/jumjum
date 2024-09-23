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
    std::set<int> mySet;

    {

        int myInt(666);

        mySet.insert(myInt);

        auto insertedInt = mySet.find(myInt);

        std::cout << "myInt: " << &myInt << " -- " << "insertedInt: " << &*insertedInt << std::endl;
    }

    auto foundInt = mySet.find(666);

    std::cout << "foundInt: " << &*foundInt << std::endl;
    
    return 0;
}
