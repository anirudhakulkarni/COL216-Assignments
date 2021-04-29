#include <bits/stdc++.h>
#include "RegisterFile.h"
#include "MemoryUnit.h"
#define fo(i, N) for (int i = 0 ; i < N; i++)

using namespace std;

int row_access_delay = 10;
int col_access_delay = 2;
const int N = 8; // Number of cores
template <class T>
string to_string(T t, ios_base &(*f)(ios_base &));
bool chk_empty(string instruction);
string remove_extra_whitespaces(const string &input);
vector<string> parseInstr(string instruction);
int getIndexofChar(string str, char c);
RegisterFile registerFile[N];
vector<string> instructionVector[N];
MemoryUnit memory;
int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);                
    cout.tie(NULL);
    fstream infile;
    string test2;
    
    for(int cor = 1; cor <= N; cor++){
        string file = "../t" + to_string(cor) + ".txt"; 
        infile.open(file, ios::in);
        while (getline(infile, test2))
        {
            if (chk_empty(test2) == true){
                continue;
            }
            instructionVector[cor].push_back(test2);
        }
        instructionVector[cor-1].push_back("EOF");
    }

    int executionOfInstructionCount[N][999] = {0};
    fo(core, N){
        for (int i = 0; i < instructionVector[core].size(); i++)
        {
            string temp = instructionVector[core][i];
            memory.storeInstr(temp, core);
        }
    }
    
    return 0;
}

bool chk_empty(string instruction){
    for(int i = 0; i < instruction.length(); i++){
        if ((instruction[i]!=' ') && (instruction[i] != '\t') && (instruction[i] != '\n')) {
            return false;
        }
    }
    return true;
}
string remove_extra_whitespaces(const string &input)
{
    string output;
    unique_copy(input.begin(), input.end(), back_insert_iterator<string>(output), [](char a, char b) { return isspace(a) && isspace(b); });
    if (output[0] == ' ')
    {
        output = output.substr(1);
    }
    for (int i = 0; i < output.length() - 1; i++)
    {
        if (output[i] == ',' && output[i + 1] != ' ')
        {
            output = output.substr(0, i + 1) + " " + output.substr(i + 1);
        }
    }

    return output;
}
template <class T>
string to_string(T t, ios_base &(*f)(ios_base &))
{
    ostringstream oss;
    oss << f << t;
    return oss.str();
}
vector<string> parseInstr(string instruction)
{
    vector<string> ans;
    while (instruction.length() > 0 && (instruction[0] == ' ' || instruction[0] == '\t'))
    {
        instruction = instruction.substr(1);
    }
    while (instruction.length() > 0 && (instruction[instruction.length() - 1] == ' ' || instruction[instruction.length() - 1] == '\n' || instruction[instruction.length() - 1] == '\t'))
    {
        instruction = instruction.substr(0, instruction.length() - 1);
    }
    if (instruction.length() == 0){
        return ans;
    }
    if (instruction.find(":") != string::npos){
        ans.push_back(instruction);
        return ans;
    }
    if (instruction[0] == 'j')
    {
        ans.push_back("j");
        instruction = instruction.substr(1);
        while (instruction.length() > 0 && (instruction[0] == ' ' || instruction[0] == '\t'))
        {
            instruction = instruction.substr(1);
        }
        while (instruction.length() > 0 && (instruction[instruction.length()-1] == ' ' || instruction[instruction.length()-1] == '\t')){
            instruction = instruction.substr(0, instruction.length() - 1);
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
int getIndexofChar(string str, char c){
    int a = 0;
    while (str[a] != c){
        a++;
    }
    return a;
}