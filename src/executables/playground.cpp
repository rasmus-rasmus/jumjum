#include <iostream>
#include "algorithms/planesweep/planesweep.hpp"

#include <set>
#include <algorithm>
#include <filesystem>

using namespace algorithms;

int main()
{
    // std::vector<primitives::LineSegment> lines = {
    //                                               primitives::LineSegment(primitives::Point(0., .1), primitives::Point(1., -.1)), 
    //                                               primitives::LineSegment(primitives::Point(0, .5), primitives::Point(0, -.5)),
    //                                               primitives::LineSegment(primitives::Point(.5, .5), primitives::Point(.5, -.5)),
    //                                               primitives::LineSegment(primitives::Point(1., .5), primitives::Point(1., -.5))
    //                                               };

    // Planesweep algo(lines);

    // auto intersections = algo.perform();

    // std::cout << intersections.size() << std::endl;

    // for (auto inter : intersections)
    // {
    //     std::cout << "Intersection between lines "
    //               << "["
    //               << inter.first.getStartPoint() << " -> " << inter.first.getEndPoint()
    //               << "]"
    //               << " and "
    //               << "["
    //               << inter.second.getStartPoint() << " -> " << inter.second.getEndPoint()
    //               << "]"
    //               << " is: "
    //               << std::get<primitives::Point>(inter.first.computeIntersection(inter.second))
    //               << std::endl;
    // }

    primitives::LineSegment firstLine(primitives::Point(580., 716.), primitives::Point(271., 427.));
    primitives::LineSegment secondLine(primitives::Point(439., 750.), primitives::Point(850., 558.));
    primitives::LineSegment problematicLine(primitives::Point(703., 692.), primitives::Point(2., 675.));

    std::vector<primitives::LineSegment> lines = {firstLine, secondLine, problematicLine};

    Planesweep algo(lines);

    auto intersections = algo.perform();

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

    // std::cout << std::get<primitives::Point>(firstLine.computeIntersection(secondLine)) << std::endl;
    // std::cout << std::get<primitives::Point>(firstLine.computeIntersection(problematicLine)) << std::endl;
    // std::cout << std::get<primitives::Point>(secondLine.computeIntersection(problematicLine)) << std::endl;
    // std::cout << (std::get<primitives::Point>(firstLine.computeIntersection(secondLine)) < std::get<primitives::Point>(firstLine.computeIntersection(problematicLine)))
    //               << std::endl;

    return 0;
}
