#include "executables/doctest.h"

#include "algorithms/planesweep/planesweep.hpp"

using namespace algorithms;

TEST_CASE("planesweep::perform")
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