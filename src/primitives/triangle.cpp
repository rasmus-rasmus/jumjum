#include "utility/geomutils.hpp"
#include "triangle.hpp"

#include <glm/glm.hpp>

namespace primitives
{

Circle Triangle::getCircumcircle() const
{
    return Circle(m_p1, m_p2, m_p3);
}

bool Triangle::contains(const Point& point) const
{
    auto a = utility::toGLM(m_p1) - utility::toGLM(point);
    auto b = utility::toGLM(m_p2) - utility::toGLM(point);
    auto c = utility::toGLM(m_p3) - utility::toGLM(point);

    auto ab = glm::cross(glm::dvec3(a, 0), glm::dvec3(b, 0));
    auto bc = glm::cross(glm::dvec3(b, 0), glm::dvec3(c, 0));
    auto ca = glm::cross(glm::dvec3(c, 0), glm::dvec3(a, 0));

    if (ab.z == 0 || bc.z == 0 || ca.z == 0)
    {
        return true;
    }

    return ab.z * bc.z > 0 && bc.z * ca.z > 0;
}

} // namespace primitives
