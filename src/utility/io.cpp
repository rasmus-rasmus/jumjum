#include "io.hpp"

#include <fstream>
#include <array>

namespace utility
{

fs::path getProjectRootPath(int loopGuard)
{
    // Set working dir to project root.
    auto currPath = std::filesystem::current_path();
    while((!std::filesystem::exists(currPath / "README.md") || !std::filesystem::exists(currPath / "LICENSE") || !std::filesystem::exists(currPath / "Makefile"))
           && loopGuard)
    {
        currPath = currPath.parent_path();
        --loopGuard;
    }

    return currPath;
}

namespace priv
{

// A bit of a hack, admittedly, but it's mainly a test utility.
struct DelaunayTriangulatorIO : algorithms::DelaunayTriangulator
{
    DelaunayTriangulatorIO(std::vector<primitives::Point> points) : DelaunayTriangulator(points) {}
    DelaunayTriangulatorIO(const algorithms::DelaunayTriangulator& triangulator) : DelaunayTriangulator(triangulator.getVertices())
    {
        for (const auto& [v1, v2] : triangulator.getEdges())
        {
            m_edges.insert({v1, v2});
        }
    }

    void addEdge(size_t v1, size_t v2, bool legalizeAfterInsertion = true)
    {
        DelaunayTriangulator::addEdge(v1, v2, legalizeAfterInsertion);
    }

    std::pair<size_t, std::optional<size_t>> getOpposingVerticesToEdge(algorithms::Edge edge, bool throwOnDegenerateTris = true) const
    {
        return DelaunayTriangulator::getOpposingVerticesToEdge(edge, throwOnDegenerateTris);
    }
};

}

void loadTriangulationFromFile(fs::path inPath, algorithms::DelaunayTriangulator& triangulation)
{
    std::ifstream file(inPath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open data file.");
    }

    std::string dataLine;
    std::getline(file, dataLine);
    std::stringstream ss(dataLine);
    std::string numPointsStr, numTrisStr;
    std::getline(ss, numPointsStr, ' ');
    int numPoints = std::stoi(numPointsStr);
    std::getline(ss, numTrisStr, ' ');
    int numTris = std::stoi(numTrisStr);

    std::string dummyStr;
    if (std::getline(ss, dummyStr, ' '))
    {
        std::string errorMessage = "Invalid input file format. Expected two integers in first line. Got third: " + dummyStr;
        throw std::logic_error(errorMessage);
    }

    std::vector<primitives::Point> points;
    std::vector<std::array<int, 3>> triangles;

    while (std::getline(file, dataLine))
    {
        std::vector<int> intsOnLine;
        std::stringstream ss(dataLine);
        std::string val;
        while (std::getline(ss, val, ' '))
        {
            intsOnLine.push_back(std::stoi(val));
        }

        if (points.size() < numPoints)
        {
            points.push_back(primitives::Point(intsOnLine[1], intsOnLine[2]));
        }
        else
        {
            triangles.push_back(std::array<int, 3>{intsOnLine[0], intsOnLine[1], intsOnLine[2]});
        }
    }

    priv::DelaunayTriangulatorIO newTriangulation(points);
    for (const auto& tri : triangles)
    {
        newTriangulation.addEdge(tri[0], tri[1], false);
        newTriangulation.addEdge(tri[1], tri[2], false);
        newTriangulation.addEdge(tri[2], tri[0], false);
    }
    triangulation = std::move(newTriangulation);
}

void writeTriangulationToFile(const algorithms::DelaunayTriangulator& triangulation, fs::path outPath)
{
    priv::DelaunayTriangulatorIO newTriangulation(triangulation);
    std::ofstream oStream(outPath);
    if (!oStream.is_open())
    {
        throw std::runtime_error("Could not open file for writing.");
    }

    std::set<std::set<size_t>> triangles;
    for (const auto& [v1, v2] : newTriangulation.getEdges())
    {
        auto [opposingV1, opposingV2] = newTriangulation.getOpposingVerticesToEdge({v1, v2}, false);
        triangles.insert({v1, v2, opposingV1});
        if (opposingV2.has_value())
        {
            triangles.insert({v1, v2, *opposingV2});
        }
    }
    
    oStream << newTriangulation.getVertices().size() << " " << triangles.size() << std::endl;

    for (auto vIdx = 0; vIdx < newTriangulation.getVertices().size(); ++vIdx)
    {
        oStream << vIdx << " " << newTriangulation.getVertices()[vIdx].x() << " " << newTriangulation.getVertices()[vIdx].y() << std::endl;
    }
    for (const auto& tri : triangles)
    {
        oStream << *tri.begin() << " " << *(++tri.begin()) << " " << *(++(++tri.begin())) << std::endl;
    }

    oStream.close();
}

} // namespace utility

