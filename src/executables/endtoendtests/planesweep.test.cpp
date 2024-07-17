#include "executables/doctest.h"

#include "algorithms/planesweep/planesweep.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

using namespace algorithms;

TEST_CASE("planesweep::perform - upper end point, interior point, and lower end point")
{
    std::vector<primitives::LineSegment> 
    lines = {
            primitives::LineSegment(primitives::Point(0., .1), primitives::Point(1., -.1)), 
            primitives::LineSegment(primitives::Point(0, .5), primitives::Point(0, -.5)),
            primitives::LineSegment(primitives::Point(.5, .5), primitives::Point(.5, -.5)),
            primitives::LineSegment(primitives::Point(1., .5), primitives::Point(1., -.5))
            };

    Planesweep algo(lines);

    auto intersections = algo.perform();

    CHECK(intersections.size() == 3);

    CHECK(std::get<primitives::Point>(intersections[0].first.computeIntersection(intersections[0].second)).squareDistance(primitives::Point(0., 0.1)) < 1e-6);
    CHECK(std::get<primitives::Point>(intersections[1].first.computeIntersection(intersections[1].second)).squareDistance(primitives::Point(0.5, 0)) < 1e-6);
    CHECK(std::get<primitives::Point>(intersections[2].first.computeIntersection(intersections[2].second)).squareDistance(primitives::Point(1., -0.1)) < 1e-6);
}

TEST_CASE("planesweep::perform - general case")
{
    // Set working dir to project root.
    auto currPath = std::filesystem::current_path();
    int infiniteLoopGuard = 10;
    while((!std::filesystem::exists(currPath / "README.md") || !std::filesystem::exists(currPath / "LICENSE") || !std::filesystem::exists(currPath / "Makefile"))
           && infiniteLoopGuard)
    {
        currPath = currPath.parent_path();
        --infiniteLoopGuard;
    }

    std::ifstream file(currPath / "src/executables/endtoendtests/testdata/lines.txt");
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open data file.");
    }

    std::vector<primitives::LineSegment> lines;

    std::string dataLine;
    while (std::getline(file, dataLine))
    {
        std::vector<int> pointCoords;
        std::stringstream ss(dataLine);
        std::string pointCoord;
        while (std::getline(ss, pointCoord, ' '))
        {
            pointCoords.push_back(std::stoi(pointCoord));
        }

        lines.push_back(primitives::LineSegment(primitives::Point(pointCoords[0], pointCoords[1]), primitives::Point(pointCoords[2], pointCoords[3])));
    }

    Planesweep algo(lines);

    auto intersections = algo.perform();
}