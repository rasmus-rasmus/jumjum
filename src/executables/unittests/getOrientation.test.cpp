#include <iostream>
#include <vector>
#include <cmath>

#include <glm/mat2x2.hpp>

#include "../doctest.h"
#include "primitives/linesegment.hpp"
#include "primitives/linesegment.cpp"

TEST_CASE("getOrientation") 
{
    /*
        Simple test. Buid some lines with start points in every quadrant with end points in eight equi-distributed directions from start points.
        Then take test points in a 3x5 grid around the line (with 3 interior points of grid lying on the line) and test their orientation.
    */
    glm::dvec2 deltaVec(1., 0.);

    for (int i = 0; i < 8; ++i)
    {
        double startPointRotationAngle = i * 2 * M_PI / 8;
        glm::dmat2 startPointRotationMat(cos(startPointRotationAngle), sin(startPointRotationAngle), -sin(startPointRotationAngle), cos(startPointRotationAngle));
        glm::dvec2 startPoint = glm::dvec2(0., 0.) + startPointRotationMat * deltaVec;
        
        for (int j = 0; j < 8; ++j)
        {
            double endPointRotationAngle = j * 2 * M_PI / 8;
            glm::dmat2 endPointRotationMat(cos(endPointRotationAngle), sin(endPointRotationAngle), -sin(endPointRotationAngle), cos(endPointRotationAngle));
            glm::dvec2 endPoint = startPoint + endPointRotationMat * deltaVec;
            
            LineSegment testLine(Point(startPoint.x, startPoint.y), Point(endPoint.x, endPoint.y));
            auto lineDir = testLine.getDirection();
            glm::dvec2 lineDirPerp(-lineDir.y, lineDir.x);
            
            for (double lineDirScalar : std::vector<double>{-.5, 0, .5, 1, 1.5})
            {
                for (int lineDirPerpScalar : std::vector<int>{-1, 0, 1})
                {
                    auto v = startPoint + lineDirScalar * lineDir + (double)lineDirPerpScalar * lineDirPerp;
                    Point testPoint(v.x, v.y);
                    auto orientation = getOrientation(testPoint, testLine);

                    CHECK(orientation == -lineDirPerpScalar);
                }
            }
        }
    }
}