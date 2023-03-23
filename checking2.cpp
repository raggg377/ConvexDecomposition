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

dcel concave;
dcel merge_concave;
vector<int> LUP;
vector<bool> LDP;
int poly_cnt = 0;
int diag_cnt = 0;
vector<dcel> decomp;
vector<vec2> concave_polygon;
vector<diag> diagList;
int vertex_counter = 0;
bool is_notch(Vertex *v1, Vertex *v2, Vertex *v3)
{
    double dot_product = (v3->pos.x - v1->pos.x) * (v2->pos.y - v1->pos.y) - (v3->pos.y - v1->pos.y) * (v2->pos.x - v1->pos.x);
    return dot_product < 0;
}

bool is_inside_rectangle(vector<Vertex *> v, Vertex *ver)
{
    // Vertex *extremes[4];
    //  vector<Vertex *> extremes;
    //  extremes[0]=point with minimum x
    //  extremes[1]=point with max x
    //  extremes[2]= point with min y
    //  extremes[3]=point with max y
    int max_x = INT_MIN, min_x = INT_MAX, max_y = INT_MIN, min_y = INT_MAX;

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i]->pos.x > max_x)
        {
            max_x = v[i]->pos.x;
            // extremes[1] = v[i];
        }
        if (v[i]->pos.y > max_y)
        {
            max_y = v[i]->pos.y;
            // extremes[3] = v[i];
        }
        if (v[i]->pos.x < min_x)
        {
            min_x = v[i]->pos.x;
            // extremes[0] = v[i];
        }
        if (v[i]->pos.y < min_y)
        {
            min_y = v[i]->pos.y;
            // extremes[2] = v[i];
        }
    }
    // cout << min_x << " " << max_x << " " << min_y << " " << max_y << endl;
    if (min_x < ver->pos.x && max_x > ver->pos.x && min_y < ver->pos.y && max_y > ver->pos.y)
        return true;
    return false;
}

