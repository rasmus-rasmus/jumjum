#ifndef POINT_HPP_INCLUDED
#define POINT_HPP_INCLUDED

struct Point
{
    Point() : m_x(0.), m_y(0.) {}
    Point(double x, double y) : m_x(x), m_y(y) {}

    double x() const { return m_x; }
    double y() const { return m_y; }

    double distance(const Point& otherPoint) const;
    double squareDistance(const Point& otherPoint) const;

    double m_x;
    double m_y;
};

bool operator<(const Point& lhs, const Point& rhs);
// {
//     if (lhs.y() < rhs.y())
//     {
//         return true;
//     }

//     return lhs.x() < rhs.x();
// }

#endif