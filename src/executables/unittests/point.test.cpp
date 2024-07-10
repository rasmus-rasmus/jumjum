#include "executables/doctest.h"

#include "primitives/point.hpp"

using namespace primitives;

TEST_CASE("Point comparison operator")
{
    CHECK(Point(0., 1.) < Point(1., 1.));
    CHECK(Point(0., 1.) < Point(-1., 0.));
    CHECK(Point(0., 1.) < Point(0., 0.));
    CHECK(Point(0., 1.) < Point(1., 0.));
}

