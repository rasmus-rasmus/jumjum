#include "circle.hpp"
#include "utility/geomutils.hpp"

#include <glm/mat3x3.hpp>

namespace primitives
{

Circle::Circle(Point p1, Point p2, Point p3)
{
    double Sx = glm::determinant(glm::mat3(p1.squareNorm(), p1.y(), 1,
                                           p2.squareNorm(), p2.y(), 1,
                                           p3.squareNorm(), p3.y(), 1));
    double Sy = glm::determinant(glm::mat3(p1.x(), p1.squareNorm(), 1,
                                           p2.x(), p2.squareNorm(), 1,
                                           p3.x(), p3.squareNorm(), 1));
    Sx *= .5;
    Sy *= .5;
    double a = glm::determinant(glm::mat3(p1.x(), p1.y(), 1,
                                          p2.x(), p2.y(), 1,
                                          p3.x(), p3.y(), 1));
    double b = glm::determinant(glm::mat3(p1.x(), p1.y(), p1.squareNorm(),
                                          p2.x(), p2.y(), p2.squareNorm(),
                                          p3.x(), p3.y(), p3.squareNorm()));
    if (std::abs(a) < 1e-5)
    {
        throw std::runtime_error("Cannot construct circle from collinear points.");
    }
    m_center = Point(Sx / a, Sy / a);
    m_radius = std::sqrt(b / a + m_center.squareNorm());
}

bool Circle::isInside(const Point& point) const
{
    return m_center.squareDistance(point) < m_radius * m_radius;
}

} // namespace primitives