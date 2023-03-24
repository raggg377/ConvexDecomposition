#include <bits/stdc++.h>
using namespace std;

int main()
{
    ifstream file("for_input.txt");
    string line;
    string str = getline(file, line);
    cout << str << endl;
    cout <
        if (file.is_open())
    {
        // string line;
        while (getline(file, line))
        {
            // note that the newline character is not included
            // in the getline() function
            cout << line << endl;
        }
    }
}