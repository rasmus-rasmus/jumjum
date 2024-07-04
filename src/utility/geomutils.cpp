#include "utility/geomutils.hpp"

glm::dmat2 getRotationMatrix(double angle)
{
    return glm::dmat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

glm::dvec2 toGLM(const Point& point)
{
    return glm::dvec2(point.x(), point.y());
}

Point toPoint(glm::dvec2 vec)
{
    return Point(vec.x, vec.y);
}