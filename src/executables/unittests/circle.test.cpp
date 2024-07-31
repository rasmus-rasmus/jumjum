#include "executables/doctest.h"

#include "primitives/circle.hpp"
#include "primitives/point.hpp"

#include <vector>

using namespace primitives;

TEST_CASE("Circumcircle")
{
    Point a(-1., 0.);
    Point b(1., 0.);
    std::vector<Point> cPoints{Point(0., 1.),
                               Point(0., .5),
                               Point(0., .1),
                               Point(0., -1.),
                               Point(-.5, .1),
                               Point(-.5, .5),
                               Point(-.5, 1.),
                               Point(-1., .1),
                               Point(-1., .5),
                               Point(-1., 1.),
                               Point(1., .1),
                               Point(1., .5),
                               Point(1., 1.)
                               };
    
    for (auto c : cPoints)
    {
        Circle circle(a, b, c);
        
        CHECK(std::abs(circle.getCenter().distance(a) - circle.getRadius()) < 1e-8);
        CHECK(std::abs(circle.getCenter().distance(b) - circle.getRadius()) < 1e-8);
        CHECK(std::abs(circle.getCenter().distance(c) - circle.getRadius()) < 1e-8);
    }

}