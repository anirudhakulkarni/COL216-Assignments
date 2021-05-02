#include <bits/stdc++.h>
#include "RegisterFile.h"
#include "MemoryUnit.h"
#define fo(i, N) for (int i = 0 ; i < N; i++)

using namespace std;

vector<string> parseInstr(string instruction);
const int N = 8; // Number of cores
RegisterFile registerFile[N];
MemoryUnit memory;
int clock_cycle = -1;
int Partition = 1024 * 32;
vector<vector<string>> cycleinfoofalln[N];
vector<pair<int, string>> DRAM_cycle_info;
int row_access_delay = 10;
int col_access_delay = 2;
int programCounter[N] = {0};
int reg_hold_sw[N][32] = {0};
int reg_hold_lw[N][32] = {0};
deque<int> core_process_next;
struct Instruction {
    string kind; // add, sub, mul, addi, lw, sw, beq, bne, slt, j, label
    string label;
    int attributes[3] = {-1}; 
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
void processInstruction(int core, Instruction current);
queue<pair<int, Instruction>> reorder_function(vector<pair<int, Instruction>> &MRM_storage);
class DRAM
{
public:
    deque<pair<int, Instruction>> DramQueue;
    int row_wait;
    int col_wait;
    int row_in_rowbuff;
    int prev_row;
    DRAM(){
        row_wait = 0;
        col_wait = 0;
        row_in_rowbuff = -1;
        prev_row = -1;
        while (!DramQueue.empty()) DramQueue.pop_front();
    }

