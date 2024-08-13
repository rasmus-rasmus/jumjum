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

double getAngle(const glm::dvec2& vec1, const glm::dvec2& vec2)
{
    return atan2(vec2.y, vec2.x) - atan2(vec1.y, vec1.x);
}

} // namespace utility
