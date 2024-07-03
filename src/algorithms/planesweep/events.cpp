#include "algorithms/planesweep/events.hpp"


IntersectionEvent::IntersectionEvent(LineSegment line, LineSegment otherLine)
{
    if (!line.intersects(otherLine))
    {
        throw std::logic_error("Trying to create intersection event with non-intersecting lines.");
    }

    firstLine = line;
    secondLine = otherLine;
}

Point IntersectionEvent::getIntersectionPoint()
{
    if (m_intersectionPoint)
    {
        return *m_intersectionPoint;
    }

    // TODO: Handle vertical lines 

    double slopeFirstLine = (firstLine.getEndPoint().y() - firstLine.getStartPoint().y()) / (firstLine.getEndPoint().x() - firstLine.getStartPoint().x());
    double interceptFirstLine = firstLine.getStartPoint().y() - slopeFirstLine * firstLine.getStartPoint().x();

    double slopeSecondLine = (secondLine.getEndPoint().y() - secondLine.getStartPoint().y()) / (secondLine.getEndPoint().x() - secondLine.getStartPoint().x());
    double interceptSecondLine = secondLine.getStartPoint().y() - slopeSecondLine * secondLine.getStartPoint().x();

    // TODO: Handle parallel, i. e., colinear, lines

    double intersectionXVal = (interceptSecondLine - interceptFirstLine) / (slopeFirstLine - slopeSecondLine);
    double intersectionYVal = slopeFirstLine * intersectionXVal + interceptFirstLine;

    Point interSectionPoint(intersectionXVal, intersectionYVal);
    m_intersectionPoint = &interSectionPoint;

    return interSectionPoint;
}