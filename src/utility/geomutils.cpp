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
    double len1 = glm::length(vec1);
    double len2 = glm::length(vec2);
    if (len1 < 1e-8 || len2 < 1e-8)
    {
        throw std::invalid_argument("Zero-length vector passed to getAngle.");
    }

    double dot = glm::dot(vec1 / len1, vec2 / len2);

    return std::acos(std::clamp(dot, -1.0, 1.0));
}

} // namespace utility
