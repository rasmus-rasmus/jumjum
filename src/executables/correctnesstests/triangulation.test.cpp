#include "executables/doctest.h"

#include "algorithms/delaunay/triangulation.hpp"
#include "utility/io.hpp"

#include <fstream>
#include <filesystem>

using namespace algorithms;

TEST_CASE("Delaunay Triangulator correctness")
{
    DelaunayTriangulator triangulator1;
    utility::loadTriangulationFromFile(utility::getProjectRootPath() / "src/executables/testdata/inputTriangulation1.txt", triangulator1);

    DelaunayTriangulator triangulator2;
    utility::loadTriangulationFromFile(utility::getProjectRootPath() / "src/executables/testdata/inputTriangulation2.txt", triangulator2);

    DelaunayTriangulator triangulator3;
    utility::loadTriangulationFromFile(utility::getProjectRootPath() / "src/executables/testdata/inputTriangulation3.txt", triangulator3);

    CHECK(triangulator1.performTriangulation());
    CHECK(triangulator2.performTriangulation());
    CHECK(triangulator3.performTriangulation());

    // Maybe a bit ><>'y to use the class' own method to test for its correctness...
    CHECK(triangulator1.isDelaunay());
    CHECK(triangulator2.isDelaunay());
    CHECK(triangulator3.isDelaunay());
}
