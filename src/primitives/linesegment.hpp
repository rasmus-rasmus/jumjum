#ifndef LINE_HPP_INCLUDED
#define LINE_HPP_INCLUDED

#include "point.hpp"

#include <stdexcept>
#include <glm/vec2.hpp>

struct LineSegment
{
    LineSegment(const Point& startPoint, const Point& endPoint)
    {
        if (startPoint.squareDistance(endPoint) < 1e-6)
        {
            throw std::logic_error("Degenerate line segment.");
        }

         m_startPoint = startPoint;
         m_endPoint = endPoint;
    }

    Point getStartPoint() const { return m_startPoint; }
    Point getEndPoint() const { return m_endPoint; }

    glm::dvec2 getDirection() const;

    bool intersects(const LineSegment& otherLine);

private:
    Point m_startPoint;
    Point m_endPoint;
};

#endif