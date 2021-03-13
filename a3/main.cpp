#include "stdio.h"
#include "iostream"
#include "fstream"
#include <string>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;
template <class T>
string to_string(T t, ios_base &(*f)(ios_base &)) // DONT change its name. will not work.
{
    // Helper function for printRegisterFile
    // to_string<long>(integer, hex)
    ostringstream oss;
    oss << f << t;
    return oss.str();
}
string remove_extra_whitespaces(const string &input)
{
    string output;
    unique_copy(input.begin(), input.end(), back_insert_iterator<string>(output), [](char a, char b) { return isspace(a) && isspace(b); });
    if (output[0] == ' ')
    {
        output = output.substr(1);
    }
    return output;
}

class RegisterFile
{
    /*
    $zero     $0            Hardware set to 0
    $at       $1            Assembler temporary
    $v0 - $v1 $2 - $3       Function result (low/high)
    $a0 - $a3 $4 - $7       Argument Register 1
    $t0 - $t7 $8 - $15      Temporary registers
    $s0 - $s7 $16 - $23     Saved registers
    $t8 - $t9 $24 - $25     Temporary registers
    $k0 - $k1 $26 - $27     Reserved for OS kernel
    $gp       $28           Global pointer
    $sp       $29           Stack pointer
    $fp       $30           Frame pointer
    $ra       $31           Return address
    */
private:
    int regArray[32];
    string regNameArray[32] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
    int get_regno(string s)
    {

        if (s == "zero")
            return 0;
        else if (s == "at")
            return 1;
        else if (s[0] == 'v')
        {
            if (s[1] != '1' || s[1] != '0')
                return -1;
            else
                return s[1] - '0' + 2;
        }
        else if (s[0] == 'a')
        {
            int x = s[1] - '0';
            if (x < 0 || x > 3)
                return -1;
            else
                return x + 4;
        }
        else if (s[0] == 't')
        {
            int x = s[1] - '0';
            if (x < 0 || x > 9)
                return -1;
            if (x < 8)
                return 8 + x;
            else
                return x + 16;
        }
        else if (s[0] == 's')
        {
            int x = s[1] - '0';
            if (x < 0 || x > 7)
                return -1;
            else
                return x + 8;
        }
        else if (s[0] == 'k')
        {
            return -1;
        }
        else if (s == "gp")
        {
            return 28;
        }
        else if (s == "sp")
        {
            return 29;
        }
        else if (s == "fp")
        {
            return 30;
        }
        else if (s == "ra")
        {
            return 31;
        }
        else
            return -1;
    }

public:
    RegisterFile()
    {
        for (int it = 0; it < 32; it++)
            regArray[it] = 0;
    }
    bool operate(string inst)
    {
        string op, reg1, reg2, reg3;
        if (op == "add" || op == "mul" || op == "sub" || op == "slt")
        {

            int ind1 = get_regno(reg1);
            int ind2 = get_regno(reg2);
            int ind3 = get_regno(reg3);

            if (ind1 < 0 || ind2 < 0 || ind3 < 0)
                return false;

            if (op == "add")
            {
                regArray[ind1] = regArray[ind2] + regArray[ind3];
            }
            else if (op == "sub")
            {
                regArray[ind1] = regArray[ind2] - regArray[ind3];
            }
            else if (op == "mul")
            {
                regArray[ind1] = regArray[ind2] * regArray[ind3];
            }
            else if (op == "slt")
            {
                regArray[ind2] < regArray[ind3] ? regArray[ind1] = 1 : regArray[ind1] = 0;
            }
            return true;
        }
    }
    int get_register_data(string register_name)
    {
        return regArray[get_regno(register_name)];
    }
    void set_register_data(string register_name, int data)
    {
        regArray[get_regno(register_name)] = data;
    }
    void printRegisters()
    {
        cout << "Contents of Register file:\n";
        for (int i = 0; i < 31; i++)
        {
            cout << "$" << regNameArray[i] << ": " << to_string<long>(regArray[i], hex) << ", ";
        }
        cout << "$" << regNameArray[31] << ": " << to_string<long>(regArray[31], hex);
    }
};
class MemoryArray
{
private:
public:
    void storeInstr(string instruction)
    {
        return;
    }
    int getAddOfLabel(string label)
    {
        return 0;
    }
    string getFirstInstr()
    {
        return "Sunny didi";
    }
    string getNextInstr(string currentInstr)
    {
        // Take care of jump instructions. If current is jump or branch then use map to get address of label and return that instruction. Do appropriately
        return "Johny bhaiyya";
    }
};

