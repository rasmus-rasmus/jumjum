#include "linesegment.hpp"

#include <glm/geometric.hpp>

#include <iostream>


enum Orientation
{
    Left = -1,
    On,
    Right,
};

Orientation getOrientation(const Point& point, const LineSegment& line)
{
    // std::cout << "Testing getOrientation with point: " << point << " and line: " << line.getStartPoint() << " -> " << line.getEndPoint() << std::endl;
    glm::dvec2 lineDir = line.getDirection();
    glm::dvec2 startPointToPoint{point.x() - line.getStartPoint().x(), point.y() - line.getStartPoint().y()};

    double determinant = lineDir.x * startPointToPoint.y - lineDir.y * startPointToPoint.x;

    if (std::abs(determinant) < 1e-8)
    {
        return Orientation::On;
    }

    return determinant > 0 ? Orientation::Left : Orientation::Right;
}

glm::dvec2 LineSegment::getDirection() const
{
    glm::dvec2 unNormalizedDir(m_endPoint.x() - m_startPoint.x(), m_endPoint.y() - m_startPoint.y());

    return glm::normalize(unNormalizedDir);
}

// bool LineSegment::intersects(const LineSegment& otherLine, Point& intersectionPoint)
// {

// }