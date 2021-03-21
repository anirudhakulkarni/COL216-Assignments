#include "stdio.h"
#include "iostream"
#include "fstream"
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#define fio                           \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL)
using namespace std;
int ROW_ACCESS_DELAY = 10;
int COL_ACCESS_DELAY = 2;
vector<string> parseInstr(string instruction)
{
    cout << "THIS IS INSIDE" << ROW_ACCESS_DELAY << endl;
    vector<string> ans;
    while (instruction.length() > 0 && instruction[0] == ' ')
    {
        instruction = instruction.substr(1);
    }
    while (instruction.length() > 0 && (instruction[instruction.length() - 1] == ' ' || instruction[instruction.length() - 1] == '\n'))
    {
        instruction = instruction.substr(0, instruction.length() - 1);
    }
    if (instruction[0] == 'j')
    {
        ans.push_back("j");
        instruction = instruction.substr(1);
        while (instruction.length() > 0 && instruction[0] == ' ')
        {
            instruction = instruction.substr(1);
        }
        ans.push_back(instruction);
        return ans;
    }
    string instr;
    while (instruction.length() > 0 && instruction[0] != ' ')
    {
        instr += instruction[0];
        instruction = instruction.substr(1);
    }
    string registers = "";
    for (int i = 0; i < instruction.length(); i++)
    {
        if (instruction[i] != ' ')
        {
            registers += instruction[i];
        }
    }
    ans.push_back(instr);
    stringstream ss(registers);
    while (ss.good())
    {
        string substr;
        getline(ss, substr, ',');
        ans.push_back(substr);
    }
    return ans;
}

bool isDepedent(string instr1, string instr2)
{
    // instr 1 is of lw or sw type which is running in background
    vector<string> instr1parsed = parseInstr(instr1);
    vector<string> instr2parsed = parseInstr(instr2);
    string workingRegister = instr1parsed[1];
    if (instr1parsed[0] == "lw")
    {
        if (instr2parsed[0] == "add")
        {
            if (workingRegister == instr2parsed[1] || workingRegister == instr2parsed[2] || workingRegister == instr2parsed[3])
            {
                return true;
            }
            return false;
        }
        if (instr2parsed[0] == "addi")
        {
            if (workingRegister == instr2parsed[1] || workingRegister == instr2parsed[2])
            {
                return true;
            }
            return false;
        }
        if (instr2parsed[0] == "sw")
        {
            if (workingRegister == instr2parsed[1])
            {
                return true;
            }
            return false;
        }
        return false;
    }
    else
    {
        if (instr2parsed[0] == "lw")
        {
            // if memory address is being used is accessed in instr2
            if (instr2parsed[2] == instr1parsed[2])
            {
                return true;
            }
            return false;
        }
        return false;
    }
}
int getNUpdateWork(int &activeRowStart, int currAdd)
{
    int ans = 2 * ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
    if (activeRowStart == -1024)
    {
        ans = ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
    }
    else if (int(currAdd / 1024) * 1024 == activeRowStart)
    {
        ans = ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
    }
    activeRowStart = currAdd;
    return ans;
}
int main(int argc, char const *argv[])
{
    ROW_ACCESS_DELAY = strtol(argv[2], NULL, 10);
    cout << isDepedent("sw $t1, 12", "lw $t2, 12") << endl;
    cout << isDepedent("sw $t1, 12", "lw $t2, 120") << endl;
    vector<string> instructionVector = {"sw $t1, 12", "lw $t2, 120"};
    int executionOfInstructionCount[999];
    int instructionsSoFar = 1;
    int programCounter = 0;
    int activeRowStart = -1024;
    vector<string> cylcles;
    string pastInstruction = "add $t1, 0";
    int currCycle = 1;
    int spareCycles = 1;
    string currentInstr = instructionVector[0];

    cout << getNUpdateWork(activeRowStart, 1000) << endl;
    cout << getNUpdateWork(activeRowStart, 1024) << endl;
    cout << getNUpdateWork(activeRowStart, 1028) << endl;

    cout << stoi("1000");
    cout << argv[1] << argv[2] << endl;

    cout << "OUTSIDE " << ROW_ACCESS_DELAY << endl;
    return 0;
}
