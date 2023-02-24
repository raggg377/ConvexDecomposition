#ifndef DCEL_FACE_HPP
#define DCEL_FACE_HPP

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
