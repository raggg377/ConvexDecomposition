#include <iostream>
#include <vector>
using namespace std;

bool point_in_polygon(pair<int, int> point, vector<pair<int, int>> &polygon)
{
    int num_intersections = 0;
    for (int i = 0; i < polygon.size(); i++)
    {
        // get the next vertex in the polygon
        pair<int, int> p1 = polygon[i];
        // get the previous vertex in the polygon
        pair<int, int> p2 = polygon[(i + polygon.size() - 1) % polygon.size()];
        // check if the point lies on the edge of the polygon
        if (point_on_edge(point, p1, p2))
        {
            return true;
        }
        // check if the ray intersects with the edge of the polygon
        if (intersect(point, make_pair(INT_MIN, point.second), p1, p2))
        {
            num_intersections++;
        }
    }
    // if the number of intersections is odd, the point is inside the polygon
    return num_intersections % 2 == 1;
}

bool point_on_edge(pair<int, int> point, pair<int, int> p1, pair<int, int> p2)
{
    if (point == p1 || point == p2)
    {
        return true;
    }
    if (point.second < min(p1.second, p2.second) || point.second > max(p1.second, p2.second))
    {
        return false;
    }
    if (p1.second == p2.second)
    {
        return false;
    }
    int x = (point.second - p1.second) * (p2.first - p1.first) / (p2.second - p1.second) + p1.first;
    return point.first == x;
}

int direction(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3)
{
    return (p3.first - p1.first) * (p2.second - p1.second) - (p2.first - p1.first) * (p3.second - p1.second);
}

bool on_segment(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3)
{
    return min(p1.first, p2.first) <= p3.first && p3.first <= max(p1.first, p2.first) &&
           min(p1.second, p2.second) <= p3.second && p3.second <= max(p1.second, p2.second);
}

bool intersect(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3, pair<int, int> p4)
{
    int d1 = direction(p3, p4, p1);
    int d2 = direction(p3, p4, p2);
    int d3 = direction(p1, p2, p3);
    int d4 = direction(p1, p2, p4);
    if ((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0))
    {
        if ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))
        {
            return true;
        }
    }
    if (d1 == 0 && on_segment(p3, p4, p1))
    {
        return true;
    }
    if (d2 == 0 && on_segment(p3, p4, p2))
    {
        return true;
    }
    if (d3 == 0 && on_segment(p1, p2, p3))
    {
        return true;
    }
    if (d4 == 0 && on_segment(p1, p2, p4))
    {
        return true;
    }
    return false;
}
