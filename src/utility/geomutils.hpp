#ifndef GEOM_UTILS_HPP_INCLUDED
#define GEOM_UTILS_HPP_INCLUDED

#include <glm/mat2x2.hpp>

#include "primitives/point.hpp"

namespace utility
{

glm::dmat2 getRotationMatrix(double angle);

glm::dvec2 toGLM(const primitives::Point& point);

primitives::Point toPoint(glm::dvec2 vec);

double getAngle(const glm::dvec2& vec1, const glm::dvec2& vec2);

} // namespace utility

#endif