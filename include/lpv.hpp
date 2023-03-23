#ifndef LPV_HPP
#define LPV_HPP

#include "dcel_vec2.hpp"

using namespace std;

class lpv
{
public:
    int k;
    Vertex *ver;
    lpv(int k, Vertex *ver) : k(this->k), ver(this->ver) {}
};

#endif
