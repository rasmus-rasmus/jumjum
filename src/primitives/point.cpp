#include "point.hpp"

#include <cmath>



double Point::squareDistance(const Point& otherPoint) const
{
    double deltaX = m_x - otherPoint.x();
    double deltaY = m_y - otherPoint.y();

    return deltaX * deltaX + deltaY * deltaY;
}

double Point::distance(const Point& otherPoint) const
{
    return std::sqrt(this->squareDistance(otherPoint));
}

bool operator<(const Point& lhs, const Point& rhs)
{
    if (lhs.y() < rhs.y())
    {
        return true;
    }

    return lhs.x() < rhs.x();
}