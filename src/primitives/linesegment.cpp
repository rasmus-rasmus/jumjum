#include "primitives/linesegment.hpp"

#include <glm/geometric.hpp>

#include <iostream>


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