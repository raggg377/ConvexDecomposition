#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "./include/dcel_vec2.hpp"

using namespace std;

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
    vector<vec2> concave_polygon;
    stringstream ss(line);
    while (ss >> ch >> x >> ch >> ws >> y >> ch)
    {
        vec2 point = vec2(x, y);
        concave_polygon.push_back(point);
        ss >> ch; // Ignore the comma separator
    }

    for (auto p : concave_polygon)
        cout << "(" << p.x << ", " << p.y << "), ";
}