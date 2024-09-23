#include "executables/doctest.h"

#include "algorithms/delaunay/trianglesearch.hpp"

using namespace algorithms;

// Wrapper class to expose protected members.
struct TriangleSearchHierarchyTest : TriangleSearchHierarchy
{
    std::set<SearchTriangle> getTriangles() { return triangles; }
};

TEST_CASE("TriangleSearchHierarchy::add")
{
    primitives::Triangle root(primitives::Point(0, 0), primitives::Point(0, 1), primitives::Point(1, 0));
    primitives::Triangle child(primitives::Point(.1, .1), primitives::Point(.1, .9), primitives::Point(.9, .01));
    primitives::Triangle otherChild(primitives::Point(0, 0), primitives::Point(0, -1), primitives::Point(-1, 0));

    TriangleSearchHierarchyTest triSearchHierarchy(root);

    auto triangles = triSearchHierarchy.getTriangles();

    CHECK(triangles.size() == 1);
    CHECK(*triangles.begin() == root);
    CHECK(triSearchHierarchy.getRoot() == root);

    triSearchHierarchy.add(child, {root});

    triangles = triSearchHierarchy.getTriangles();
    auto currRoot = triSearchHierarchy.getRoot();

    CHECK(triangles.size() == 2);
    CHECK(currRoot == root);
    CHECK(currRoot.children.size() == 1);
    CHECK(*currRoot.children[0] == child);

    triSearchHierarchy.add(otherChild, {root, child});

    triangles = triSearchHierarchy.getTriangles();
    currRoot = triSearchHierarchy.getRoot();
    auto currChild = triangles.find(TriangleSearchHierarchyTest::SearchTriangle(child));

    CHECK(triangles.size() == 3);
    CHECK(currRoot == root);
    CHECK(currRoot.children.size() == 2);
    CHECK(currChild->children.size() == 1);
    CHECK(*currChild->children[0] == otherChild);
}