#include "planesweep.hpp"

#include <set>

#include "algorithms/planesweep/events.hpp"

namespace algorithms
{

Planesweep::Planesweep(const std::vector<primitives::LineSegment>& lines)
{
    for (auto line : lines)
    {
        m_lines.push_back(line);   
    }
}   

// std::multimap<primitives::LineLineIntersection, LinePair> Planesweep::perform()
// {
//     std::set<Event> eventQueue;
//     std::set<Event> statusLine;
//     std::map<primitives::LineLineIntersection, LinePair> intersectionsOut;

//     for (size_t i = 0; i < m_lines.size(); ++i)
//     {
//         auto linePtr = &m_lines[i];

//         // Insert EndPointEvent once for each end point with isUpper set resp not set. 
//         // The EndPointEvent will take care of returning the relevant end point for comparison
//         // based on whether it is upper or not.
//         eventQueue.insert(EndPointEvent(linePtr, true));
//         eventQueue.insert(EndPointEvent(linePtr, false));
//     }

//     while (!eventQueue.empty())
//     {
//         auto nextEvent = eventQueue.begin();

//         if (typeid(nextEvent) == typeid(IntersectionEvent))
//         {
    
//             intersectionsOut[nextEvent->getIntersection()] = {nextEvent->getFirstLine(), nextEvent->getSecondLine()};
//         }
//     }
// }

} // namespace algorithms