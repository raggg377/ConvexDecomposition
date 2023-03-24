#ifndef LPV_HPP
#define LPV_HPP

#include "dcel_vertex.hpp"

using namespace std;

/**
 * @brief LPvj is a object of the list containing pairs (k,vr) where k is the index of a polygon containing vj as one of its vertices and vr is the next vertex to vj in that polygon k, but these pairs are in LPvj only if vr is not the consecutive vertex of vj in the initial polygon, i.e., if (vj ,vr) is a diagonal.
 *
 */
class lpv
{
public:
    int k;       ///< index of decomposed polygon containing Vertex j
    Vertex *ver; ///< Vertex adjacent to vj in the decomposed polygon but not in the original polygon

    // lpv(int k, Vertex *ver)
    // {
    //     k = (this->k);
    //     ver = (this->ver);
    // }

    lpv() {}
};

#endif
