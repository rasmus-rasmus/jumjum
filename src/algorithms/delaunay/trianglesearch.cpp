#include "trianglesearch.hpp"

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

} // namespace algorithms
