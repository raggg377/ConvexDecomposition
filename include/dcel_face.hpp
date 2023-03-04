#ifndef DCEL_FACE_HPP
#define DCEL_FACE_HPP

using namespace std;
class DoubleEdgeListHalfEdge;

class DoubleEdgeListFace
{
public:
    DoubleEdgeListHalfEdge *edge;

    DoubleEdgeListFace()
        : edge(nullptr)
    {
    }

    int getEdgeCount() const;
};

#endif
