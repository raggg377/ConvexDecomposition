/**
 * @file fair.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-03-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "./include/dcel_vec2.hpp"
#include "./src/dcel_vec2.cpp"
#include "./include/dcel.hpp"
#include "./src/dcel.cpp"
#include "./include/diag.hpp"
#include "./include/lpv.hpp"

using namespace std;

dcel concave;                 ///< DCEL of the concave polygon
dcel merge_concave;           ///< DCEL of the concave used polygon
vector<int> LUP;              ///< LUP is an ordered list of integer i.e. the polygon with index i is part of the polygon with index j.
vector<bool> LDP;             ///< LDP is a boolean list which is true if polygon i is a definitive polygon after merging.
int poly_cnt = 0;             ///< Number of polygons in the decomposition
int diag_cnt = 0;             ///< Number of diagonals in the decomposition
vector<dcel> decomp;          ///< Decomposition of the concave polygon
vector<vec2> concave_polygon; ///< List of vertices of the concave polygon
vector<diag> diagList;        ///< List of diagonals in the decomposition

/**
 * @brief function to check if the angle at vertex v2 is reflex(greater than 180 degrees)
 *
 * @param v1 vertex1
 * @param v2 vertex2
 * @param v3 vertex3
 * @return true
 * @return false
 */

bool is_notch(Vertex *v1, Vertex *v2, Vertex *v3)
{
    double dot_product = (v3->pos.x - v1->pos.x) * (v2->pos.y - v1->pos.y) - (v3->pos.y - v1->pos.y) * (v2->pos.x - v1->pos.x);
    return dot_product < 0;
}

/**
 * @brief checks if the vertex is inside the minimum bounding rectangle formed by the points in the list v
 *
 * @param v list of vertices of a polygon
 * @param ver vertex to be checked
 * @return true if v is inside the minimum bounding rectangle formed by p
 * @return false if v is not inside the minimum bounding rectangle formed by p
 */
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

/**
 * @brief Get the LPVS object
 *
 * @param L list of vertices of a polygon
 * @return vector<Vertex *>
 */
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

/**
 * @brief checks if the vertex is inside the polygon
 *
 * @param vertices list of vertices of a polygon
 * @param point vertex to be checked
 * @return true if point is inside the polygon
 * @return false if point is not inside the polygon
 */
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

/**
 * @brief checks if last lies in the semiplane formed by v1 and vo
 *
 * @param v1 vertex of one endpoint of the line v1vo
 * @param vo vertex of the other endpoint of the line v1vo
 * @param last vertex to be checked
 * @return double signed area of the triangle formed by v1, vo and last
 */
double signed_area(Vertex *v1, Vertex *vo, Vertex *last)
{
    double slope = (v1->pos.y - vo->pos.y) / (v1->pos.x - vo->pos.x);
    double c = v1->pos.y - (slope * v1->pos.x);
    return (last->pos.y - (slope * last->pos.x) - c);
}

/**
 * @brief Get the Vertices VTR object which lies in the semiplane formed by v1 and vo
 *
 * @param L list of vertices of a polygon
 * @param v1 vertex1
 * @param vo vertex2
 * @return vector<Vertex *> list of vertices of VTR
 */
vector<Vertex *> getVerticesVTR(vector<Vertex *> L, Vertex *v1, Vertex *vo)
{
    vector<Vertex *> VTR;
    Vertex *last = L.back();
    double semiplane = signed_area(v1, vo, last);
    for (int i = 1; i < L.size(); i++)
    {
        if (semiplane * signed_area(v1, vo, L[i]) > 0)
        {
            VTR.push_back(L[i]);
        }
    }
    return VTR;
}

/**
 * @brief Decomposes the concave polygon into convex polygons
 *
 */
