#include "algorithms/planesweep/events.hpp"


IntersectionEvent::IntersectionEvent(LineSegment line, LineSegment otherLine)
{
    if (!line.intersects(otherLine))
    {
        throw std::logic_error("Trying to create intersection event with non-intersecting lines.");
    }

    m_firstLine = line;
    m_secondLine = otherLine;
}

Point IntersectionEvent::getIntersectionPoint()
{
    if (m_intersectionPoint.has_value())
    {
        return *m_intersectionPoint;
    }
    if (m_firstLine.isVertical() || m_secondLine.isVertical())
    {
        // TODO: Handle vertical lines 
        return Point(666.,666.);
    }

    double slopeFirstLine = (m_firstLine.getEndPoint().y() - m_firstLine.getStartPoint().y()) / (m_firstLine.getEndPoint().x() - m_firstLine.getStartPoint().x());
    double interceptFirstLine = m_firstLine.getStartPoint().y() - slopeFirstLine * m_firstLine.getStartPoint().x();

    double slopeSecondLine = (m_secondLine.getEndPoint().y() - m_secondLine.getStartPoint().y()) / (m_secondLine.getEndPoint().x() - m_secondLine.getStartPoint().x());
    double interceptSecondLine = m_secondLine.getStartPoint().y() - slopeSecondLine * m_secondLine.getStartPoint().x();

    if (std::abs(slopeFirstLine - slopeSecondLine) < 1e-6)
    {
        // TODO: Handle parallel, i. e., colinear, lines
        return Point(666.,666.);
    }


    double intersectionXVal = (interceptSecondLine - interceptFirstLine) / (slopeFirstLine - slopeSecondLine);
    double intersectionYVal = slopeFirstLine * intersectionXVal + interceptFirstLine;

    Point interSectionPoint(intersectionXVal, intersectionYVal);
    m_intersectionPoint = interSectionPoint;

    return interSectionPoint;
}