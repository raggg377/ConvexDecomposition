#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "./include/dcel_vec2.hpp"
#include "./src/dcel_vec2.cpp"
#include "./include/dcel.hpp"
#include "./src/dcel.cpp"

using namespace std;

dcel concave;

vector<vec2> concave_polygon;
int vertex_counter = 0;
bool is_obtuse(Vertex *v1, Vertex *v2, Vertex *v3)
{
    int dot_product = (v2->pos.x - v1->pos.x) * (v3->pos.x - v1->pos.x) + (v2->pos.y - v1->pos.y) * (v3->pos.y - v1->pos.y);
    return dot_product < 0;
}

bool is_inside_rectangle(vector<Vertex *> v, Vertex *ver)
{
    Vertex *extremes[4];
    // vector<Vertex *> extremes;
    // extremes[0]=point with minimum x
    // extremes[1]=point with max x
    // extremes[2]= point with min y
    // extremes[3]=point with max y
    int max_x = INT_MIN, min_x = INT_MAX, max_y = INT_MIN, min_y = INT_MAX;

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i]->pos.x > max_x)
        {
            max_x = extremes[0]->pos.x;
            extremes[1] = v[i];
        }
        if (v[i]->pos.y > max_y)
        {
            max_y = extremes[0]->pos.y;
            extremes[3] = v[i];
        }
        if (v[i]->pos.x < min_x)
        {
            min_x = extremes[0]->pos.x;
            extremes[0] = v[i];
        }
        if (v[i]->pos.y < min_y)
        {
            min_y = extremes[0]->pos.y;
            extremes[2] = v[i];
        }
    }

    // return extremes;
}

vector<Vertex *> getLPVS(vector<Vertex *> L)
{
    vector<Vertex *> LPVS;
    for (int i = 0; i < concave.vertexList.size(); i++)
    {
        auto _t = find(L.begin(), L.end(), concave.vertexList[i]);
        if (_t == L.end() && is_obtuse(concave.vertexList[i]->inc_edge->prev->org, concave.vertexList[i], concave.vertexList[i]->inc_edge->next->org))
        {
            LPVS.push_back(concave.vertexList[i]);
        }
    }
    return LPVS;
}

bool pointInL(Vertex *point, vector<Vertex *> L)
{
    int n = L.size();
    bool inside = false;
    float x = point->pos.x;
    float y = point->pos.y;
    float p1x = L[0]->pos.x;
    float p1y = L[0]->pos.y;

    for (int i = 1; i <= n; i++)
    {
        float p2x = L[i % n]->pos.x;
        float p2y = L[i % n]->pos.y;

        if (y > min(p1y, p2y))
        {
            if (y <= max(p1y, p2y))
            {
                if (x <= max(p1x, p2x))
                {
                    if (p1y != p2y)
                    {
                        float xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x;
                        if (p1x == p2x || x <= xinters)
                        {
                            inside = !inside;
                        }
                    }
                }
            }
        }
        p1x = p2x;
        p1y = p2y;
    }
    return inside;
}

void Decompose()
{
    vector<Vertex *> L, v;
    L.push_back(concave.vertexList[0]);
    int m = 1, i;
    int n = concave.vertexList.size();
    while (n > 3)
    {
        v[1] = L.back();
        v[2] = v[1]->inc_edge->next->org;
        L.clear();
        L.push_back(v[1]);
        L.push_back(v[2]);
        i = 2;
        v[i + 1] = v[i]->inc_edge->next->org;
        while (is_obtuse(v[i - 1], v[i], v[i + 1]) && is_obtuse(v[i], v[i + 1], v[1]) && is_obtuse(v[i + 1], v[1], v[2]) && L.size() < n)
        {
            auto _t = find(L.begin(), L.end(), v[i + 1]);
            if (_t == L.end())
                L.push_back(v[++i]);
            v[i + 1] = v[i]->inc_edge->next->org;
        }
        if (L.size() == concave.vertexList.size())
        {
            vector<Vertex *> LPVS = getLPVS(L);
            while (LPVS.size() > 0)
            {
                // make rectangle

                bool backward = false;
                while (!backward && LPVS.size() > 0)
                {
                    do
                    {
                        Vertex *v = LPVS[0];
                        if (!is_inside_rectangle(L, v))
                        {
                            LPVS.erase(LPVS.begin() + 0);
                            LPVS.push_back(v);
                        }
                    } while (is_inside_rectangle(L, v) || LPVS.size() == 0);
                    if (LPVS.size())
                    {
                        if (is_inside_polygon())
                    }
                }
            }
        }
    }
}

int main()
{

    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error: input.txt not Found." << endl;
        exit(0);
    }
    char line[1000];
    inputFile.getline(line, 1000);
    char ch;
    float x, y;
    stringstream ss(line);
    while (ss >> ch >> x >> ch >> ws >> y >> ch)
    {
        vec2 point = vec2(x, y);
        concave_polygon.push_back(point);
        ss >> ch; // Ignore the comma separator
    }
    concave.make_dcel(concave_polygon);
}