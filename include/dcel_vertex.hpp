#ifndef DCEL_VERTEX_HPP
#define DCEL_VERTEX_HPP

#include "dcel_vec2.hpp"
class Edge;

class Vertex
{
public:
    int id;   ///< Identifier for the vertex
    vec2 pos; ///< Geometric co-ordinates of the vetex
    /**
     *  Pointer to any directed edge that has this vertex as origin. inc_edge.org = this vertex
     * */
    Edge *inc_edge;
};

#endif