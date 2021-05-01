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
MemoryUnit memory;
int clock_cycle = -1;
vector<vector<vector<string>>> cycleinfoofalln;
int programCounter[N] = {0};
struct Instruction {
    string kind; // add, sub, mul, addi, lw, sw, beq, bne, slt, j, label
    string label;
    int attributes[3]; 
    string line; //stores entire instruction
    //for add, sub, mul, slt --- stores register number of all registers
    //for addi, stores register numbers of Rdest and Rsrc and const
    //for beq, bne, stores the register numbers of R1 and R2 and label stores the label
    //for lw / sw , stores the Register number of Rdest, offset and registerNumber of R3.
    void setInstruction(string instruction){
        line = instruction;
        if (instruction == "EOF"){
            kind = "EOF";
            return;
        }
        vector<string> params = parseInstr(instruction);
        if (params.size() == 1){
            kind = label;
            label = params[0];
            return;
        }
        kind = params[0];
        if (kind == "add" || kind == "sub" || kind == "mul" || kind == "slt"){
            attributes[0] = registerFile[0].get_regno(params[1]);
            attributes[1] = registerFile[0].get_regno(params[2]);
            attributes[2] = registerFile[0].get_regno(params[3]);
            return;
        }
        else if (kind == "addi"){
            attributes[0] = registerFile[0].get_regno(params[1]);
            attributes[1] = registerFile[0].get_regno(params[2]);
            attributes[2] = stoi(params[3]);
        }
        else if (kind == "beq" || kind == "bne"){
            attributes[0] = registerFile[0].get_regno(params[1]);
            attributes[1] = registerFile[0].get_regno(params[2]);
            label = params[3];
        }
        else if (kind ==  "j"){
            label = params[1];
        }
        else if (kind == "lw" || kind == "sw"){
            attributes[0] = registerFile[0].get_regno(params[1]);
            string add = params[2];
            size_t start = add.find_first_of("(");
            size_t end = add.find_first_of(")");
            int sz = end - start - 1;
            stringstream ss(add.substr(0, start));
            int offset;ss >> offset;
            attributes[1] = offset;
            string reg = add.substr(start + 1, sz);
            attributes[2] = registerFile[0].get_regno(reg);
        }
        else{
            throw exception();
        }
    }
};
vector<Instruction> instructionVector[N];

void simulate_lookahead();
void simulate_basic();

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
            Instruction currIst;
            currIst.setInstruction(test2);
            instructionVector[cor-1].push_back(currIst);
        }
        Instruction endIst;
        endIst.setInstruction("EOF");
        instructionVector[cor-1].push_back(endIst);
    }
    for (int core = 0; core < N; core++){
        for (int i = 0; i < instructionVector[core].size(); i++){
            memory.storeInstr(instructionVector[core][i].line, i);
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
void simulate_lookahead(){
    bool complete = false;
    bool over = false;
    while(!over){
        clock_cycle++;
        for (int core = 0; core < N; core++){
            vector<string> this_cycle_info;
            Instruction current = instructionVector[core][programCounter[core]];
        }
    }
}
void simulate_basic(){
    bool complete = false;
    bool over = false;
    while(!over){
        clock_cycle++;
        for (int core = 0; core < N; core++){
            vector<string> this_cycle_info;
            Instruction current = instructionVector[core][programCounter[core]];
        }
    }
}