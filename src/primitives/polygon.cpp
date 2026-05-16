#include "polygon.hpp"

#include "linesegment.hpp"
#include "algorithms/planesweep/planesweep.hpp"

#include <iostream>
#include <algorithm>
#include <format>
#include <variant>
#include <set>


namespace primitives
{
    
Polygon::Polygon(const std::vector<Point>& vertices)
{
    std::vector<LineSegment> edges;
    m_vertices = std::move(vertices);
    
    // Verifying that no two edges intersect (in interior point).
    for (size_t i = 0; i < m_vertices.size(); ++i)
    {
        edges.push_back(getEdge(i));
    }

    algorithms::Planesweep planesweep(edges);
    auto intersections = planesweep.perform();

    for (const auto& inter : intersections)
    {
        auto computedInter = inter.first.computeIntersection(inter.second);
        if (std::holds_alternative<std::monostate>(computedInter))
        {
            // Unexpected -- gobling up error. Yum yum yum
            continue;
        }
        if (std::holds_alternative<LineSegment>(computedInter))
        {
            throw std::logic_error("Two edges intersect in line");
        }
        auto intersectionPoint = std::get<Point>(computedInter);
        if (intersectionPoint.squareDistance(inter.first.getStartPoint()) > 1e-12*1e-12 && intersectionPoint.squareDistance(inter.first.getEndPoint()) > 1e-12*1e-12)
        {
            throw std::logic_error("Two edges intersected in interior point");
        }
    }
}

bool Polygon::contains(const Point& point)
{
    int windingNumber = 0;
    for (size_t i = 0; i < m_vertices.size(); ++i)
    {
        auto edge = getEdge(i);
        if (projectPointOnLine(point, edge).squareDistance(point) <= 1e-10)
        {
            // Point lies on boundary; we consider it contained
            return true;
        }

        double verMaxX = std::max(edge.getStartPoint().x(), edge.getEndPoint().x());
        if (verMaxX < point.x()) continue;
        auto raySegment = LineSegment(point, Point(verMaxX + std::abs(verMaxX) + 16, point.y()));

        if (raySegment.intersects(edge))
        {
            windingNumber += edge.getStartPoint().y() < edge.getEndPoint().y() ? 1 : -1;
        }
    }

    return windingNumber != 0;
}

} // namespace primitives
