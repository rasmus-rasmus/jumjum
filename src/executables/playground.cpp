#include <iostream>
#include "algorithms/planesweep/planesweep.hpp"

#include <set>
#include <algorithm>

using namespace algorithms;

int main()
{
    std::vector<primitives::LineSegment> lines = {
                                                  primitives::LineSegment(primitives::Point(0., .1), primitives::Point(1., -.1)), 
                                                  primitives::LineSegment(primitives::Point(0, .5), primitives::Point(0, -.5)),
                                                  primitives::LineSegment(primitives::Point(.5, .5), primitives::Point(.5, -.5)),
                                                  primitives::LineSegment(primitives::Point(1., .5), primitives::Point(1., -.5))
                                                  };

    Planesweep algo(lines);

    auto intersections = algo.perform();

    std::cout << intersections.size() << std::endl;

    for (auto inter : intersections)
    {
        std::cout << "Intersection between lines "
                  << "["
                  << inter.first.getStartPoint() << " -> " << inter.first.getEndPoint()
                  << "]"
                  << " and "
                  << "["
                  << inter.second.getStartPoint() << " -> " << inter.second.getEndPoint()
                  << "]"
                  << " is: "
                  << std::get<primitives::Point>(inter.first.computeIntersection(inter.second))
                  << std::endl;
    }

    return 0;
}
