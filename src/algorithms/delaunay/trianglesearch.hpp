#ifndef ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED
#define ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED

#include "primitives/triangle.hpp"

#include <vector>
#include <map>

namespace algorithms
{

using ChildMap = std::map<primitives::Triangle, std::vector<primitives::Triangle>>;

struct TriangleSearchHierarchy
{
    // "Root" is a bit misleading, since this search structure isn't actually a tree.
    // "Root" here just means "triangle containing all other triangles."
    TriangleSearchHierarchy(const primitives::Triangle& rootTriangle);

    primitives::Triangle getRoot() const { return root; }

    void add(primitives::Triangle triangleToAdd, const std::vector<primitives::Triangle>& parents);

    // "Leaf" here just means childless.
    primitives::Triangle getContainingLeafTriangle(primitives::Point point) const;

protected:
    ChildMap triangles;
    primitives::Triangle root;
};

}

#endif // ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED
