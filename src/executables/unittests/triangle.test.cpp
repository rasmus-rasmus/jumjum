#include "executables/doctest.h"

#include "primitives/triangle.hpp"

using namespace primitives;

TEST_CASE("Triangle::contains")
{
    Triangle tri(Point(0, 0), Point(1, 0), Point(0, 1));
    Point pointInTri(0.5, 0.1);
    Point pointNotInTri(0.5, 0.6);
    Point pointOnEdge(.5, 0);
    Point pointOnVertex(1., 0.);
    

    CHECK(tri.contains(pointInTri));
    CHECK(!tri.contains(pointNotInTri));
    CHECK(tri.contains(pointOnEdge));
    CHECK(tri.contains(pointOnVertex));
}

