#include "../doctest.h"
#include <glm/mat2x2.hpp>
#include <iostream>

#include "algorithms/planesweep/events.hpp"

TEST_CASE("IntersectionEvent.getIntersectionPoint")
{
    auto performCheck = [] (int i, LineSegment lineA, LineSegment lineB)
    {
        IntersectionEvent intersection(&lineA, &lineB);

        auto expectedPoint = Point(0., 0.);

        auto result = intersection.getIntersection();

        if (i != 2 && i != 6)
        {
            CHECK_MESSAGE(std::abs(std::get<Point>(result).distance(expectedPoint)) < 1e-6, 
                        "Check failed -- lineB: " << lineB.getStartPoint() 
                                                    << " -> " 
                                                    << lineB.getEndPoint() 
                                                    << ". Intersection point: " 
                                                    << std::get<Point>(result)
                                                    << ". Expected: " << expectedPoint << ".");
        }
        else
        {
            auto intersectionLine = std::get<LineSegment>(result);

            CHECK_MESSAGE(std::abs(intersectionLine.getStartPoint().distance(lineA.getStartPoint())) < 1e-6, 
                        "Check failed -- lineB start point: " << lineB.getStartPoint() 
                                                              << ". Intersection line start point: " 
                                                              << intersectionLine.getStartPoint()
                                                              << ". Expected: " << lineA.getStartPoint() << ".");
            
            CHECK_MESSAGE(std::abs(intersectionLine.getEndPoint().distance(lineA.getEndPoint())) < 1e-6, 
                        "Check failed -- lineB end point: " << lineB.getEndPoint() 
                                                            << ". Intersection line end point: " 
                                                            << intersectionLine.getEndPoint()
                                                            << ". Expected: " << lineA.getEndPoint() << ".");
        }
    };

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

        // Test both orders to test that result is not order dependent
        performCheck(i, lineA, lineB);
        performCheck(i, lineB, lineA);
    }
}