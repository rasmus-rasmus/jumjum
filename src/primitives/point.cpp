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

double Point::squareNorm() const
{
    return m_x * m_x + m_y * m_y;
}

double Point::norm() const
{
    return std::sqrt(squareNorm());
}

bool operator<(const Point& lhs, const Point& rhs)
{
    if (std::abs(lhs.y() - rhs.y()) < 1e-6)
    {
        return lhs.x() < rhs.x();
    }
    return lhs.y() > rhs.y();
}

std::ostream& operator<<(std::ostream& ost, const Point& point)
{
    return ost << "Point: [" << point.x() << ", " << point.y() << "]";
}

Point Point::operator+(const Point& otherPoint)
{
    return Point(m_x + otherPoint.x(), m_y + otherPoint.y());
}


} //namespace primitives
