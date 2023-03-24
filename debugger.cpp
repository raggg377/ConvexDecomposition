#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "./include/dcel_vec2.hpp"
#include "./src/dcel_vec2.cpp"
#include "./include/dcel.hpp"
#include "./src/dcel.cpp"

using namespace std;

dcel concave;
int dcnt = 0;
vector<dcel> decomp;
vector<vec2> concave_polygon;
int vertex_counter = 0;
bool is_notch(Vertex *v1, Vertex *v2, Vertex *v3)
{
    double dot_product = (v3->pos.x - v1->pos.x) * (v2->pos.y - v1->pos.y) - (v3->pos.y - v1->pos.y) * (v2->pos.x - v1->pos.x);
    return dot_product < 0;
}

bool is_inside_rectangle(vector<Vertex *> v, Vertex *ver)
{
    int max_x = INT_MIN, min_x = INT_MAX, max_y = INT_MIN, min_y = INT_MAX;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i]->pos.x > max_x)
        {
            max_x = v[i]->pos.x;
        }
        if (v[i]->pos.y > max_y)
        {
            max_y = v[i]->pos.y;
        }
        if (v[i]->pos.x < min_x)
        {
            min_x = v[i]->pos.x;
        }
        if (v[i]->pos.y < min_y)
        {
            min_y = v[i]->pos.y;
        }
    }
    if (min_x < ver->pos.x && max_x > ver->pos.x && min_y < ver->pos.y && max_y > ver->pos.y)
        return true;
    return false;
}

vector<Vertex *> getLPVS(vector<Vertex *> L)
{
    vector<Vertex *> LPVS;
    for (int i = 0; i < concave.vertexList.size(); i++)
    {
        auto _t = find(L.begin(), L.end(), concave.vertexList[i]);
        if (_t == L.end() && is_notch(concave.vertexList[i]->inc_edge->prev->org, concave.vertexList[i], concave.vertexList[i]->inc_edge->next->org))
        {
            LPVS.push_back(concave.vertexList[i]);
        }
    }
    return LPVS;
}

bool is_inside_polygon(vector<Vertex *> vertices, Vertex *point)
{
    int n = vertices.size();
    double crossProduct = 0;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        double dx1 = vertices[i]->pos.x - point->pos.x;
        double dy1 = vertices[i]->pos.y - point->pos.y;
        double dx2 = vertices[j]->pos.x - point->pos.x;
        double dy2 = vertices[j]->pos.y - point->pos.y;
        double cp = dx1 * dy2 - dx2 * dy1;
        if (i == 0)
        {
            crossProduct = cp;
        }
        else if ((crossProduct > 0 && cp < 0) || (crossProduct < 0 && cp > 0))
        {
            // If the sign of the cross product changes, the point is outside the polygon
            return false;
        }
    }
    // If we make it to the end without finding a sign change, the point is inside the polygon
    return true;
}
double vector_line_value(Vertex *v1, Vertex *vo, Vertex *last)
{
    return last->pos.y - (v1->pos.y - vo->pos.y) / (v1->pos.x - vo->pos.x) * last->pos.x;
}

vector<Vertex *> getVerticesVTR(vector<Vertex *> L, Vertex *v1, Vertex *vo)
{
    vector<Vertex *> VTR;
    // double slope = (v1->pos.y - vo->pos.y) / (v1->pos.x - vo->pos.x);
    Vertex *last = L.back();
    double semiplane = vector_line_value(v1, vo, last);

    for (int i = 0; i < L.size(); i++)
    {
        if (semiplane * vector_line_value(v1, vo, L[i]) < 0)
        {
            VTR.push_back(L[i]);
        }
    }
    return VTR;
}