void Decompose()
{

    vector<Vertex *> L, v;

    L.push_back(concave.vertexList[0]);
    int m = 1, i;
    int n = concave.vertexList.size();
    while (n > 3)
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
            v[i + 1] = v[i]->inc_edge->next->org;
        }

        if (L.size() != concave.vertexList.size())
        {
            vector<Vertex *> LPVS = getLPVS(L);

            while (LPVS.size() > 0)
            {
                bool backward = false;
                while (!backward && LPVS.size() > 0)
                {
                    Vertex *vo = LPVS[0];
                    do
                    {
                        vo = LPVS[0];
                        if (!is_inside_rectangle(L, vo))
                        {
                            LPVS.erase(LPVS.begin() + 0);
                        }
                    } while (!(is_inside_rectangle(L, vo) || LPVS.size() == 0));
                    if (LPVS.size())
                    {
                        if (is_inside_polygon(L, vo))
                        {
                            vector<Vertex *> VTR = getVerticesVTR(L, v[1], vo);
                            for (int p = 0; p < VTR.size(); p++)
                            {
                                auto index = find(L.begin(), L.end(), VTR[p]);
                                L.erase(index);
                            }
                            backward = true;
                            auto index = find(LPVS.begin(), LPVS.end(), vo);
                            LPVS.erase(index);
                        }
                        else
                        {
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
            decomp.resize(poly_cnt + 1);
            decomp[poly_cnt].make_dcel(_temp);
            poly_cnt++;

            n = n - L.size() + 2;
            if (n > 2)
            {
                diagList.resize(diag_cnt + 1);
                for (int in1 = 0; in1 < merge_concave.vertexList.size(); in1++)
                {
                    if (L[0]->pos.x == merge_concave.vertexList[in1]->pos.x && L[0]->pos.y == merge_concave.vertexList[in1]->pos.y)
                    {
                        diagList[diag_cnt].v1 = merge_concave.vertexList[in1];
                    }
                    if (L.back()->pos.x == merge_concave.vertexList[in1]->pos.x && L.back()->pos.y == merge_concave.vertexList[in1]->pos.y)
                    {
                        diagList[diag_cnt].v2 = merge_concave.vertexList[in1];
                    }
                }
                diag_cnt++;
            }
        }
        m = m + 1;
    }
    if (n > 2)
    {
        vector<vec2> _temp;
        for (int i = 0; i < concave.vertexList.size(); i++)
        {
            _temp.push_back(concave.vertexList[i]->pos);
        }
        decomp.resize(poly_cnt + 1);
        decomp[poly_cnt].make_dcel(_temp);
        poly_cnt++;
    }
}

/**
 * @brief creates LPvj is a object of the list containing pairs (k,vr) where k is the index of a polygon containing vj as one of its vertices and vr is the next vertex to vj in that polygon k.
        These pairs are in LPvj only if vr is not the consecutive vertex of vj in the initial polygon, i.e., if (vj ,vr) is a diagonal.
 *
 * @param v Vertex j whose LPvj is to be created
 * @return vector<lpv *> LPvj
 */
vector<lpv *> make_LPV(Vertex *v)
{
    vector<lpv *> LPVs;
    int count = 0;
    for (int i = 0; i < decomp.size(); i++)
    {
        if (LDP[i] == false)
        {
            continue;
        }
        for (int j = 0; j < decomp[i].vertexList.size(); j++)
        {
            if (decomp[i].vertexList[j]->pos == v->pos)
            {
                Vertex *ne1 = decomp[i].vertexList[j]->inc_edge->next->org;
                Vertex *ne2 = decomp[i].vertexList[j]->inc_edge->twin->next->next->org;

                Vertex *one1 = v->inc_edge->next->org;
                Vertex *one2 = v->inc_edge->twin->next->next->org;

                if (!(ne1->pos == one1->pos) && !(ne1->pos == one2->pos))
                {
                    LPVs.resize(count + 1);
                    LPVs[count] = new lpv();
                    LPVs[count]->k = i;
                    LPVs[count]->ver = ne1;
                    count++;
                }
                if (!(ne2->pos == one1->pos) && !(ne2->pos == one2->pos))
                {
                    LPVs.resize(count + 1);
                    LPVs[count] = new lpv();
                    LPVs[count]->k = i;
                    LPVs[count]->ver = ne2;
                    count++;
                }
            }
        }
    }
    return LPVs;
}

/**
 * @brief checks if the vertices vs and vt both are convex or not
 *
 * @param vs endpoint of the diagonal
 * @param vt other endpoint of the diagonal
 * @param LPVs LPvj of vt
 * @return true
 * @return false
 */
bool isConvex(Vertex *vs, Vertex *vt, vector<lpv *> LPVs)
{
    bool ans = true;
    Vertex *ne1p1, *ne2p1, *ne1p2, *ne2p2;
    Vertex *v1, *v2;
    int polyind[2];
    int tmp = 0;
    for (int itr = 0; itr < LPVs.size(); itr++)
    {
        if (LPVs[itr]->ver->pos == vt->pos)
        {
            int polyindex = LPVs[itr]->k;
            polyind[tmp] = polyindex;
            tmp++;
        }
    }
    for (int in1 = 0; in1 < decomp[polyind[0]].vertexList.size(); in1++)
    {
        if (vs->pos == decomp[polyind[0]].vertexList[in1]->pos)
        {
            ne1p1 = decomp[polyind[0]].vertexList[in1]->inc_edge->next->org;
            ne2p1 = decomp[polyind[0]].vertexList[in1]->inc_edge->twin->next->next->org;
        }
    }
    for (int in1 = 0; in1 < decomp[polyind[1]].vertexList.size(); in1++)
    {
        if (vs->pos == decomp[polyind[1]].vertexList[in1]->pos)
        {
            ne1p2 = decomp[polyind[1]].vertexList[in1]->inc_edge->next->org;
            ne2p2 = decomp[polyind[1]].vertexList[in1]->inc_edge->twin->next->next->org;
        }
    }
    if (vt->pos == ne1p1->pos)
    {
        v1 = ne2p1;
    }
    else
    {
        v1 = ne1p1;
    }
    if (vt->pos == ne1p2->pos)
    {
        v2 = ne2p2;
    }
    else
    {
        v2 = ne1p2;
    }
    if (is_notch(v2, vs, v1))
        ans = false;
    return ans;
}

/**
 * @brief merges the two polygons polyind1 and polyind2
 *
 * @param polyind1 polygon to be merged
 * @param polyind2 other polygon to be merged
 * @param vs endpoint of the diagonal
 * @param vt other endpoint of the diagonal
 */
void mergePolygons(int polyind1, int polyind2, Vertex *vs, Vertex *vt)
{
    if (polyind1 == polyind2)
    {
        cout << "Error merging same polygon" << endl;
    }
    vector<vec2> _temp;
    int flagck = 0;

    for (int in1 = 0; in1 < decomp[polyind1].vertexList.size(); in1++)
    {
        if (vs->pos == decomp[polyind1].vertexList[in1]->pos)
        {
            if (!(vt->pos == decomp[polyind1].vertexList[in1]->inc_edge->twin->org->pos))
            {
                flagck = 1;
                auto ptr = decomp[polyind1].vertexList[in1]->inc_edge;
                while (!(ptr->org->pos == vt->pos))
                {
                    _temp.push_back(ptr->org->pos);
                    ptr = ptr->next;
                }
            }
            else
            {
                auto ptr = decomp[polyind1].vertexList[in1]->inc_edge->twin->org->inc_edge;
                while (!(ptr->org->pos == vs->pos))
                {
                    _temp.push_back(ptr->org->pos);
                    ptr = ptr->next;
                }
            }
            break;
        }
    }
    for (int in2 = 0; in2 < decomp[polyind2].vertexList.size(); in2++)
    {
        if (vt->pos == decomp[polyind2].vertexList[in2]->pos)
        {
            if (!(vs->pos == decomp[polyind2].vertexList[in2]->inc_edge->twin->org->pos))
            {
                auto ptr = decomp[polyind2].vertexList[in2]->inc_edge;
                while (!(ptr->org->pos == vs->pos))
                {
                    _temp.push_back(ptr->org->pos);
                    ptr = ptr->next;
                }
            }
            else
            {
                auto ptr = decomp[polyind2].vertexList[in2]->inc_edge->twin->org->inc_edge;
                while (!(ptr->org->pos == vt->pos))
                {
                    _temp.push_back(ptr->org->pos);
                    ptr = ptr->next;
                }
            }
            break;
        }
    }
    decomp.resize(poly_cnt + 1);
    decomp[poly_cnt].make_dcel(_temp);
    poly_cnt++;
}

/**
 * @brief implemets the merging algorithm
 *
 */
void Merge()
{
    int np = poly_cnt - 1;
    for (int i = 0; i < poly_cnt; i++)
    {
        LDP.push_back(true);
        LUP.push_back(i);
    }
    for (int j = 0; j < diag_cnt; j++)
    {
        Vertex *vs = diagList[j].v2;
        Vertex *vt = diagList[j].v1;
        vector<lpv *> LPVs = make_LPV(vs);
        vector<lpv *> LPVt = make_LPV(vt);
        if ((LPVs.size() > 2 && LPVt.size() > 2) ||
            (LPVs.size() > 2 && isConvex(vt, vs, LPVt)) ||
            (LPVt.size() > 2 && isConvex(vs, vt, LPVs)) ||
            (isConvex(vs, vt, LPVs) && isConvex(vt, vs, LPVt)))
        {
            Vertex *j2 = vt;
            Vertex *i2 = vs;
            Vertex *j3;
            Vertex *i3;
            Vertex *j1;
            Vertex *i1;
            for (int k = 0; k < decomp[LUP[j]].vertexList.size(); k++)
            {
                if (decomp[LUP[j]].vertexList[k]->pos == vt->pos)
                {
                    j3 = decomp[LUP[j]].vertexList[k]->inc_edge->next->org;
                }
                if (decomp[LUP[j]].vertexList[k]->pos == vs->pos)
                {
                    i1 = decomp[LUP[j]].vertexList[k]->inc_edge->twin->next->next->org;
                }
            }
            int u;
            for (int k = 0; k < LPVt.size(); k++)
            {
                if (LPVt[k]->ver->pos == vs->pos && LPVt[k]->k != LUP[j] && LDP[LPVt[k]->k] != false)
                {
                    u = LPVt[k]->k;
                    break;
                }
            }
            for (int k = 0; k < decomp[u].vertexList.size(); k++)
            {
                if (decomp[u].vertexList[k]->pos == vt->pos)
                {
                    j1 = decomp[u].vertexList[k]->inc_edge->twin->next->next->org;
                }
                if (decomp[u].vertexList[k]->pos == vs->pos)
                {

                    i3 = decomp[u].vertexList[k]->inc_edge->next->org;
                }
            }
            if (!is_notch(i1, i2, i3) && !is_notch(j1, j2, j3))
            {
                np++;
                mergePolygons(LUP[j], u, vs, vt);
                LDP.resize(np + 1, true);

                LDP[j] = false;
                LDP[u] = false;
                LDP[np] = true;
                LUP[j] = np;
                if (u >= LUP.size())
                {
                    for (int i = LUP.size(); i < u + 1; i++)
                    {
                        LUP.push_back(i);
                    }
                }
                LUP[u] = np;

                for (int h = 0; h < np - 1; h++)
                {
                    if (LUP[h] == j || LUP[h] == u)
                    {
                        if (h >= LUP.size())
                        {
                            for (int i = LUP.size(); i < h + 1; i++)
                            {
                                LUP.push_back(i);
                            }
                        }
                        LUP[h] = np;
                    }
                }
            }
        }
    }
}

int main()
{

    ifstream inputFile("./testcases/testcase7.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error: input.txt not Found." << endl;
        exit(0);
    }
    char line[1000000];
    inputFile.getline(line, 1000000);
    char ch;
    float x, y;
    stringstream ss(line);
    while (ss >> ch >> x >> ch >> ws >> y >> ch)
    {
        vec2 point = vec2(x, y);
        concave_polygon.push_back(point);
        ss >> ch;
    }
    concave.make_dcel(concave_polygon);
    merge_concave.make_dcel(concave_polygon);
    Decompose();
    ofstream decompFile("./decompose/decompose7.txt");
    if (!decompFile.is_open())
    {
        cerr << "Error: decomp.txt not Found." << endl;
        exit(0);
    }
    for (int i = 0; i < decomp.size(); i++)
    {
        for (int j = 0; j < decomp[i].vertexList.size(); j++)
        {
            decompFile << "(" << decomp[i].vertexList[j]->pos.x << ", " << decomp[i].vertexList[j]->pos.y << "), ";
        }
        decompFile << endl;
    }
    Merge();
    ofstream mergeFile("./merge/merge7.txt");
    if (!mergeFile.is_open())
    {
        cerr << "Error: merge.txt not Found." << endl;
        exit(0);
    }
    for (int i = 0; i < decomp.size(); i++)
    {
        if (LDP[i])
        {
            for (int j = 0; j < decomp[i].vertexList.size(); j++)
            {
                mergeFile << "(" << decomp[i].vertexList[j]->pos.x << ", " << decomp[i].vertexList[j]->pos.y << "), ";
            }
            mergeFile << endl;
        }
    }
}