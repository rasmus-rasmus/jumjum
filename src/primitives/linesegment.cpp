#include "primitives/linesegment.hpp"

#include <glm/geometric.hpp>

#include <iostream>

#include "utility/geomutils.hpp"


namespace primitives
{


enum Orientation
{
    Left = -1,
    On,
    Right,
};

bool areIntervalsOverlapping(std::pair<double, double> firstInterval, std::pair<double, double> secondInterval)
{
    // First ensure that first < second for both intervals, 
    // then ensure that firstInterval is "to the left" of rightInterval.
    if (firstInterval.first > firstInterval.second) std::swap(firstInterval.first, firstInterval.second);
    if (secondInterval.first > secondInterval.second) std::swap(secondInterval.first, secondInterval.second);
    if (firstInterval.first > secondInterval.first)
    {
        std::swap(firstInterval, secondInterval);
    }

    return firstInterval.second >= secondInterval.first && firstInterval.first <= secondInterval.second;
}

Orientation getOrientation(const Point& point, const LineSegment& line)
{
    glm::dvec2 lineDir = line.getDirection();
    glm::dvec2 startPointToPoint{point.x() - line.getStartPoint().x(), point.y() - line.getStartPoint().y()};

    double determinant = lineDir.x * startPointToPoint.y - lineDir.y * startPointToPoint.x;

    if (std::abs(determinant) < 1e-6)
    {
        return Orientation::On;
    }

    return determinant > 0 ? Orientation::Left : Orientation::Right;
}

LineSegment::LineSegment(const Point& startPoint, const Point& endPoint)
{
    if (startPoint.squareDistance(endPoint) < 1e-6)
    {
        throw std::logic_error("Degenerate line segment.");
    }

        m_startPoint = startPoint;
        m_endPoint = endPoint;
}

glm::dvec2 LineSegment::getDirection() const
{
    glm::dvec2 unNormalizedDir(m_endPoint.x() - m_startPoint.x(), m_endPoint.y() - m_startPoint.y());

    return glm::normalize(unNormalizedDir);
}

double LineSegment::length() { return m_startPoint.distance(m_endPoint); }

bool LineSegment::intersects(const LineSegment& otherLine)
{
    if (getOrientation(otherLine.getStartPoint(), *this) == Orientation::On && getOrientation(otherLine.getEndPoint(), *this) == Orientation::On)
    {
        // Colinear lines; intersect iff they overlap
        if (std::abs(m_startPoint.x() - m_endPoint.x()) < 1e-6)
        {
            return areIntervalsOverlapping({m_startPoint.y(), m_endPoint.y()}, {otherLine.getStartPoint().y(), otherLine.getEndPoint().y()});
        }

        return areIntervalsOverlapping({m_startPoint.x(), m_endPoint.x()}, {otherLine.getStartPoint().x(), otherLine.getEndPoint().x()});
    }

    return getOrientation(m_startPoint, otherLine) * getOrientation(m_endPoint, otherLine) <= 0
           && getOrientation(otherLine.getStartPoint(), *this) * getOrientation(otherLine.getEndPoint(), *this) <= 0;
}

LineLineIntersection LineSegment::computeIntersection(const LineSegment& otherLine)
{
    if (!this->intersects(otherLine))
    {
        return std::monostate{};
    }

    glm::dvec2 startPointToOrigo(-getStartPoint().x(), -getStartPoint().y());
    auto firstLineDir = getDirection();
    double angleFirstLineWithXAxis = std::atan2(firstLineDir.y, firstLineDir.x);
    
    auto transformToFirstLineCoords = [startPointToOrigo, angleFirstLineWithXAxis] (glm::dvec2 vecToTransform)
    {
        auto rotationMatrix = utility::getRotationMatrix(angleFirstLineWithXAxis);

        return rotationMatrix * (vecToTransform + startPointToOrigo);
    };
    auto transformBackToGlobalCoordinates = [startPointToOrigo, angleFirstLineWithXAxis] (glm::dvec2 vecToTransform)
    {
        auto rotationMatrix = utility::getRotationMatrix(-angleFirstLineWithXAxis);

        return rotationMatrix * vecToTransform - startPointToOrigo;
    };

    // Transforms second line to firstLine's local coordinate system
    auto secondStartPointTransformed = transformToFirstLineCoords(utility::toGLM(otherLine.getStartPoint()));
    auto secondEndPointTransformed = transformToFirstLineCoords(utility::toGLM(otherLine.getEndPoint()));

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
            glm::dvec2 endPointOut(std::min(length(), std::max(secondStartPointTransformed.x, secondEndPointTransformed.x)), 0.);

            return LineSegment(utility::toPoint(transformBackToGlobalCoordinates(startPointOut)), 
                               utility::toPoint(transformBackToGlobalCoordinates(endPointOut)));
        }
        else
        {
            // general
            zeroX = -intercept / slope;
        }
    }

    // Transforms the intersection point back to the global coordinate system
    glm::dvec2 intersectionInGlobalCoords = transformBackToGlobalCoordinates(glm::dvec2(zeroX, 0.));

    return utility::toPoint(intersectionInGlobalCoords);
}

}