void Decompose()
{
    vector<Vertex *> L, v;
    L.push_back(concave.vertexList[0]);
    int m = 1, i;
    int n = concave.vertexList.size();
    int ctr = 16;
    while (n > 3 and ctr--)
    {
        v.resize(n + 1);
        v[1] = L.back();
        v[2] = v[1]->inc_edge->next->org;
        L.clear();
        L.push_back(v[1]);
        L.push_back(v[2]);
        i = 2;
        v[i + 1] = v[i]->inc_edge->next->org;
        while (!(is_notch(v[i - 1], v[i], v[i + 1]) or is_notch(v[i], v[i + 1], v[1]) or is_notch(v[i + 1], v[1], v[2])) && L.size() < n)
        {
            auto _t = find(L.begin(), L.end(), v[i + 1]);
            if (_t == L.end())
                L.push_back(v[++i]);
            // cout << v[i]->pos.x << " " << v[i]->pos.y << endl;
            v[i + 1] = v[i]->inc_edge->next->org;
        }
        if (L.size() != concave.vertexList.size())
        {
            vector<Vertex *> LPVS = getLPVS(L);
            while (LPVS.size() > 0)
            {
                // make rectangle

                bool backward = false;
                while (!backward && LPVS.size() > 0)
                {
                    Vertex *vo = LPVS[0];
                    do
                    {
                        vo = LPVS[0];
                        // cout << vo->pos.x << " " << vo->pos.y << endl;
                        if (!is_inside_rectangle(L, vo))
                        {
                            LPVS.erase(LPVS.begin() + 0);
                            // LPVS.push_back(vo);
                        }
                    } while (!(is_inside_rectangle(L, vo) || LPVS.size() == 0));
                    if (LPVS.size())
                    {
                        // cout << m << " are we getting here?" << endl;
                        if (is_inside_polygon(L, vo))
                        {
                            L = getVerticesVTR(L, v[1], vo);
                            backward = true;
                            auto index = find(LPVS.begin(), LPVS.end(), vo);
                            LPVS.erase(index);
                        }
                    }
                }
            }
        }
        if (L.back() != v[2])
        {
            vector<vec2> _temp;
            for (int i = 0; i < L.size(); i++)
            {
                _temp.push_back(L[i]->pos);
            }
            for (int i = 1; i < L.size() - 1; i++)
            {
                auto index = find(concave.vertexList.begin(), concave.vertexList.end(), L[i]) - concave.vertexList.begin();
                concave.removeVertex(concave.vertexList[index]);
            }
            decomp[dcnt++].make_dcel(_temp);
            // decomp[dcnt - 1].print_dcel();
            n = n - L.size() + 2;
        }
        m = m + 1;
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
    double x, y;
    stringstream ss(line);
    while (ss >> ch >> x >> ch >> ws >> y >> ch)
    {
        cout << x << " " << y << endl;
        vec2 point = vec2(x, y);
        concave_polygon.push_back(point);
        ss >> ch; // Ignore the comma separator
    }
    concave.make_dcel(concave_polygon);
    decomp.resize(concave_polygon.size() / 3);
    cout << "Initial vertices: " << endl;
    concave.print_dcel();

    // cout << "intial edges: " << endl;
    // concave.print_dcel_edges();
    vec2 v1p = vec2(0, 0);
    vec2 v11 = vec2(1, 3);
    vec2 v12 = vec2(1, 4);
    vec2 v13 = vec2(2, 4);
    vec2 v16 = vec2(2, 4);
    vec2 v14 = vec2(2, 3);
    vec2 v15 = vec2(3, 3);

    Vertex *v = new Vertex(v1p);
    Vertex *v1 = new Vertex(v11);
    Vertex *v2 = new Vertex(v12);
    Vertex *v3 = new Vertex(v13);
    Vertex *v4 = new Vertex(v14);
    Vertex *v5 = new Vertex(v15);
    Vertex *v6 = new Vertex(v16);
    vector<Vertex *> l;
    l.push_back(v1);
    l.push_back(v2);
    l.push_back(v3);
    l.push_back(v6);
    l.push_back(v4);
    l.push_back(v5);
    Decompose();
    ofstream decompFile("decomp2.txt");
    if (!decompFile.is_open())
    {
        cerr << "Error: decomp.txt not Found." << endl;
        exit(0);
    }
    cout << "decomp size: " << decomp.size() << endl;
    for (int i = 0; i < decomp.size(); i++)
    {
        for (int j = 0; j < decomp[i].vertexList.size(); j++)
        {
            decompFile << "(" << decomp[i].vertexList[j]->pos.x << ", " << decomp[i].vertexList[j]->pos.y << "), ";
        }
        decompFile << endl;
    }
}