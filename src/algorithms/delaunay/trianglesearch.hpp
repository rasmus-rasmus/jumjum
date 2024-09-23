#ifndef ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED
#define ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED

#include "primitives/triangle.hpp"

#include <vector>
#include <set>

namespace algorithms
{

struct TriangleSearchHierarchy
{
    struct SearchTriangle: primitives::Triangle
    {
        std::vector<const SearchTriangle*> children;
    };

    // "Root" is a bit misleading, since this search structure isn't actually a tree.
    // "Root" here just means "triangle containing all other triangles."
    TriangleSearchHierarchy(const primitives::Triangle& rootTriangle);

    SearchTriangle getRoot() const { return *root; }

    void add(primitives::Triangle triangleToAdd, const std::vector<primitives::Triangle>& parents);

    // "Leaf" here just means childless.
    primitives::Triangle getContainingLeafTriangle(primitives::Point point) const;

protected:
    std::set<SearchTriangle> triangles;
    std::set<SearchTriangle>::iterator root;
};

}

#endif // ALGORITHMS_DELAUNAY_TRIANGLESEARCH_HPP_INCLUDED
