#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

#include "point.hpp"
#include "linesegment.hpp"

namespace primitives
{

struct Polygon
{
    Polygon(const std::vector<Point>& vertices);
    
    size_t size() { return m_vertices.size(); }
    Point getVertex(size_t idx) {return m_vertices.at(idx); }
    // Returns edge starting at vertex <idx>
    LineSegment getEdge(size_t idx) { return LineSegment(getVertex(idx), getVertex((idx + 1) % m_vertices.size())); }

    bool contains(const Point& point);

private:
    std::vector<Point> m_vertices;
};

}


#endif // POLYGON_HPP
