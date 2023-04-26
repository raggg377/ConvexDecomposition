#ifndef DIAG_HPP
#define DIAG_HPP

#include "dcel_vec2.hpp"

using namespace std;

/**
 * @brief class to store the vertices of diagonals
 *
 */
class diag
{
public:
    Vertex *v1; ///< First Vertex of diagonal
    Vertex *v2; ///< Second Vertex of diagonal
};

#endif