vector<Vertex *> getLPVS(vector<Vertex *> L)
{
    // cout << "just inside get LPVS" << endl;
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
double signed_area(Vertex *v1, Vertex *vo, Vertex *last)
{
    double slope = (v1->pos.y - vo->pos.y) / (v1->pos.x - vo->pos.x);
    double c = v1->pos.y - (slope * v1->pos.x);
    return (last->pos.y - (slope * last->pos.x) - c);
}

vector<Vertex *> getVerticesVTR(vector<Vertex *> L, Vertex *v1, Vertex *vo)
{
    vector<Vertex *> VTR;
    // double slope = (v1->pos.y - vo->pos.y) / (v1->pos.x - vo->pos.x);
    Vertex *last = L.back();
    double semiplane = signed_area(v1, vo, last);
    // cout << "v1: " << v1->pos.x << " " << v1->pos.y << endl;
    // cout << "vo: " << vo->pos.x << " " << vo->pos.y << endl;
    // cout << "last: " << last->pos.x << " " << last->pos.y << endl;
    // cout << "should be zero: " << signed_area(v1, vo, vo) << endl;
    // cout << "slope :" << semiplane << endl;
    for (int i = 1; i < L.size(); i++)
    {
        if (semiplane * signed_area(v1, vo, L[i]) > 0)
        {
            VTR.push_back(L[i]);
        }
    }
    // cout << "printing vtr: " << VTR.size() << endl;
    for (int vt = 0; vt < VTR.size(); vt++)
    {
        // cout << vt << " " << VTR[vt]->pos.x << " " << VTR[vt]->pos.y << endl;
    }
    return VTR;
}

void Decompose()
{

    vector<Vertex *> L, v;

    L.push_back(concave.vertexList[0]);
    int m = 1, i;
    int n = concave.vertexList.size();
    // int ctr = 16;
    while (n > 3)
    {
        v.resize(n + 1);
        cout << "inside while loop, n is: " << n << " and m is: " << m << endl;
        // cout << "size of L is: " << L.size() << endl;
        // cout << "checking L.back just before v1" << endl;
        // cout << L.back()->pos.x << " " << L.back()->pos.y << endl;
        v[1] = L.back();
        // cout << "printing incident edge of v1" << endl;
        // cout << v[1]->inc_edge->org->pos.x << " " << v[1]->pos.y << endl;
        v[2] = v[1]->inc_edge->next->org;
        // cout << "v1 and v2 are: " << endl;
        // cout << v[1]->pos.x << " " << v[1]->pos.y << endl;
        // cout << v[2]->pos.x << " " << v[2]->pos.y << endl;
        L.clear();
        L.push_back(v[1]);
        L.push_back(v[2]);
        // cout << "size of L: " << L.size() << endl;
        i = 2;
        v[i + 1] = v[i]->inc_edge->next->org;
        // cout << "v[i+1]: " << v[i + 1]->pos.x << " " << v[i + 1]->pos.y << endl;
        // cout << "rest of the contents of L for i: " << i << endl;
        while (!(is_notch(v[i - 1], v[i], v[i + 1]) or is_notch(v[i], v[i + 1], v[1]) or is_notch(v[i + 1], v[1], v[2])) && L.size() < n)
        {
            auto _t = find(L.begin(), L.end(), v[i + 1]);
            if (_t == L.end())
                L.push_back(v[++i]);
            // cout << v[i]->pos.x << " " << v[i]->pos.y << endl;
            v[i + 1] = v[i]->inc_edge->next->org;
        }
        // cout << "Notch Found and size of L: " << L.size() << endl;
        for (int num = 0; num < i; num++)
        {
            // cout << v[i - 1]->pos.x << " " << v[i - 1]->pos.y << endl;
            // cout << L[num]->pos.x << " " << L[num]->pos.y << endl;
            // cout << v[i + 1]->pos.x << " " << v[i + 1]->pos.y << endl;
        }

        // cout << "size of L is: " << L.size() << endl;
        if (L.size() != concave.vertexList.size())
        {
            vector<Vertex *> LPVS = getLPVS(L);
            // cout << "printing LPVS" << endl;

            for (int num = 0; num < LPVS.size(); num++)
            {
                // cout << v[i - 1]->pos.x << " " << v[i - 1]->pos.y << endl;
                // cout << LPVS[num]->pos.x << " " << LPVS[num]->pos.y << endl;
                // cout << v[i + 1]->pos.x << " " << v[i + 1]->pos.y << endl;
            }
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
                            // cout << "Are we inside this if" << endl;
                            LPVS.erase(LPVS.begin() + 0);
                            // LPVS.push_back(vo);
                        }
                    } while (!(is_inside_rectangle(L, vo) || LPVS.size() == 0));
                    // cout << "size of LPVS" << LPVS.size() << endl;
                    if (LPVS.size())
                    {
                        // cout << m << " are we getting here?" << endl;
                        if (is_inside_polygon(L, vo))
                        {
                            // cout << "Hello" << endl;
                            vector<Vertex *> VTR = getVerticesVTR(L, v[1], vo);
                            for (int p = 0; p < VTR.size(); p++)
                            {
                                auto index = find(L.begin(), L.end(), VTR[p]);
                                L.erase(index);
                            }
                            // cout << L.size() << endl;
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
        // cout << "checking L.back" << endl;
        // cout << L[0]->pos.x << " " << L[0]->pos.y << endl;
        if (L.back() != v[2])
        {
            cout << "we are now here in " << m << "th iteration" << endl;
            //  dcel d;
            cout << "decomposed: " << endl;
            vector<vec2> _temp;
            for (int i = 0; i < L.size(); i++)
            {
                _temp.push_back(L[i]->pos);
                cout << i << " " << _temp[i].x << " " << _temp[i].y << endl;
            }

            for (int i = 1; i < L.size() - 1; i++)
            {
                auto index = find(concave.vertexList.begin(), concave.vertexList.end(), L[i]) - concave.vertexList.begin();
                concave.removeVertex(concave.vertexList[index]);
            }
            // cout << "we are here now" << endl;
            // cout << poly_cnt << endl;
            decomp.resize(poly_cnt + 1);
            decomp[poly_cnt].make_dcel(_temp);
            poly_cnt++;

            n = n - L.size() + 2;
            if (n > 2)
            {
                // cout << "Iteration: " << m << endl;
                // cout << "L diag size is" << L.size() << endl;
                // for (int i = 0; i < merge_concave.vertexList.size(); i++)
                // {
                //     cout << merge_concave.vertexList[i]->pos.x << " " << merge_concave.vertexList[i]->pos.y << endl;
                // }
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

                // cout << "printing L.back" << L[L.size() - 1]->pos.x << " " << L[L.size() - 1]->pos.y << endl;
                // cout << diagList[diag_cnt].v2->pos.x << " " << diagList[diag_cnt].v2->pos.y << endl;

                diag_cnt++;
            }
            // cout << "n after decrement: " << n << endl;
        }
        m = m + 1;
    }
    if (n > 2)
    {
        vector<vec2> _temp;
        for (int i = 0; i < concave.vertexList.size(); i++)
        {
            _temp.push_back(concave.vertexList[i]->pos);
            // cout << i << " " << _temp[i].x << " " << _temp[i].y << endl;
        }
        decomp.resize(poly_cnt + 1);
        decomp[poly_cnt].make_dcel(_temp);
        poly_cnt++;
    }
}

vector<lpv *> make_LPV(Vertex *v)
{
    vector<lpv *> LPVs;
    int count = 0;
    // cout << "inside LPVs" << endl;
    for (int i = 0; i < decomp.size(); i++)
    {
        if (LDP[i] == false)
        {
            cout << "False for i " << i << endl;
            continue;
        }
        // cout << "i is: " << i << endl;
        for (int j = 0; j < decomp[i].vertexList.size(); j++)
        {
            if (decomp[i].vertexList[j]->pos == v->pos)
            {
                // cout << "j is: " << j << endl;
                Vertex *ne1 = decomp[i].vertexList[j]->inc_edge->next->org;
                Vertex *ne2 = decomp[i].vertexList[j]->inc_edge->twin->next->next->org;

                Vertex *one1 = v->inc_edge->next->org;
                Vertex *one2 = v->inc_edge->twin->next->next->org;

                if (!(ne1->pos == one1->pos) && !(ne1->pos == one2->pos))
                {
                    // cout << "inside first if" << endl;
                    LPVs.resize(count + 1);
                    LPVs[count] = new lpv();
                    LPVs[count]->k = i;
                    LPVs[count]->ver = ne1;
                    // cout << LPVs[count] << endl;
                    // cout << "resized" << endl;
                    count++;
                }
                if (!(ne2->pos == one1->pos) && !(ne2->pos == one2->pos))
                {
                    // cout << "inside second if" << endl;
                    LPVs.resize(count + 1);
                    // cout << "resized" << endl;
                    LPVs[count] = new lpv();
                    LPVs[count]->k = i;
                    LPVs[count]->ver = ne2;
                    // cout << LPVs[count]->k << " ver: " << LPVs[count]->ver->pos.x << " " << LPVs[count]->ver->pos.y << endl;
                    count++;
                }
            }
        }
    }

    cout << "Printing LPVs for ";
    cout << v->pos.x << " " << v->pos.y << endl;
    for (int y = 0; y < LPVs.size(); y++)
    {
        cout << LPVs[y]->k << " ver: " << LPVs[y]->ver->pos.x << " " << LPVs[y]->ver->pos.y << endl;
    }
    cout << "Exiting LPVS" << endl;
    return LPVs;
}

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
            // cout << "Polyindex is: " << polyindex << endl;
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

void mergePolygons(int polyind1, int polyind2, Vertex *vs, Vertex *vt)
{
    if (polyind1 == polyind2)
    {
        cout << "Error merging same polygon" << endl;
    }
    cout << "Merging indices: " << polyind1 << " " << polyind2 << endl;
    vector<vec2> _temp;
    int flagck = 0;

    for (int in1 = 0; in1 < decomp[polyind1].vertexList.size(); in1++)
    {
        if (vs->pos == decomp[polyind1].vertexList[in1]->pos) // found vs
        {
            // vs is not pointing to vt
            if (!(vt->pos == decomp[polyind1].vertexList[in1]->inc_edge->twin->org->pos))
            {
                flagck = 1;
                // cout << "In 1.1 " << endl;
                auto ptr = decomp[polyind1].vertexList[in1]->inc_edge;
                while (!(ptr->org->pos == vt->pos))
                {
                    _temp.push_back(ptr->org->pos);
                    ptr = ptr->next;
                }
            }
            else
            {
                // cout << "In 1.2 " << endl;
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
        // finding vt
        // cout << decomp[polyind2].vertexList[in2]->pos.x << " " << decomp[polyind2].vertexList[in2]->pos.y << endl;
        if (vt->pos == decomp[polyind2].vertexList[in2]->pos)
        {
            // cout << "vt Found" << endl;
            // if vt is not poitning to vs
            if (!(vs->pos == decomp[polyind2].vertexList[in2]->inc_edge->twin->org->pos))
            {
                // cout << "vs Found" << endl;
                // cout << "In 2.1 " << endl;
                auto ptr = decomp[polyind2].vertexList[in2]->inc_edge;
                while (!(ptr->org->pos == vs->pos))
                {
                    _temp.push_back(ptr->org->pos);
                    ptr = ptr->next;
                }
            }
            else
            {
                // cout << "In 2.2 " << endl;
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
    cout << "Merged polygon is: " << endl;
    decomp[poly_cnt].print_dcel();
    poly_cnt++;
}

void Merge()
{
    int np = poly_cnt - 1;
    cout << "Intial np " << np << endl;
    for (int i = 0; i < poly_cnt; i++)
    {
        LDP.push_back(true);
        LUP.push_back(i);
    }
    for (int j = 0; j < diag_cnt; j++)
    {
        cout << "start of for j is now: " << j << endl;
        // for (int k = 0; k < di; k++)
        // {

        // }
        Vertex *vs = diagList[j].v2;
        Vertex *vt = diagList[j].v1;
        cout << "vt " << vt->pos.x << " " << vt->pos.y << endl;
        cout << "vs " << vs->pos.x << " " << vs->pos.y << endl;

        vector<lpv *> LPVs = make_LPV(vs);
        vector<lpv *> LPVt = make_LPV(vt);
        if ((LPVs.size() > 2 && LPVt.size() > 2) ||
            (LPVs.size() > 2 && isConvex(vt, vs, LPVt)) ||
            (LPVt.size() > 2 && isConvex(vs, vt, LPVs)) ||
            (isConvex(vs, vt, LPVs) && isConvex(vt, vs, LPVt)))
        {
            cout << "Are we reaching here?" << endl;
            Vertex *j2 = vt;
            cout << "j2 " << j2->pos.x << " " << j2->pos.y << endl;
            Vertex *i2 = vs;
            cout << "i2 " << i2->pos.x << " " << i2->pos.y << endl;
            Vertex *j3;
            Vertex *i3;
            Vertex *j1;
            Vertex *i1;
            for (int k = 0; k < decomp[j].vertexList.size(); k++)
            {
                if (decomp[j].vertexList[k]->pos == vt->pos)
                {
                    j3 = decomp[j].vertexList[k]->inc_edge->next->org;
                    cout << "j3 " << j3->pos.x << " " << j3->pos.y << endl;
                }
                if (decomp[j].vertexList[k]->pos == vs->pos)
                {
                    i1 = decomp[j].vertexList[k]->inc_edge->twin->next->next->org;
                    cout << "i1 " << i1->pos.x << " " << i1->pos.y << endl;
                }
            }
            int u;
            for (int k = 0; k < LPVt.size(); k++)
            {
                if (LPVt[k]->ver->pos == vs->pos && LPVt[k]->k != j && LDP[LPVt[k]->k] != false)
                {
                    u = LPVt[k]->k;
                    break;
                }
            }
            cout << "u: " << u << endl;
            for (int k = 0; k < decomp[u].vertexList.size(); k++)
            {
                if (decomp[u].vertexList[k]->pos == vt->pos)
                {
                    j1 = decomp[u].vertexList[k]->inc_edge->twin->next->next->org;
                    cout << "j1 " << j1->pos.x << " " << j1->pos.y << endl;
                }
                if (decomp[u].vertexList[k]->pos == vs->pos)
                {

                    i3 = decomp[u].vertexList[k]->inc_edge->next->org;
                    cout << "i3 " << i3->pos.x << " " << i3->pos.y << endl;
                }
            }
            if (!is_notch(i1, i2, i3) && !is_notch(j1, j2, j3))
            {
                cout << "Reaching in if here np is: " << np << endl;
                np++;
                // Write NPth polygon
                cout << "This value should be 7: " << LUP[u] << endl;
                mergePolygons(LUP[j], LUP[u], vs, vt);
<<<<<<< HEAD
                LDP.resize(np + 1, true);

=======
                // LDP.resize(np);
                // LUP.resize(u);
                // LDP[j] = false;
                // // LDP[u] = false;
                // if (u >= LDP.size())
                //     LDP.push_back(false);
                // else
                //     LDP[u] = false;
                // // LDP[np] = true;
                // if (np >= LDP.size())
                //     LDP.push_back(true);
                // else
                //     LDP[np] = true;
                // // LDP.push_back(true);
                // LUP[j] = np;
                // if (u >= LUP.size())
                //     LUP.push_back(np);
                // else
                //     LUP[u] = np;
                // LUP.push_back(np);
                LDP.resize(np);
                // LUP.resize(u);
                cout << "LUP size: " << LUP.size() << " LDP size: " << LDP.size() << endl;
                cout << "u: " << u << " np: " << np << endl;
>>>>>>> 866d38b2f6404bdef8125ec01c64eefea73e22a7
                LDP[j] = false;
                LDP[u] = false;
                LDP[np] = true;
                LUP[j] = np;
                // LUP[u] = np;
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
<<<<<<< HEAD
                        if (h >= LUP.size())
                        {
                            for (int i = LUP.size(); i < h + 1; i++)
                            {
                                LUP.push_back(i);
                            }
                        }
=======
                        cout << "h: " << h << endl;
>>>>>>> 866d38b2f6404bdef8125ec01c64eefea73e22a7
                        LUP[h] = np;
                    }
                }
                cout << "LUP size: " << LUP.size() << " LDP size: " << LDP.size() << endl;
                cout << "u: " << u << " np: " << np << endl;
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
    // const int SIZE = ;
    char line[100000];
    inputFile.getline(line, 100000);
    char ch;
    float x, y;
    stringstream ss(line);
    while (ss >> ch >> x >> ch >> ws >> y >> ch)
    {
        // cout << "ch is now: " << ch << endl;
        // cout << x << " " << y << endl;
        vec2 point = vec2(x, y);
        concave_polygon.push_back(point);
        ss >> ch; // Ignore the comma separator
    }
    // cout << "intial input: " << concave_polygon.size() << endl;
    concave.make_dcel(concave_polygon);
    merge_concave.make_dcel(concave_polygon);
    // cout << "Initial vertices: " << endl;
    // concave.print_dcel();

    // cout << "intial edges: " << endl;
    // concave.print_dcel_edges();

    // cout << is_inside_polygon(l, v) << endl;
    // not a notch

    // cout << isInsideConvexPolygon(l, v) << endl;
    Decompose();
    ofstream decompFile("decomp3.txt");
    if (!decompFile.is_open())
    {
        cerr << "Error: decomp.txt not Found." << endl;
        exit(0);
    }
    // cout << "decomp size: " << decomp.size() << endl;
    for (int i = 0; i < decomp.size(); i++)
    {
        for (int j = 0; j < decomp[i].vertexList.size(); j++)
        {
            decompFile << "(" << decomp[i].vertexList[j]->pos.x << ", " << decomp[i].vertexList[j]->pos.y << "), ";
        }
        decompFile << endl;
    }
    // cout << "Diagonals" << endl;
    // for (int i = 0; i < diagList.size(); i++)
    // {
    //     cout << diagList[i].v1->pos.x << " " << diagList[i].v1->pos.y << " | ";
    //     cout << diagList[i].v2->pos.x << " " << diagList[i].v2->pos.y << endl;
    // }
    // Merge();
    // ofstream mergeFile("merged.txt");
    // if (!mergeFile.is_open())
    // {
    //     cerr << "Error: merge.txt not Found." << endl;
    //     exit(0);
    // }
    // for (int i = 0; i < decomp.size(); i++)
    // {
    //     if (LDP[i])
    //     {
    //         for (int j = 0; j < decomp[i].vertexList.size(); j++)
    //         {
    //             mergeFile << "(" << decomp[i].vertexList[j]->pos.x << ", " << decomp[i].vertexList[j]->pos.y << "), ";
    //         }
    //         mergeFile << endl;
    //     }
    // }
    // cout << "Vertex is: " << merge_concave.vertexList[4]->pos.x << " " << merge_concave.vertexList[4]->pos.y << endl;
    // vector<lpv *> LPVs = make_LPV(merge_concave.vertexList[4]);

    // lpv *obj = new lpv(0, merge_concave.vertexList[2]);
    // lpv *obj = new lpv();
    // cout << "End of file are we here?" << endl;

    // obj->k = 0;
    // cout << "End of file are we here?" << endl;

    // obj->ver = merge_concave.vertexList[0];
    // cout << "End of file are we here?" << endl;

    // cout << obj->ver->pos.x << endl;
    cout << "End of file are we here?" << endl;
    // cout<< decomp.size() << endl;
}