#include <bits/stdc++.h>
using namespace std;

int main()
{
    ifstream inputFile("for_input.txt");

    if (inputFile.is_open())
    {
        string line;
        while (getline(file, line))
        {
            // note that the newline character is not included
            // in the getline() function
            cout << line << endl;
        }
    }
}