vector<string> getInstructions(ifstream &infile)
{
    vector<string> temp;
    string test2;
    while (getline(infile, test2))
    {
        temp.push_back(remove_extra_whitespaces(test2));
    }
    return temp;
}

void processInstructions(vector<string> instructionVector, RegisterFile &registerFile, MemoryArray &memory)
{
    int instructionsSoFar = 1;
    for (int i = 0; i < instructionVector.size(); i++)
    {
        memory.storeInstr(instructionVector[i]);
    }
    string currentInstr = memory.getFirstInstr();
    while (currentInstr != "")
    {
        /* PROCESS INSTRUCTION*/
        cout << "Instruction number executed: " << instructionsSoFar << endl;
        registerFile.printRegisters();
        currentInstr = memory.getNextInstr(currentInstr);
        instructionsSoFar++;
    }
}

int main(int argc, char const *argv[])
{
    // Input file section
    ifstream infile;
    if (argc < 2)
    {
        infile.open("../input.txt", ios::in);
    }
    else
    {
        infile.open(argv[1]);
    }
    vector<string> instructionVector = getInstructions(infile);
    // Declarations
    RegisterFile registerFile;
    MemoryArray memory;
    processInstructions(instructionVector, registerFile, memory);
    registerFile.set_register_data("t0", INT32_MAX + 1);
    cout << registerFile.get_register_data("t0") << endl;
    registerFile.printRegisters();

    return 0;
}

//EXTRA

// void printRegisterFile(map<string, pair<int, int>> registerFile)
// {
//     // Input: array containing register file
//     // output: prints containts of register file
//     // Assumptions: size of array is 32
//     map<string, pair<int, int>>::iterator itr;
//     cout << "\nThe map registerFile is : \n";
//     cout << "\tKEY\tELEMENT\n";
//     for (itr = registerFile.begin(); itr != registerFile.end(); ++itr)
//     {
//         cout << '\t' << itr->first
//              << '\t' << itr->second.first << '\n';
//     }
// }
// void initializeRegisterFile(map<string, pair<int, int>> &registerFile)
// {
//     registerFile.insert(make_pair("$zero", make_pair(0, 0)));
//     registerFile.insert(make_pair("$at", make_pair(1, 0)));
//     registerFile.insert(make_pair("$v0", make_pair(2, 0)));
//     registerFile.insert(make_pair("$v1", make_pair(3, 0)));
//     registerFile.insert(make_pair("$a0", make_pair(4, 0)));
//     registerFile.insert(make_pair("$a1", make_pair(5, 0)));
//     registerFile.insert(make_pair("$a2", make_pair(6, 0)));
//     registerFile.insert(make_pair("$a3", make_pair(7, 0)));
//     registerFile.insert(make_pair("$t0", make_pair(8, 0)));
//     registerFile.insert(make_pair("$t1", make_pair(9, 0)));
//     registerFile.insert(make_pair("$t2", make_pair(10, 0)));
//     registerFile.insert(make_pair("$t3", make_pair(11, 0)));
//     registerFile.insert(make_pair("$t4", make_pair(12, 0)));
//     registerFile.insert(make_pair("$t5", make_pair(13, 0)));
//     registerFile.insert(make_pair("$t6", make_pair(14, 0)));
//     registerFile.insert(make_pair("$t7", make_pair(15, 0)));
//     registerFile.insert(make_pair("$s0", make_pair(16, 0)));
//     registerFile.insert(make_pair("$s1", make_pair(17, 0)));
//     registerFile.insert(make_pair("$s2", make_pair(18, 0)));
//     registerFile.insert(make_pair("$s3", make_pair(19, 0)));
//     registerFile.insert(make_pair("$s4", make_pair(20, 0)));
//     registerFile.insert(make_pair("$s5", make_pair(21, 0)));
//     registerFile.insert(make_pair("$s6", make_pair(22, 0)));
//     registerFile.insert(make_pair("$s7", make_pair(23, 0)));
//     registerFile.insert(make_pair("$t8", make_pair(24, 0)));
//     registerFile.insert(make_pair("$t9", make_pair(25, 0)));
//     registerFile.insert(make_pair("$k0", make_pair(26, 0)));
//     registerFile.insert(make_pair("$k1", make_pair(27, 0)));
//     registerFile.insert(make_pair("$gp", make_pair(28, 0)));
//     registerFile.insert(make_pair("$sp", make_pair(29, 0)));
//     registerFile.insert(make_pair("$fp", make_pair(30, 0)));
//     registerFile.insert(make_pair("$ra", make_pair(31, 0)));
// }