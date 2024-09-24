#include "trianglesearch.hpp"

#include <stack>

namespace algorithms
{

TriangleSearchHierarchy::TriangleSearchHierarchy(const primitives::Triangle& rootTriangle) : root(rootTriangle)
{
    triangles[rootTriangle] = {};
}

void TriangleSearchHierarchy::add(primitives::Triangle triangleToAdd, const std::vector<primitives::Triangle>& parents)
{
    if (triangles.find(triangleToAdd) != triangles.end())
    {
        throw std::logic_error("Triangle already in search hierarchy.");
    }
    triangles[triangleToAdd] = {};

    for (const auto& parentTri : parents)
    {
        auto parentIt = triangles.find(parentTri);

        if (parentIt == triangles.end())
        {
            throw std::logic_error("Couldn't find my parents. Wah wah wah.");
        }
        
        parentIt->second.push_back(triangleToAdd);
    }
}

primitives::Triangle TriangleSearchHierarchy::getContainingLeafTriangle(primitives::Point point) const
{
    if (!root.contains(point))
    {
        throw std::logic_error("Root triangle doesn't contain point, so no triangle does.");
    }

    std::stack<primitives::Triangle> unprocessed;
    unprocessed.push(root);

    while (!unprocessed.empty())
    {
        auto currTriangle = unprocessed.top();
        unprocessed.pop();

        if (triangles.find(currTriangle)->second.empty()) return currTriangle;

        for (auto child : triangles.find(currTriangle)->second)
        {
            if (child.contains(point))
            {
                unprocessed.push(child);
            }
        }
    }

    throw std::logic_error("Couldn't find containing leaf triangle.");
}

} // namespace algorithms
