#include "stdio.h"
#include "iostream"
#include "fstream"
using namespace std;

int main(int argc, char const *argv[])
{
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
    outfile.open("../output.txt");
    outfile << "F";
    cout << argv[1];
    return 0;
}
int getRegisterNo(string reg)
{
    // cout << reg << endl;
    int regno = -1;
    if (reg.compare("$zero") == 0)
        regno = 0;
    else if (reg.compare("$at") == 0)
        regno = 1;
    else if (reg.compare("$v0") == 0)
        regno = 2;
    else if (reg.compare("$v1") == 0)
        regno = 3;
    else if (reg.compare("$a0") == 0)
        regno = 4;
    else if (reg.compare("$a1") == 0)
        regno = 5;
    else if (reg.compare("$a2") == 0)
        regno = 6;
    else if (reg.compare("$a3") == 0)
        regno = 7;
    else if (reg.compare("$t0") == 0)
        regno = 8;
    else if (reg.compare("$t1") == 0)
        regno = 9;
    else if (reg.compare("$t2") == 0)
        regno = 10;
    else if (reg.compare("$t3") == 0)
        regno = 11;
    else if (reg.compare("$t4") == 0)
        regno = 12;
    else if (reg.compare("$t5") == 0)
        regno = 13;
    else if (reg.compare("$t6") == 0)
        regno = 14;
    else if (reg.compare("$t7") == 0)
        regno = 15;
    else if (reg.compare("$s0") == 0)
        regno = 16;
    else if (reg.compare("$s1") == 0)
        regno = 17;
    else if (reg.compare("$s2") == 0)
        regno = 18;
    else if (reg.compare("$s3") == 0)
        regno = 19;
    else if (reg.compare("$s4") == 0)
        regno = 20;
    else if (reg.compare("$s5") == 0)
        regno = 21;
    else if (reg.compare("$s6") == 0)
        regno = 22;
    else if (reg.compare("$s7") == 0)
        regno = 23;
    else if (reg.compare("$t8") == 0)
        regno = 24;
    else if (reg.compare("$t9") == 0)
        regno = 25;
    else if (reg.compare("$k0") == 0)
        regno = 26;
    else if (reg.compare("$k1") == 0)
        regno = 27;
    else if (reg.compare("$gp") == 0)
        regno = 28;
    else if (reg.compare("$sp") == 0)
        regno = 29;
    else if (reg.compare("$fp") == 0)
        regno = 30;
    else if (reg.compare("$ra") == 0)
        regno = 31;
    return regno;
}