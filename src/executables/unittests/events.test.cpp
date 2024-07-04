#include "../doctest.h"
#include <glm/mat2x2.hpp>
#include <iostream>

#include "algorithms/planesweep/events.hpp"

TEST_CASE("IntersectionEvent.getIntersectionPoint")
{
    glm::dvec2 startVecA(-1., -1.);
    glm::dvec2 endVecA(1., 1.);
    glm::dvec2 startVecB(-1., 1.);
    glm::dvec2 endVecB(1., -1.);

    for (int i = 0; i < 8; ++i)
    {
        double rotationAngle = i * 2 * M_PI / 8;
        glm::dmat2 rotationMat(cos(rotationAngle), sin(rotationAngle), -sin(rotationAngle), cos(rotationAngle));

        glm::dvec2 startPointB = rotationMat * startVecB;
        glm::dvec2 endPointB = rotationMat * endVecB;

        LineSegment lineA(Point(startVecA.x, startVecA.y), Point(endVecA.x, endVecA.y));
        LineSegment lineB(Point(startPointB.x, startPointB.y), Point(endPointB.x, endPointB.y));

        IntersectionEvent intersection(lineA, lineB);

        auto expectedPoint = i != 2 && i != 6 ? Point(0., 0.) : Point(-1., -1.);

        CHECK_MESSAGE(std::abs(intersection.getIntersectionPoint().distance(expectedPoint)) < 1e-6, 
                      "Check failed -- lineB: " << lineB.getStartPoint() 
                                                << " -> " 
                                                << lineB.getEndPoint() 
                                                << ". Intersection point: " 
                                                << intersection.getIntersectionPoint() 
                                                << ". Expected: " << expectedPoint << ".");
    }
}