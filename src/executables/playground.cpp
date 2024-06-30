#include <iostream>
#include "primitives/linesegment.hpp"

#include "primitives/linesegment.cpp"

int main()
{
    Point startPoint(0, 0);
    Point endPoint(0, 1);
    LineSegment line(startPoint, endPoint);

    for (int i = -5; i < 6; ++i)
    {
        Point refPoint(5, (double)i/10);

        std::cout << getOrientation(refPoint, line) << std::endl;
    }

    return 0;
}
