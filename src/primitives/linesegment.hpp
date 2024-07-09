#ifndef LINE_SEGMENT_HPP_INCLUDED
#define LINE_SEGMENT_HPP_INCLUDED

#include "point.hpp"

#include <stdexcept>
#include <variant>
#include <glm/vec2.hpp>


namespace primitives
{

struct LineSegment;
using LineLineIntersection = std::variant<Point, LineSegment>;

struct LineSegment
{
    LineSegment()
    {
        m_startPoint = Point();
        m_endPoint = Point(1., 0.);
    }

    LineSegment(const Point& startPoint, const Point& endPoint);

    Point getStartPoint() const { return m_startPoint; }
    Point getEndPoint() const { return m_endPoint; }
    glm::dvec2 getDirection() const;

    double length();

    bool intersects(const LineSegment& otherLine);

    LineLineIntersection computeIntersection(const LineSegment& otherLine);

private:
    Point m_startPoint;
    Point m_endPoint;
};

} // namespace primitives

#endif