#ifndef DCEL_EDGE_HPP
#define DCEL_EDGE_HPP

#include "dcel_vertex.hpp"
#include "dcel_face.hpp"
using namespace std;
class Edge
{
public:
    Edge *twin;  ///< Pointer to oppositely directed edge
    Vertex *org; ///< Pointer to origin vertex of directed
    Edge *next;  ///< Pointer to next edge in clockwise direction
    Edge *prev;  ///< Pointer to previous edge in clockwise direction
    Edge()
    {
        twin = nullptr;
        org = nullptr;
        next = nullptr;
        prev = nullptr;
    }
};

#endif