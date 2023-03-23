#include <bits/stdc++.h>
#include "../include/dcel_vertex.hpp"
#include "../include/dcel_edge.hpp"

void dcel::make_dcel(vector<vec2> &points)
{
    // for (int i = 0; i < vertexList.size(); i++)
    // {
    //     // vertexList[i]->inc_edge = NULL;
    //     delete vertexList[i];
    // }
    // for (int i = 0; i < edgeList.size(); i++)
    // {
    //     // edgeList[i]->twin = NULL;
    //     // edgeList[i]->org = NULL;
    //     // edgeList[i]->next = NULL;
    //     // edgeList[i]->prev = NULL;
    //     delete edgeList[i];
    // }
    // vertexList.clear();
    // edgeList.clear();

    int n = points.size();
    Edge *prevEdge1 = nullptr;
    Edge *prevEdge2 = nullptr;
    cout << "n = " << n << endl;
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
        cout << "i =" << i << endl;

        edgeList.push_back(he1);
        cout << "i =" << i << endl;

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
    cout << "exiting for" << endl;
    Edge *firstLeftEdge = edgeList.front();
    prevEdge1->next = firstLeftEdge;

    auto second = edgeList[next(edgeList.begin(), 1) - edgeList.begin()];
    // Edge *second = edgeList[1];
    Edge *firstRightEdge = second;
    firstRightEdge->next = prevEdge2;

    prevEdge2->org = vertexList.front();

    for (int j = 0; j < edgeList.size(); j++)
    {
        Edge *edge = edgeList[j]->twin->next->twin;
        // walk around the face
        while (edge->next != edgeList[j])
        {
            edge = edge->next->twin;
        }
        edgeList[j]->prev = edge;
    }
}

void dcel::print_dcel()
{
    for (int i = 0; i < vertexList.size(); i++)
    {
        cout << " vertex " << i << " : " << vertexList[i]->pos.x << ", " << vertexList[i]->pos.y << endl;
        cout << " vertex of inc edge" << i << " : " << vertexList[i]->inc_edge->next->org->pos.x << " -> " << vertexList[i]->inc_edge->next->org->pos.y << endl;
    }
}
void dcel::print_dcel_edges()
{
    for (int i = 0; i < edgeList.size(); i++)
    {
        cout << "origin vertex " << i << " : " << edgeList[i]->org->pos.x << " , " << edgeList[i]->org->pos.y << endl;
        cout << "prevedge" << i << " : " << edgeList[i]->prev->org->pos.x << " , " << edgeList[i]->prev->org->pos.y << endl;
        cout << "nextedge" << i << " : " << edgeList[i]->next->org->pos.x << " , " << edgeList[i]->next->org->pos.y << endl;
    }
}

// Assume that the DCEL has a `Vertex` class with an `id` field and a `HalfEdge` class with `origin`, `twin`, `next`, and `prev` fields.

void dcel::removeVertex(Vertex *v)
{
    // Remove all half-edges incident on the vertex.
    Edge *e1 = v->inc_edge;
    Edge *e2 = v->inc_edge->twin->next;
    Edge *e3 = v->inc_edge->twin;
    Edge *e4 = v->inc_edge->twin->next->twin;
    Vertex *nextVertex = v->inc_edge->next->org;
    Vertex *prevVertex = v->inc_edge->twin->next->next->org;

    Edge *ne1 = new Edge();
    Edge *ne2 = new Edge();

    prevVertex->inc_edge = ne1;

    ne1->org = prevVertex;
    ne2->org = nextVertex;
    ne1->twin = ne2;
    ne2->twin = ne1;
    ne1->prev = e4->prev;
    ne2->prev = e3->prev;
    ne1->next = e1->next;
    ne2->next = e2->next;

    e2->next->prev = ne2;
    e4->prev->next = ne1;
    e1->next->prev = ne1;
    e3->prev->next = ne2;
    auto index = find(edgeList.begin(), edgeList.end(), e1);
    edgeList.erase(index);
    index = find(edgeList.begin(), edgeList.end(), e2);
    edgeList.erase(index);
    index = find(edgeList.begin(), edgeList.end(), e3);
    edgeList.erase(index);
    index = find(edgeList.begin(), edgeList.end(), e4);
    edgeList.erase(index);
    delete e1;
    delete e2;
    delete e3;
    delete e4;

    edgeList.push_back(ne1);
    edgeList.push_back(ne2);

    auto index2 = find(vertexList.begin(), vertexList.end(), v);
    vertexList.erase(index2);
    delete v;
}
