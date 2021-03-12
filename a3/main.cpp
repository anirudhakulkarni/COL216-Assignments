#include "stdio.h"
#include "iostream"
#include "fstream"
#include <string>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;
template <class T>
string to_string(T t, ios_base &(*f)(ios_base &))
{
    // Helper function for printRegisterFile
    ostringstream oss;
    oss << f << t;
    return oss.str();
}

void printRegisterFile(map<string, pair<int, int>> registerFile)
{
    // Input: array containing register file
    // output: prints containts of register file
    // Assumptions: size of array is 32
    map<string, pair<int, int>>::iterator itr;
    cout << "\nThe map registerFile is : \n";
    cout << "\tKEY\tELEMENT\n";
    for (itr = registerFile.begin(); itr != registerFile.end(); ++itr)
    {
        cout << '\t' << itr->first
             << '\t' << itr->second.first << '\n';
    }
}
void initializeRegisterFile(map<string, pair<int, int>> &registerFile)
{
    registerFile.insert(make_pair("$zero", make_pair(0, 0)));
    registerFile.insert(make_pair("$at", make_pair(1, 0)));
    registerFile.insert(make_pair("$v0", make_pair(2, 0)));
    registerFile.insert(make_pair("$v1", make_pair(3, 0)));
    registerFile.insert(make_pair("$a0", make_pair(4, 0)));
    registerFile.insert(make_pair("$a1", make_pair(5, 0)));
    registerFile.insert(make_pair("$a2", make_pair(6, 0)));
    registerFile.insert(make_pair("$a3", make_pair(7, 0)));
    registerFile.insert(make_pair("$t0", make_pair(8, 0)));
    registerFile.insert(make_pair("$t1", make_pair(9, 0)));
    registerFile.insert(make_pair("$t2", make_pair(10, 0)));
    registerFile.insert(make_pair("$t3", make_pair(11, 0)));
    registerFile.insert(make_pair("$t4", make_pair(12, 0)));
    registerFile.insert(make_pair("$t5", make_pair(13, 0)));
    registerFile.insert(make_pair("$t6", make_pair(14, 0)));
    registerFile.insert(make_pair("$t7", make_pair(15, 0)));
    registerFile.insert(make_pair("$s0", make_pair(16, 0)));
    registerFile.insert(make_pair("$s1", make_pair(17, 0)));
    registerFile.insert(make_pair("$s2", make_pair(18, 0)));
    registerFile.insert(make_pair("$s3", make_pair(19, 0)));
    registerFile.insert(make_pair("$s4", make_pair(20, 0)));
    registerFile.insert(make_pair("$s5", make_pair(21, 0)));
    registerFile.insert(make_pair("$s6", make_pair(22, 0)));
    registerFile.insert(make_pair("$s7", make_pair(23, 0)));
    registerFile.insert(make_pair("$t8", make_pair(24, 0)));
    registerFile.insert(make_pair("$t9", make_pair(25, 0)));
    registerFile.insert(make_pair("$k0", make_pair(26, 0)));
    registerFile.insert(make_pair("$k1", make_pair(27, 0)));
    registerFile.insert(make_pair("$gp", make_pair(28, 0)));
    registerFile.insert(make_pair("$sp", make_pair(29, 0)));
    registerFile.insert(make_pair("$fp", make_pair(30, 0)));
    registerFile.insert(make_pair("$ra", make_pair(31, 0)));
}
int main(int argc, char const *argv[])
{
    // Input file section
    ofstream outfile;
    ifstream infile;
    if (argc < 2)
    {
        infile.open("input.txt");
    }
    else
    {
        infile.open(argv[1]);
    }

    // Declarations
    map<string, pair<int, int>> registerFile; // Register name => (registr number, Value)
    initializeRegisterFile(registerFile);
    printRegisterFile(registerFile);
    return 0;
}
