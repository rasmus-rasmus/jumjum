#include "executables/doctest.h"

#include "primitives/polygon.hpp"


using namespace primitives;

TEST_CASE("Polygon::contains smoke test")
{
    // Simple smoke test; copied from triangle.test.cpp
    Polygon tri({Point(0, 0), Point(1, 0), Point(0, 1)});
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
}

TEST_CASE("Polygon::contains chevron shape")
{
    Polygon chevron({
        Point(0, 0),
        Point(-1, 2),
        Point(0, 1),
        Point(1, 2)
    });
    Point pointInChev(0, .5);
    Point pointBetweenTails(0, 1.5);
    Point pointToTheLeft(-1, 1);
    Point pointToTheRight(1, 1);
    Point pointInLeftTail(-.75, 1.5);
    Point pointInRightTail(.75, 1.5);

    CHECK(chevron.contains(pointInChev));
    CHECK(!chevron.contains(pointBetweenTails));
    CHECK(!chevron.contains(pointToTheLeft));
    CHECK(!chevron.contains(pointToTheRight));
    CHECK(chevron.contains(pointInLeftTail));
    CHECK(chevron.contains(pointInRightTail));
}
