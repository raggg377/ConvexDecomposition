// this code is a part of the "dcel" library
#include "../include/dcel_vertex.hpp"

Vertex *Vertex::getPreviousVertex()
{
    return this->inc_edge->next->org;
}
Vertex *Vertex::getNextVertex()
{
    return this->inc_edge->twin->next->next->org;
}
