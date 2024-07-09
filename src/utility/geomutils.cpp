#include "utility/geomutils.hpp"

namespace utility
{


glm::dmat2 getRotationMatrix(double angle)
{
    return glm::dmat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

glm::dvec2 toGLM(const primitives::Point& point)
{
    return glm::dvec2(point.x(), point.y());
}

primitives::Point toPoint(glm::dvec2 vec)
{
    return primitives::Point(vec.x, vec.y);
}

} // namespace utility
