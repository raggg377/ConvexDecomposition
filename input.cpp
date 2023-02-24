#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "./include/dcel_vec2.hpp"

using namespace std;

vector<vec2> concave_polygon;

bool is_obtuse(vec2 v1, vec2 v2, vec2 v3)
{
    int dot_product1 = (v2.x - v1.x) * (v3.x - v1.x) + (v2.y - v1.y) * (v3.y - v1.y);
    return dot_product < 0;
}

void Decompose()
{
    vector<vec2> L, v;
    v[0] = vec2();
    L.push_back(concave_polygon[0]);
    int m = 2, n, l, index, i;
    while (n > 3)
    {
        l = L.size();
        v[1] = vec2(L[l - 1].x, L[l - 1].y);
        index = find(concave_polygon.begin(), concave_polygon.end(), v[1]) - concave_polygon.begin();
        v[2] = vec2(concave_polygon[index + 1].x, concave_polygon[index + 1].y);
        L.clear();
        L.push_back(v[1]);
        L.push_back(v[2]);
        i = 2;
        index = find(concave_polygon.begin(), concave_polygon.end(), v[i]) - concave_polygon.begin();
        v[i + 1] = vec2(concave_polygon[index + 1].x, concave_polygon[index + 1].y);
        while (is_obtuse(v[i - 1], v[i], v[i + 1]) && is_obtuse(v[i], v[i + 1], v[1]) && is_obtuse(v[i + 1], v[1], v[2]) && L.size() < n)
        {
            L.push_back(v[i + 1]);
            i++;
            index = find(concave_polygon.begin(), concave_polygon.end(), v[i]) - concave_polygon.begin();
            v[i + 1] = vec2(concave_polygon[index + 1].x, concave_polygon[index + 1].y);
        }
        if (L.size() != concave_polygon.size())
        {
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

    for (auto p : concave_polygon)
        cout << "(" << p.x << ", " << p.y << "), ";

    vector<vec2> L, v;
    v[0] = vec2();
    L.push_back(concave_polygon[0]);
    int m = 2, n, l, index, i;
    while (n > 3)
    {
        l = L.size();
        v[1] = vec2(L[l - 1].x, L[l - 1].y);
        index = find(concave_polygon.begin(), concave_polygon.end(), v[1]) - concave_polygon.begin();
        v[2] = vec2(concave_polygon[index + 1].x, concave_polygon[index + 1].y);
        L.clear();
        L.push_back(v[1]);
        L.push_back(v[2]);
        i = 2;
        index = find(concave_polygon.begin(), concave_polygon.end(), v[i]) - concave_polygon.begin();
        v[i + 1] = vec2(concave_polygon[index + 1].x, concave_polygon[index + 1].y);
    }
}