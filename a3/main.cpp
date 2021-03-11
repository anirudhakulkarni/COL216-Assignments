#include "stdio.h"
#include "iostream"
using namespace std;
int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        freopen(argv[1], "r", stdin); // redirects standard input
    }
    else
    {
        freopen(argv[1], "r", stdin); // redirects standard input
    }

    freopen("../output.txt", "w", stdout); // redirects standard output. Any standard output is directed to file
    cout << argv[1];
    return 0;
}