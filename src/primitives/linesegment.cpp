#include "linesegment.hpp"

#include <glm/vec2.hpp>


enum Orientation
{
    Left = -1,
    On,
    Right,
};

Orientation getOrientation(const Point& point, const LineSegment& line)
{
    glm::dvec2 lineDir{line.m_endPoint.x() - line.m_startPoint.x(), line.m_endPoint.y() - line.m_startPoint.y()};
    glm::dvec2 startPointToPoint{point.x() - line.m_startPoint.x(), point.y() - line.m_startPoint.y()};

    double determinant = lineDir.x * startPointToPoint.y - lineDir.y * startPointToPoint.x;

    if (std::abs(determinant) < 1e-8)
    {
        return Orientation::On;
    }

    return determinant > 0 ? Orientation::Left : Orientation::Right;
}

// bool LineSegment::intersects(const LineSegment& otherLine, Point& intersectionPoint)
// {

// }