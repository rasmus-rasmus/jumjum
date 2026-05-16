#include "executables/doctest.h"

#include "primitives/triangle.hpp"
#include "primitives/polygon.hpp"

using namespace primitives;

TEST_CASE("Triangle::contains")
{
    Triangle tri(Point(0, 0), Point(1, 0), Point(0, 1));
    Point pointInTri(0.5, 0.1);
    Point pointNotInTri(0.5, 0.6);
    Point pointOnEdge(.5, 0);
    Point pointOnVertex(1., 0.);
    Point pointOnLineSpannedByVertex(2, 0);
    
    
    CHECK(tri.contains(pointInTri));
    CHECK(!tri.contains(pointNotInTri));
    CHECK(tri.contains(pointOnEdge));
    CHECK(tri.contains(pointOnVertex));
    CHECK(!tri.contains(pointOnLineSpannedByVertex));
    
    // Hacky way to smoke test polygon logic
    Polygon pol({tri.getPoint1(), tri.getPoint2(), tri.getPoint3()});
    CHECK(pol.contains(pointInTri));
    CHECK(!pol.contains(pointNotInTri));
    CHECK(pol.contains(pointOnEdge));
    CHECK(pol.contains(pointOnVertex));
    CHECK(!pol.contains(pointOnLineSpannedByVertex));
}

