#ifndef DCEL_VEC2_HPP
#define DCEL_VEC2_HPP
using namespace std;

class vec2
{
public:
    double x; ///< X co-ordinate of the vertex
    double y; ///< Y co-ordinate of the vertex

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
    vec2(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    bool operator==(const vec2 &other) const
    {
        return (x == other.x) && (y == other.y);
    }
};

#endif