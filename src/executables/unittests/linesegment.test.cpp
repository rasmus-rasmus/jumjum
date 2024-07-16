#include "executables/doctest.h"

#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>
#include <cmath>
#include <iostream>

#include "primitives/linesegment.hpp"

using namespace primitives;

TEST_CASE("LineSegment::intersects")
{
    LineSegment refLine(Point(-1., 0.), Point(1., 0.));
    LineSegment refLinePerp(Point(0., -1.), Point(0., 1.));
    glm::dvec2 testStartPoint(0., -.5);
    glm::dvec2 testEndPoint(0., .5);

    for (int transIdx = -2; transIdx < 3; ++transIdx)
    {
        for (int rotIdx = 0; rotIdx < 8; ++rotIdx)
        {
            double rotAngle = rotIdx * 2 * M_PI / 8;
            glm::dmat2 rotationMat(cos(rotAngle), sin(rotAngle), -sin(rotAngle), cos(rotAngle));
            glm::dvec2 translationVec((double)transIdx, 0.);
            auto rotatedStartPoint = rotationMat * testStartPoint + translationVec;
            auto rotatedEndPoint = rotationMat * testEndPoint + translationVec;
            LineSegment testLine(Point(rotatedStartPoint.x, rotatedStartPoint.y), 
                                 Point(rotatedEndPoint.x, rotatedEndPoint.y));

            glm::dvec2 rotatedStartPointPerp(-rotatedStartPoint.y, rotatedStartPoint.x);
            glm::dvec2 rotatedEndPointPerp(-rotatedEndPoint.y, rotatedEndPoint.x);
            LineSegment testLinePerp(Point(rotatedStartPointPerp.x, rotatedStartPointPerp.y),
                                     Point(rotatedEndPointPerp.x, rotatedEndPointPerp.y));

            CHECK(refLine.intersects(testLine) == (std::abs(transIdx) != 2));
            // Primarily to also check correct intersection results of vertical lines.
            CHECK(refLinePerp.intersects(testLinePerp) == (std::abs(transIdx) != 2));
        }
    }
}

TEST_CASE("LineSegment::computeIntersection")
{
    auto performCheck = [] (int i, LineSegment lineA, LineSegment lineB)
    {
        auto expectedPoint = Point(0., 0.);
        auto result = lineA.computeIntersection(lineB);

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

TEST_CASE("Intersection in end points")
{
    LineSegment lineA(Point(0., .1), Point(1., 0.));
    LineSegment lineB(Point(1., .5), Point(1., -.5));

    CHECK(lineA.intersects(lineB));

    auto result = lineA.computeIntersection(lineB);

    CHECK(std::holds_alternative<Point>(result));

    CHECK(std::get<Point>(result).distance(Point(1., 0.)) < 1e-6);
}
