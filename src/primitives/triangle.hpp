#ifndef TRIANGLE_HPP_INCLUDED
#define TRIANGLE_HPP_INCLUDED

#include "point.hpp"
#include "circle.hpp"

namespace primitives
{

struct Triangle
{
    Triangle(Point p1, Point p2, Point p3) : m_p1(p1), m_p2(p2), m_p3(p3) {}

    Point getPoint1() const { return m_p1; }
    Point getPoint2() const { return m_p2; }
    Point getPoint3() const { return m_p3; }

    bool contains(const Point& point) const;

    Circle getCircumcircle() const;

private:
    Point m_p1;
    Point m_p2;
    Point m_p3;
};

} // namespace primitives

#endif // TRIANGLE_HPP_INCLUDED
