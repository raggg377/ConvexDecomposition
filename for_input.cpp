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
        }
}