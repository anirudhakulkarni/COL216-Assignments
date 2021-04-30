#include <bits/stdc++.h>

using namespace std;

class RegisterFile
{
public:    
    int get_regno(string reg);
    RegisterFile();
    int get_register_data(string register_name);
    void set_register_data(string register_name, int data);
    void printRegisters();
};