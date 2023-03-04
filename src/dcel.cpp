#include <bits/stdc++.h>
#include "../include/dcel_vertex.hpp"
#include "../include/dcel_edge.hpp"

void dcel::make_dcel(vector<vec2> &points)
{
    int n = points.size();

    Edge *prevEdge1 = nullptr;
    Edge *prevEdge2 = nullptr;

    for (int i = 0; i < n; i++)
    {
        vec2 point = points[i];

        Vertex *vertex = new Vertex(point);
        Edge *he1 = new Edge();
        Edge *he2 = new Edge();

        he1->next = nullptr;
        he1->org = vertex;
        he1->twin = he2;

        he2->next = prevEdge2;
        he2->org = nullptr;
        he2->twin = he1;

        edgeList.push_back(he1);
        edgeList.push_back(he2);

        vertex->inc_edge = he1;
        vertexList.push_back(vertex);

        if (prevEdge1 != nullptr)
        {
            prevEdge1->next = he1;
        }

        if (prevEdge2 != nullptr)
        {
            prevEdge2->org = vertex;
        }

        prevEdge1 = he1;
        prevEdge2 = he2;
    }

    Edge *firstLeftEdge = edgeList.front();
    prevEdge1->next = firstLeftEdge;

    auto second = edgeList[next(edgeList.begin(), 1) - edgeList.begin()];
    // Edge *second = edgeList[1];
    Edge *firstRightEdge = second;
    firstRightEdge->next = prevEdge2;

    prevEdge2->org = vertexList.front();
}
