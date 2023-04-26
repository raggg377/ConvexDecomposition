#ifndef DCEL_EDGE_HPP
#define DCEL_EDGE_HPP

#include "dcel_vertex.hpp"

using namespace std;

/**
 * @brief Implements the Edge type with its origin vertex ,twin edge ,next edge and previous edge
 *
 */
class Edge
{
public:
    Edge *twin;  ///< Pointer to oppositely directed edge
    Vertex *org; ///< Pointer to origin vertex of directed
    Edge *next;  ///< Pointer to next edge in clockwise direction
    Edge *prev;  ///< Pointer to previous edge in clockwise direction
    /**
     * @brief Construct a new Edge object initialise all the pointers to nullptr
     *
     */
    Edge()
    {
        twin = nullptr;
        org = nullptr;
        next = nullptr;
        prev = nullptr;
    }
};

#endif