#ifndef CIRCLE_HPP_INCLUDED
#define CIRCLE_HPP_INCLUDED

#include "point.hpp"

namespace primitives
{

struct Circle
{
    Circle(Point center, double radius) : m_center(center), m_radius(radius) {}
    Circle(Point p1, Point p2, Point p3);

    Point getCenter() const { return m_center; }
    double getRadius() const { return m_radius; }

    bool isInside(const Point& point) const;

private:
    Point m_center;
    double m_radius;
};

} // namespace primitives

#endif // CIRCLE_HPP_INCLUDED