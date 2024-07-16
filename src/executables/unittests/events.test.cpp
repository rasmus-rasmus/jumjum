#include "executables/doctest.h"

#include "algorithms/planesweep/events.hpp"
#include "primitives/linesegment.hpp"
#include "primitives/point.hpp"

using namespace algorithms;

TEST_CASE("IntersectionEvent constructor")
{
    primitives::LineSegment firstLine(primitives::Point(-1., 0.), primitives::Point(1., 0.));
    primitives::LineSegment secondLine(primitives::Point(-1., 1.), primitives::Point(1., 1.));

    CHECK_THROWS(IntersectionEvent(firstLine, secondLine));
}

TEST_CASE("getType polymorphism")
{
    primitives::LineSegment firstLine(primitives::Point(-1., 0.), primitives::Point(1., 0.));
    primitives::LineSegment secondLine(primitives::Point(0., 1.), primitives::Point(0., -1.));
    IntersectionEvent intersection(firstLine, secondLine);
    Event* eventPtrToIntersection = &intersection;

    CHECK(eventPtrToIntersection->getType() == EventType::Intersection);

    EndPointEvent endPoint(firstLine, true);

    Event* eventPtrToEndPoint = &endPoint;

    CHECK(eventPtrToEndPoint->getType() == EventType::EndPoint);
}
