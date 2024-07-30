#ifndef POINT_HPP_INCLUDED
#define POINT_HPP_INCLUDED

#include <ostream>

namespace primitives
{

struct Point
{
    Point() : m_x(0.), m_y(0.) {}
    Point(double x, double y) : m_x(x), m_y(y) {}

    double x() const { return m_x; }
    double y() const { return m_y; }

    double distance(const Point& otherPoint) const;
    double squareDistance(const Point& otherPoint) const;
    double squareNorm() const;
    double norm() const;

    double m_x;
    double m_y;
};

bool operator<(const Point& lhs, const Point& rhs);

std::ostream& operator<<(std::ostream& ost, const Point& point);

} // namespace primitives

#endif