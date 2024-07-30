#include "triangle.hpp"

namespace primitives
{

Circle Triangle::getCircumcircle() const
{
    return Circle(m_p1, m_p2, m_p3);
}

} // namespace primitives
