#include <bits/stdc++.h>
using namespace std;

int main()
{
    ifstream inputFile("for_input.txt");
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
    string str;
    int n;
    getline(cin, str);
    n = stoi(str);
    cout << n << endl;
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
    Merge();
    ofstream mergeFile("merged.txt");
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