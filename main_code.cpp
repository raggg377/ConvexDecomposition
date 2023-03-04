#include <bits/stdc++.h>
using namespace std;

struct Point
{
    int x;
    int y;
};

bool is_notch(Point A, Point B, Point C)
{
    int dot_product1 = (B.x - A.x) * (C.x - A.x) + (B.y - A.y) * (C.y - A.y);
    return dot_product < 0;
}
Point *rectangulate_polygon(vector<Point> &v)
{
    Point p[4];
    // p[0]=point with minimum x
    // p[1]=point with max x
    // p[2]= point with min y
    // p[3]=point with max y
    int max_x = INT_MIN, min_x = INT_MAX, max_y = INT_MIN, min_y = INT_MAX;

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].x > max_x)
        {
            max_x = p[0].x;
            p[1] = v[i];
        }
        if (v[i].y > max_y)
        {
            max_y = p[0].y;
            p[3] = v[i];
        }
        if (v[i].x < min_x)
        {
            min_x = p[0].x;
            p[0] = v[i];
        }
        if (v[i].y < min_y)
        {
            min_y = p[0].y;
            p[2] = v[i];
        }
    }
    return p;
}

class DoublyLinkedList
{
    Point p;
    Point *prev;
    Point *next;
} L;
int main()
{
    // Point p1;
    vector<Point> v;
    L li;
    i = 1;
    while (!(is_notch(v[i + 1], v[i], v[i - 1]) and is_notch(v[1], v[2], v[i + 1] and is_notch(v[i + 1], v[i], v[1]))))
    {
        i += 1;
    }
    Point notch = v[i]; // notch is at v[i]

    Point vec[4] = rectangulate_polygon(v);
    return 0;
}

bool is_inside_polygon(vector<Vertex *> L, Vertex *point)
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

        if (y >= min(p1y, p2y))
        {
            if (y <= max(p1y, p2y))
            {
                if (x <= max(p1x, p2x))
                {
                    if (p1y != p2y)
                    {
                        float xinters = (y - p1y) * (p2x - p1x) / (p2y - p1y) + p1x;
                        if (p1x == p2x || x < xinters) // Changed this line
                        {
                            cout << p1x << " " << p1y << "  " << p2x << " " << p2y << endl;
                            inside = !inside;
                            cout << "I is now: " << i << " " << xinters << endl;
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