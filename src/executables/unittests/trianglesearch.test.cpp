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

TEST_CASE("TriangleSearchHierarchy::getContainingLeafTriangle")
{
    primitives::Triangle root(primitives::Point(-1, 0), primitives::Point(1, 0), primitives::Point(0, 1));
    primitives::Triangle intermediate1(primitives::Point(-1, 0), primitives::Point(0, 0), primitives::Point(0, 1));
    primitives::Triangle intermediate2(primitives::Point(0, 0), primitives::Point(1, 0), primitives::Point(0, 1));
    primitives::Triangle leaf1(primitives::Point(-.5, .5), primitives::Point(.5, .5), primitives::Point(0, 1));
    primitives::Triangle leaf2(primitives::Point(-.5, .5), primitives::Point(.5, .5), primitives::Point(0, .25));

    TriangleSearchHierarchyTest triSearch(root);
    triSearch.add(intermediate1, {root});
    triSearch.add(intermediate2, {root});
    triSearch.add(leaf1, {intermediate1, intermediate2});
    triSearch.add(leaf2, {intermediate1, intermediate2});

    auto containingTri1 = triSearch.getContainingLeafTriangle(primitives::Point(0, .75));
    auto containingTri2 = triSearch.getContainingLeafTriangle(primitives::Point(0, .3));

    CHECK(containingTri1 == leaf1);
    CHECK(containingTri2 == leaf2);
    CHECK_THROWS(triSearch.getContainingLeafTriangle(primitives::Point(0, .1)));
}
