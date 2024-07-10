#include "primitives/point.hpp"

#include <cmath>

namespace primitives
{


double Point::squareDistance(const Point& otherPoint) const
{
    double deltaX = m_x - otherPoint.x();
    double deltaY = m_y - otherPoint.y();

    return deltaX * deltaX + deltaY * deltaY;
}

double Point::distance(const Point& otherPoint) const
{
    return std::sqrt(squareDistance(otherPoint));
}

bool operator<(const Point& lhs, const Point& rhs)
{
    if (lhs.y() > rhs.y())
    {
        return true;
    }

    return lhs.x() < rhs.x();
}

std::ostream& operator<<(std::ostream& ost, const Point& point)
{
    return ost << "Point: [" << point.x() << ", " << point.y() << "]";
}

} //namespace primitives
