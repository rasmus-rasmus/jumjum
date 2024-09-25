#include "utility/geomutils.hpp"
#include "triangle.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <set>
#include <algorithm>

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
        // If one of the orientations is 0, it means that the point lies on the line spanned 
        // by one (or more in which case it coincides with a vertex) of the edges. It then
        // actually lies on that edge, and thus is contained in the triangle, if and only if
        // the two remaining orientations have the same sign.
        std::set<double> orientations{ab.z, bc.z, ca.z};

        return orientations.size() <= 2 || *(++orientations.begin()) != 0;
    }

    return ab.z * bc.z > 0 && bc.z * ca.z > 0;
}

bool Triangle::operator==(const Triangle& otherTri)
{
    auto thisPoints = std::vector<Point>{getPoint1(), getPoint2(), getPoint3()};
    auto otherPoints = std::vector<Point>{otherTri.getPoint1(), otherTri.getPoint2(), otherTri.getPoint3()};

    std::sort(thisPoints.begin(), thisPoints.end());
    std::sort(otherPoints.begin(), otherPoints.end());

    for (int i = 0; i < 3; ++i)
    {
        if (thisPoints[i].squareDistance(otherPoints[i]) >= 1e-10) return false;
    }

    return true;
}

bool operator<(const Triangle& lhs, const Triangle& rhs)
{
    auto lhsPoints = std::vector<Point>{lhs.getPoint1(), lhs.getPoint2(), lhs.getPoint3()};
    auto rhsPoints = std::vector<Point>{rhs.getPoint1(), rhs.getPoint2(), rhs.getPoint3()};

    std::sort(lhsPoints.begin(), lhsPoints.end());
    std::sort(rhsPoints.begin(), rhsPoints.end());

    return lhsPoints < rhsPoints;
}

} // namespace primitives
