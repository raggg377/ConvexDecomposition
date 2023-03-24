#ifndef DCEL_HPP
#define DCEL_HPP
using namespace std;
#include <vector>
#include "dcel_vertex.hpp"
#include "dcel_edge.hpp"

/**
 * @brief Class for creating objects of dcel type
 *
 */
class dcel
{
public:
    vector<Edge *> edgeList;     ///< List of half-edges of dcel
    vector<Vertex *> vertexList; ///< List of vertices of dcel
    /**
     * @brief creates a dcel data structure for a vector list of vec2 given in clockwise direction
     *
     * @param points vector list of pair of coordinates stored in vec2 data type
     */
    void make_dcel(vector<vec2> &points);
    /**
     * @brief removes a vertex from a dcel
     *
     * @param v the vertex which needs to be removed
     */
    void removeVertex(Vertex *v);
    /**
     * @brief prints the vertcies of dcel and their incident edges
     *
     */
    void print_dcel();
    /**
     * @brief prints the edges of the dcel
     *
     */
    void print_dcel_edges();
};
#endif