    void insertoDRAMqueue(int core, Instruction instruction){
        DramQueue.push_back(make_pair(core, instruction));
    }
    void insert_front_to_dram(int core, Instruction ins){
        deque<pair<int, Instruction>>::iterator it = DramQueue.begin();
        it++;
        it = DramQueue.insert(it, make_pair(core, ins));
    }
    bool busy_DRAM(){
        if (DramQueue.empty()){
            return false;
        }
        return true;
    }
    pair<bool, string> process_DRAM(){
        pair<int, Instruction> here = DramQueue.front();
        Instruction current_ins = here.second;
        int curr_core = here.first;
        int mem_add = (Partition * curr_core + current_ins.attributes[1]);
        if (row_wait = 0 && col_wait == 0){
            int row_to_access = mem_add / 1024;
            if (row_in_rowbuff == -1){
                row_in_rowbuff = row_to_access;
                row_wait = row_access_delay;
                col_wait = col_access_delay;
            }
            else if (row_to_access == row_in_rowbuff){
                row_wait = 0;
                col_wait = col_access_delay;
            }
            else{
                prev_row = row_in_rowbuff;
                row_in_rowbuff = row_to_access;
                row_wait = 2 * row_access_delay;
                col_wait = col_access_delay;
            }
        }
        if (row_wait > 10){
            row_wait--;
            return make_pair(false, "Writing Back Row " + to_string(prev_row) + " in Row Buffer");
        }
        else if (row_wait > 0){
            row_wait--;
            return make_pair(false, "Loading Row " + to_string(row_in_rowbuff) + " in Row Buffer");
        }
        else if (col_wait > 0){
            col_wait--;
            return make_pair(false, "Loading Column " + to_string(mem_add % 1024) + " in Row Buffer");
        }
        else{
            processInstruction(here.first, current_ins);
            return make_pair(true, "Core #" + to_string(here.first) + " : Execution of Instruction " + current_ins.line + " finished ");
            DramQueue.pop_front();
        }
    }
};
DRAM Dram;
class MRM
{
public:
    vector<pair<int, Instruction>> MRM_Storage;
    void insert_to_MRM(int core, Instruction instruction){
        if (!Dram.busy_DRAM()){
            Dram.insertoDRAMqueue(core, instruction);
            return;
        }
        int current_row = (Partition * core + MRM_Storage[0].second.attributes[1]) / 1024;
        if (current_row == Dram.row_in_rowbuff){
            Dram.insert_front_to_dram(core, instruction);
            return;
        }
        MRM_Storage.push_back(make_pair(core, instruction));
    }
    void issue_Request_to_Dram(int core){
        if (!Dram.busy_DRAM()){
            queue<pair<int, Instruction>> toInsert = reorder_function(MRM_Storage);
        }
    }
};
MRM Mem_Request_manager;
void simulate_lookahead();
void simulate_basic();
template <class T>
string to_string(T t, ios_base &(*f)(ios_base &));
bool chk_empty(string instruction);
string remove_extra_whitespaces(const string &input);
bool check_dep(Instruction current);
int getIndexofChar(string str, char c);
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
bool check_dep(Instruction current){
    
    //vector<string> parametersVec = parseInstr(currInstr);
    //string Rdest = parametersVec[1];              j, beq, bne, lw, sw, addi, add, mul, sub, slt, 
    if (current.kind == "beq" || current.kind == "bne"){
        if (reg_hold_lw[current.attributes[0]] != 0 || reg_hold_lw[current.attributes[1]] != 0){
            return false;
        }
        return true;
    }
    if (current.kind == "j") return true;
    if (reg_hold_lw[current.attributes[0]] != 0 || reg_hold_sw[current.attributes[0]] != 0){
        return false;
    }
    if (current.kind == "addi"){
        if (reg_hold_lw[current.attributes[1]] != 0){
            return false;
        }
        return true;
    }
    if (reg_hold_lw[current.attributes[1]] != 0 || reg_hold_lw[current.attributes[2]] != 0){
        return false;
    }
    return true;
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
    int cores_executed = 0;
    bool cores_execution_finished[N] = {false};
    while(cores_executed < N){
        clock_cycle++;
        bool DRAM_issued = false; //only one DRAM request issued in this cycle
        for (int core = 0; core < N; core++){
            vector<string> this_cycle_info;
            Instruction current = instructionVector[core][programCounter[core]];
            if (current.kind == "EOF"){
                if (cores_execution_finished[core] == false){
                    cores_execution_finished[core] = true;cores_executed++;
                }
                continue;
            }
            if (current.kind == "lw" || current.kind == "sw"){
                if (DRAM_issued) continue;
                current.attributes[1] = current.attributes[1] + registerFile[core].get_register_data(current.attributes[2]);
                Mem_Request_manager.insert_to_MRM(core, current);
                if (current.kind == "lw"){
                    reg_hold_lw[core][current.attributes[0]]++;
                }
                else{
                    reg_hold_sw[core][current.attributes[0]]++;
                }
                DRAM_issued = true;
                this_cycle_info.push_back("Core #" + to_string(core+1) + " : DRAM Request Issued for" + current.line);
                programCounter[core]++;
            }
            else if (current.kind == "j"){
                processInstruction(core, current);
            }
            else if (check_dep(current) == false){
                deque<int>::iterator it = find(core_process_next.begin(),core_process_next.end(),core);
                if(it != core_process_next.end()){
                    core_process_next.push_back(core);
                }
                continue;
            }
            else{
                processInstruction(core, current);
                this_cycle_info.push_back("Instruction executed for core " + to_string(core+1) + " : " + current.line);
            }
            cycleinfoofalln[core].push_back(this_cycle_info);
        }
        pair<int, string> state = Dram.process_DRAM();
        if (state.first == true){
            clock_cycle++; //TO avoid 2 register writes in same cycle
            DRAM_cycle_info.push_back(make_pair(clock_cycle, state.second));
        }
        else{
            DRAM_cycle_info.push_back(make_pair(clock_cycle, state.second));
        }
        if (!core_process_next.empty()) Mem_Request_manager.issue_Request_to_Dram(core_process_next.front());
    }
}
void processInstruction(int core, Instruction current)
{
        if (current.kind == "add")
        {
            int ans = registerFile[core].get_register_data(current.attributes[1]) +
                            registerFile[core].get_register_data(current.attributes[2]);
            registerFile[core].set_register_data(current.attributes[0], ans);
            programCounter[core]++;
        }
        if (current.kind == "sub")
        {
            int ans = registerFile[core].get_register_data(current.attributes[1]) -
                            registerFile[core].get_register_data(current.attributes[2]);
            registerFile[core].set_register_data(current.attributes[0], ans);
            programCounter[core]++;
        }
        if (current.kind == "mul")
        {
            int ans = registerFile[core].get_register_data(current.attributes[1]) *
                            registerFile[core].get_register_data(current.attributes[2]);
            registerFile[core].set_register_data(current.attributes[0], ans);
            programCounter[core]++;
        }
        else if (current.kind == "beq")
        {
            if (registerFile[core].get_register_data(current.attributes[0]) == registerFile[core].get_register_data(current.attributes[1]))
            {
                programCounter[core] = memory.getAddOfLabel(current.label, core);
            }
            else
            {
                programCounter[core]++;
            }
        }
        else if (current.kind == "bne")
        {
            if (registerFile[core].get_register_data(current.attributes[0]) != registerFile[core].get_register_data(current.attributes[1]))
            {
                programCounter[core] = memory.getAddOfLabel(current.label, core);
            }
            else
            {
                programCounter[core]++;
            }
        }
        else if (current.kind == "slt")
        {
            if (registerFile[core].get_register_data(current.attributes[1]) < registerFile[core].get_register_data(current.attributes[2]))
            {
                registerFile[core].set_register_data(current.attributes[0], 1);
            }
            else
            {
                registerFile[core].set_register_data(current.attributes[0], 0);
            }
            programCounter[core]++;
        }
        else if (current.kind == "j")
        {
            if (isdigit(current.label[0]))
            {
                programCounter[core] = stoi(current.label);
            }
            else
            {
                programCounter[core] = memory.getAddOfLabel(current.label, core);
            }
        }
        else if (current.kind == "lw")
        {
            registerFile[core].set_register_data(current.attributes[0], memory.getDataAdd(current.attributes[1], core));
        }
        else if (current.kind == "sw")
        {
            memory.setDataAdd(current.attributes[1],
                registerFile[core].get_register_data(current.attributes[0]), core);
        }
        else if (current.kind == "addi")
        {
            int ans = registerFile[core].get_register_data(current.attributes[1]) + current.attributes[2];
            registerFile[core].set_register_data(current.attributes[0], ans);
            programCounter[core]++;
        }
        else
        {
            cout << "INVALID INSTRUCTION DETECTED!! " << current.line << endl;
            throw exception();
        }
}