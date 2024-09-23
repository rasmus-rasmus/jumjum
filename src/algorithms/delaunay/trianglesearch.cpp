#include "trianglesearch.hpp"

#include <stack>

namespace algorithms
{

TriangleSearchHierarchy::TriangleSearchHierarchy(const primitives::Triangle& rootTriangle)
{
    triangles = {(SearchTriangle)rootTriangle};
    root = triangles.begin();
}

void TriangleSearchHierarchy::add(primitives::Triangle triangleToAdd, const std::vector<primitives::Triangle>& parents)
{
    auto insertedTriangle = triangles.insert((SearchTriangle)triangleToAdd).first;

    for (const auto& parentTri : parents)
    {
        auto parentIt = triangles.find((SearchTriangle)parentTri);

        if (parentIt == triangles.end())
        {
            throw std::logic_error("Couldn't find my parents. Wah wah wah.");
        }
        
        auto parent = *parentIt;
        auto parentErased = triangles.erase(parentIt);
        
        parent.children.push_back(&*insertedTriangle);
        triangles.insert(parentErased, parent);
    }
}

primitives::Triangle TriangleSearchHierarchy::getContainingLeafTriangle(primitives::Point point) const
{
    if (!root->contains(point))
    {
        throw std::logic_error("Root triangle doesn't contain point, so no triangle does.");
    }

    std::stack<SearchTriangle> unprocessed;
    unprocessed.push(*root);

    while (!unprocessed.empty())
    {
        auto currTriangle = unprocessed.top();
        unprocessed.pop();

        if (currTriangle.children.empty()) return currTriangle;

        for (auto child : currTriangle.children)
        {
            if (child->contains(point))
            {
                unprocessed.push(*child);
            }
        }
    }

    throw std::logic_error("Couldn't find containing leaf triangle.");
}

} // namespace algorithms
