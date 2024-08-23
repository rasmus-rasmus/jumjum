#include "io.hpp"

#include <fstream>

namespace utility
{

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

    algorithms::DelaunayTriangulator newTriangulation(points);
    triangulation = std::move(newTriangulation);
    for (const auto& tri : triangles)
    {
        triangulation.addEdge(tri[0], tri[1], false);
        triangulation.addEdge(tri[1], tri[2], false);
        triangulation.addEdge(tri[2], tri[0], false);
    }
}


} // namespace utility

