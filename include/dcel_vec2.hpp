#ifndef DCEL_VEC2_HPP
#define DCEL_VEC2_HPP
using namespace std;

class vec2
{
public:
    float x; ///< X co-ordinate of the vertex
    float y; ///< Y co-ordinate of the vertex

    vec2()
    {
        x = 0;
        y = 0;
    }
    /**
     * @brief Construct a new vec2 object using given co-ordintes
     *
     * @param x X co-ordinate of the vertex
     * @param y Y co-ordinate of the vertex
     */
    vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    };
};

#endif