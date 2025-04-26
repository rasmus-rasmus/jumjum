#ifndef ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED
#define ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED

#include "primitives/triangle.hpp"

#include <vector>
#include <map>

namespace algorithms
{

using ChildMap = std::map<primitives::Triangle, std::vector<primitives::Triangle>>;

/// @brief A search hierarchy for triangles. This is a directed graph whose vertices are triangles and whose
///        edges signify "parent-child" relationship as explained below:
///        A triangle is considered to be a "child" of another triangle if it was created as a result of 
///        either a point insertion in the parent triangle (upon which the inserted point was connected to the 
///        parent's corners, and the child was one of the three new triangles thus drawn) or an edge flip 
///        (in which case the two resulting new triangles are considered children of the two triangles whose shared edge was flipped).
///
///        Though this hierarchy is not actually a tree, it is still meaningfull to refer to the "root" triangle
///        as the triangle that contains all other triangles, and to the "leaf" triangles as the triangles that
///        have no children. The leaf triangles correspond to triangles actually in the (current state of) the triangulation.
///
///        To find a leaf triangle (i.e., a triangle in the triangulation) that contains a point, one can start
///        at the root triangle and recursively check the children of the current triangle, until a leaf triangle
///        is found. This is used to insert new vertices into the triangulation.
struct TriangleSearchHierarchy
{
    TriangleSearchHierarchy(const primitives::Triangle& rootTriangle);

    primitives::Triangle getRoot() const { return root; }

    void add(primitives::Triangle triangleToAdd, const std::vector<primitives::Triangle>& parents);

    primitives::Triangle getContainingLeafTriangle(primitives::Point point) const;

protected:
    /// @brief A map from a triangle to its children, i.e., an edge map.
    ChildMap triangles;
    /// @brief The root triangle of the search hierarchy. Any triangle added, should be fully contained within this triangle.
    ///        Note: This is not actually enforced, but the search hierarchy will not work correctly if this is not the case.
    primitives::Triangle root;
};

}

#endif // ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED
