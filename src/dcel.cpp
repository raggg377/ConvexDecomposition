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

void dcel::addEdges(int i, int j)
{
    auto iter_i = next(vertexList.begin(), i);
    auto vertex1 = *iter_i;
    auto iter_j = next(vertexList.begin(), j);
    auto vertex2 = *iter_j;

    addEdges(vertex1, vertex2);
}

void dcel::addEdges(Vertex *v1, Vertex *v2)
{
    // auto face = new DoubleEdgeListFace();

    auto left = new Edge();
    auto right = new Edge();

    // auto referenceDoubleEdgeListFace = getReferenceFace(v1, v2);

    // auto prev1 = getPreviousEdge(v1, referenceDoubleEdgeListFace);
    // auto prev2 = getPreviousEdge(v2, referenceDoubleEdgeListFace);

    // auto prev1 = v1->inc_edge;

    // face->edge = left;
    // referenceDoubleEdgeListFace->edge = right;

    // left->face = face;
    left->next = prev2->next;
    left->origin = v1;
    left->twin = right;

    // right->face = referenceDoubleEdgeListFace;
    right->next = prev1->next;
    right->origin = v2;
    right->twin = left;

    prev1->next = left;
    prev2->next = right;

    auto curr = left->next;
    while (curr != left)
    {
        // curr->face = face;
        curr = curr->next;
    }

    edgeList.push_back(left);
    edgeList.push_back(right);

    // faces.push_back(face);
}

Edge *dcel::getPreviousEdge(
    Vertex *vertex,
    DoubleEdgeListFace *face)
{
    auto twin = vertex->leaving->twin;
    auto edge = vertex->leaving->twin->next->twin;

    while (edge != twin)
    {
        if (edge->face == face)
        {
            return edge;
        }
        edge = edge->next->twin;
    }

    return edge;
}

DoubleEdgeListFace *dcel::getReferenceFace(
    Vertex *v1,
    Vertex *v2)
{
    if (v1->leaving->face == v2->leaving->face)
    {
        return v1->leaving->face;
    }

    auto e1 = v1->leaving->twin->next->twin;
    while (e1 != v1->leaving->twin)
    {
        auto e2 = v2->leaving->twin->next->twin;
        while (e2 != v2->leaving->twin)
        {
            if (e1->face == e2->face)
            {
                return e1->face;
            }
            e2 = e2->next->twin;
        }
        e1 = e1->next->twin;
    }

    return v1->leaving->face;
}

void dcel::removeHalfEdges(int index)
{
    auto iter = next(edgeList.begin(), index);
    auto e = *iter;
    removeHalfEdges(e);
}

void dcel::removeHalfEdges(Edge *edge)
{
    DoubleEdgeListFace *face = edge->twin->face;

    auto ePrev = edge->getPrevious();
    auto tPrev = edge->twin->getPrevious();
    auto eNext = edge->next;
    auto tNext = edge->twin->next;

    ePrev->next = tNext;
    tPrev->next = eNext;
    eNext->prev = ePrev;
    eNext->prev = tPrev;

    face->edge = eNext;

    auto te = eNext;
    while (te != tNext)
    {
        te->face = face;
        te = te->next;
    }

    faces.remove(edge->face);
    delete edge->face;

    auto twin = edge->twin;

    edgeList.remove(edge);
    edgeList.remove(twin);
    delete edge;
    delete twin;
}