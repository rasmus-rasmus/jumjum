#ifndef LINE_HPP_INCLUDED
#define LINE_HPP_INCLUDED

#include "point.hpp"

#include <stdexcept>

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

    bool intersects(const LineSegment& otherLine, Point& intersectionPoint);

    Point m_startPoint;
    Point m_endPoint;
};

#endif