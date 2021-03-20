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

vector<string> parseInstr(string instruction)
{
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
int main(int argc, char const *argv[])
{
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

    string currentInstr = instructionVector[0];
    while (currentInstr != "")
    {
        executionOfInstructionCount[programCounter]++;
        vector<string> parametersVec = parseInstr(currentInstr);
        if ()
    }
    return 0;
}
