#include "algorithms/planesweep/events.hpp"

#include <cmath>
#include <glm/geometric.hpp>

#include "utility/geomutils.hpp"

IntersectionEvent::IntersectionEvent(LineSegment* line, LineSegment* otherLine)
{
    if (!line->intersects(*otherLine))
    {
        throw std::logic_error("Trying to create intersection event with non-intersecting lines.");
    }

    m_firstLine = line;
    m_secondLine = otherLine;
}

LineLineIntersection IntersectionEvent::getIntersection()
{
    if (m_intersection.has_value())
    {
        return *m_intersection;
    }

    glm::dvec2 startPointToOrigo(-m_firstLine->getStartPoint().x(), -m_firstLine->getStartPoint().y());
    auto firstLineDir = m_firstLine->getDirection();
    double angleFirstLineWithXAxis = std::atan2(firstLineDir.y, firstLineDir.x);
    
    auto transformToFirstLineCoords = [startPointToOrigo, angleFirstLineWithXAxis] (glm::dvec2 vecToTransform)
    {
        auto rotationMatrix = getRotationMatrix(angleFirstLineWithXAxis);

        return rotationMatrix * (vecToTransform + startPointToOrigo);
    };
    auto transformBackToGlobalCoordinates = [startPointToOrigo, angleFirstLineWithXAxis] (glm::dvec2 vecToTransform)
    {
        auto rotationMatrix = getRotationMatrix(-angleFirstLineWithXAxis);

        return rotationMatrix * vecToTransform - startPointToOrigo;
    };

    // Transforms second line to firstLine's local coordinate system
    auto secondStartPointTransformed = transformToFirstLineCoords(toGLM(m_secondLine->getStartPoint()));
    auto secondEndPointTransformed = transformToFirstLineCoords(toGLM(m_secondLine->getEndPoint()));

    // zeroX is the x-coodiante of the intersection point in the first line's local coordinate system.
    // Note that there are three cases to consider:
    // 1. The second line is vertical. In this case, the intersection point is at the x-coordinate of either of the second line's points.
    // 2. The second line is horizontal. In this case, the lines actually intersect in a line segment. By convention we take the leftmost point.
    // 3. The general case. In this case, we calculate the intersection point by solving the linear equation.
    double zeroX = secondEndPointTransformed.x; //<- vertical
    if (std::abs(secondEndPointTransformed.x - secondStartPointTransformed.x) > 1e-5)
    {
        double slope = (secondEndPointTransformed.y - secondStartPointTransformed.y) / (secondEndPointTransformed.x - secondStartPointTransformed.x);
        double intercept = secondStartPointTransformed.y - slope * secondStartPointTransformed.x;

        if (std::abs(slope) < 1e-5)
        {
            // horizontal
            glm::dvec2 startPointOut(std::max(0., std::min(secondStartPointTransformed.x, secondEndPointTransformed.x)), 0.);
            glm::dvec2 endPointOut(std::min(m_firstLine->length(), std::max(secondStartPointTransformed.x, secondEndPointTransformed.x)), 0.);

            return LineSegment(toPoint(transformBackToGlobalCoordinates(startPointOut)), 
                               toPoint(transformBackToGlobalCoordinates(endPointOut)));
        }
        else
        {
            // general
            zeroX = -intercept / slope;
        }
    }

    // Transforms the intersection point back to the global coordinate system
    glm::dvec2 intersectionInGlobalCoords = transformBackToGlobalCoordinates(glm::dvec2(zeroX, 0.));

    m_intersection = toPoint(intersectionInGlobalCoords);

    return *m_intersection;
}

Point EndPointEvent::getPoint()
{
    if (m_isUpper)
    {
        return m_line->getStartPoint() < m_line->getEndPoint() ? m_line->getStartPoint() : m_line->getEndPoint();
    }

    return m_line->getStartPoint() < m_line->getEndPoint() ? m_line->getEndPoint() : m_line->getStartPoint();
}
