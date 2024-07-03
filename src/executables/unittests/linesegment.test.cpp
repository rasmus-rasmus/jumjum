#include "../doctest.h"

#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>
#include <cmath>
#include <iostream>

#include "primitives/linesegment.hpp"

TEST_CASE("lineSegment.intersects")
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