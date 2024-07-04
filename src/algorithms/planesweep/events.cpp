#include "algorithms/planesweep/events.hpp"

#include <cmath>
#include <glm/geometric.hpp>

#include "utility/geomutils.hpp"

IntersectionEvent::IntersectionEvent(LineSegment line, LineSegment otherLine)
{
    if (!line.intersects(otherLine))
    {
        throw std::logic_error("Trying to create intersection event with non-intersecting lines.");
    }

    m_firstLine = line;
    m_secondLine = otherLine;
}

Point IntersectionEvent::getIntersectionPoint()
{
    if (m_intersectionPoint.has_value())
    {
        return *m_intersectionPoint;
    }

    glm::dvec2 startPointToOrigo(-m_firstLine.getStartPoint().x(), -m_firstLine.getStartPoint().y());
    double angleFirstLineWithXAxis = std::acos(glm::dot(m_firstLine.getDirection(), glm::dvec2(1., 0.)));
    
    auto transformToFirstLineCoords = [startPointToOrigo, angleFirstLineWithXAxis] (glm::dvec2 vecToTransform)
    {
        auto rotationMatrix = getRotationMatrix(angleFirstLineWithXAxis);

        return rotationMatrix * (vecToTransform + startPointToOrigo);
    };

    // Transforms second line to firstLine's local coordinate system
    auto secondStartPointTransformed = transformToFirstLineCoords(toGLM(m_secondLine.getStartPoint()));
    auto secondEndPointTransformed = transformToFirstLineCoords(toGLM(m_secondLine.getEndPoint()));

    // zeroX is the x-coodiante of the intersection point in the first line's local coordinate system.
    // Note that there are three cases to consider:
    // 1. The second line is vertical. In this case, the intersection point is at the x-coordinate of either of the second line's points.
    // 2. The second line is horizontal. In this case, the lines actually intersect in a line segment. By convention we take the leftmost point.
    // 3. The general case. In this case, we calculate the intersection point by solving the linear equation.
    double zeroX = secondEndPointTransformed.x;
    if (std::abs(secondEndPointTransformed.x - secondStartPointTransformed.x) > 1e-5)
    {
        double slope = (secondEndPointTransformed.y - secondStartPointTransformed.y) / (secondEndPointTransformed.x - secondStartPointTransformed.x);
        double intercept = secondStartPointTransformed.y - slope * secondStartPointTransformed.x;

        if (std::abs(slope) < 1e-5)
        {
            zeroX = std::min(secondStartPointTransformed.x, secondEndPointTransformed.x);
        }
        else
        {
            zeroX = -intercept / slope;
        }
    }

    glm::dvec2 intersectionInGlobalCoords = getRotationMatrix(-angleFirstLineWithXAxis) * glm::dvec2(zeroX, 0.) - startPointToOrigo;

    m_intersectionPoint = toPoint(intersectionInGlobalCoords);

    return *m_intersectionPoint;
}