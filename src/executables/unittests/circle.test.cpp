#include "executables/doctest.h"

#include "primitives/circle.hpp"
#include "primitives/point.hpp"

#include <vector>
#include <random>
#include <iostream>

using namespace primitives;

TEST_CASE("Circumcircle")
{
    std::mt19937 gen(9876543210);
    std::uniform_real_distribution<double> xDist(-10, 10);
    std::uniform_real_distribution<double> yDist(-10, 10);
    std::uniform_real_distribution<double> radiusDist(.01, 10);
    std::uniform_real_distribution<double> angleDist(0, 2 * M_PI);

    for (int i = 0; i < 100; ++i)
    {
        Point center(xDist(gen), yDist(gen));
        double radius = radiusDist(gen);
        double angle1 = angleDist(gen);
        double angle2 = angleDist(gen);
        double angle3 = angleDist(gen);

        Point p1 = center + Point(radius * cos(angle1), radius * sin(angle1));
        Point p2 = center + Point(radius * cos(angle2), radius * sin(angle2));
        Point p3 = center + Point(radius * cos(angle3), radius * sin(angle3));

        Circle circ(p1, p2, p3);

        CHECK(circ.getCenter().distance(center) < 0.01);
        CHECK(std::abs(circ.getRadius() - radius) < 0.01);
    }
}