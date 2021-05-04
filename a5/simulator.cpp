#include <bits/stdc++.h>
#include "RegisterFile.h"
#include "MemoryUnit.h"
#define fo(i, N) for (int i = 0 ; i < N; i++)

using namespace std;

vector<string> parseInstr(string instruction);
const int N = 3; // Number of cores
//RegisterFile registerFile[N];
RegisterFile registerFile;
MemoryUnit memory;
int clock_cycle = 0;
int Partition = 1024 * 32;
vector<vector<string>> cycleinfoofalln[N];
vector<pair<int, string>> DRAM_cycle_info;
void print_clock_cycleinfo(vector<vector<string>> cycleinfoofalln[N], vector<pair<int, string>> &DRAM_cycle_info);
int row_access_delay = 10;
int col_access_delay = 2;
int programCounter[N] = {0};
int reg_hold_sw[N][32] = {0};
int reg_hold_lw[N][32] = {0};
vector<int> core_process_next;
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
            kind = "label";
            label = params[0];
            return;
        }
        kind = params[0];
        if (kind == "add" || kind == "sub" || kind == "mul" || kind == "slt"){
            attributes[0] = registerFile.get_regno(params[1]);
            attributes[1] = registerFile.get_regno(params[2]);
            attributes[2] = registerFile.get_regno(params[3]);
            return;
        }
        else if (kind == "addi"){
            attributes[0] = registerFile.get_regno(params[1]);
            attributes[1] = registerFile.get_regno(params[2]);
            attributes[2] = stoi(params[3]);
        }
        else if (kind == "beq" || kind == "bne"){
            attributes[0] = registerFile.get_regno(params[1]);
            attributes[1] = registerFile.get_regno(params[2]);
            label = params[3];
        }
        else if (kind ==  "j"){
            label = params[1];
        }
        else if (kind == "lw" || kind == "sw"){
            attributes[0] = registerFile.get_regno(params[1]);
            string add = params[2];
            size_t start = add.find_first_of("(");
            size_t end = add.find_first_of(")");
            int sz = end - start - 1;
            stringstream ss(add.substr(0, start));
            int offset;ss >> offset;
            attributes[1] = offset;
            string reg = add.substr(start + 1, sz);
            attributes[2] = registerFile.get_regno(reg);
        }
        else{
            throw exception();
        }
    }
};
vector<Instruction> instructionVector[N];
void processInstruction(int core, Instruction current);
queue<pair<int, Instruction>> reorder_function(int core, vector<pair<int, Instruction>> &MRM_storage);
class DRAM
{
public:
    vector<pair<int, Instruction>> DramQueue;
    int row_wait = 0;
    int col_wait = 0;
    int row_in_rowbuff = -1;
    int prev_row  = -1;
    DRAM(){
        row_wait = 0;
        col_wait = 0;
        row_in_rowbuff = -1;
        prev_row = -1;
    }

    void insertoDRAMqueue(int core, Instruction instruction){
        DramQueue.push_back(make_pair(core, instruction));
    }
    void insert_front_to_dram(int core, Instruction ins){
        vector<pair<int, Instruction>>::iterator it = DramQueue.begin();
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
        if (DramQueue.empty()){
            return make_pair(false, "");
        }
        pair<int, Instruction> here = DramQueue[0];
        Instruction current_ins = here.second;
        int curr_core = here.first;
        int mem_add = (Partition * curr_core + current_ins.attributes[1]);
        if (row_wait == 0 && col_wait == 0){
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
        if (row_wait > row_access_delay){
            row_wait--;
            return make_pair(false, "Writing Back Row " + to_string(prev_row) + " in Row Buffer");
        }
        else if (row_wait > 0){
            row_wait--;
            return make_pair(false, "Loading Row " + to_string(row_in_rowbuff) + " in Row Buffer");
        }
        else if (col_wait > 0){
            col_wait--;
            if (col_wait == 0 && row_wait == 0){
                processInstruction(here.first, current_ins);
                vector<pair<int, Instruction>> :: iterator it = DramQueue.begin();
                DramQueue.erase(it);
                return make_pair(true, to_string(here.first + 1) + ":Execution of Instruction " + current_ins.line + " finished." + "Loading Column " + to_string(mem_add % 1024) + " in Row Buffer\n");
            }
            return make_pair(false, "Loading Column " + to_string(mem_add % 1024) + " in Row Buffer");
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
        int current_row = (Partition * core + instruction.attributes[1]) / 1024;
        if (current_row == Dram.row_in_rowbuff){
            Dram.insert_front_to_dram(core, instruction);
            return;
        }
        MRM_Storage.push_back(make_pair(core, instruction));
    }
    void issue_Request_to_Dram(int core){
        if (!Dram.busy_DRAM()){
            queue<pair<int, Instruction>> toInsert = reorder_function(core, MRM_Storage);
            while(!toInsert.empty()){
                Dram.insertoDRAMqueue(toInsert.front().first, toInsert.front().second);
                toInsert.pop();
            }
        }
    }
    void assert_core_executed(){
        bool to_remove = true;
        if (core_process_next.size() == 0) return;
        for (int i = 0; i < Dram.DramQueue.size(); i++) {
            if (Dram.DramQueue[i].first == core_process_next[0]){
                to_remove = false;
            }
        }
        for (auto x : MRM_Storage){
            if (x.first == core_process_next[0]){
                to_remove = false;
            }
        }
        if (to_remove == true){
            vector<int> :: iterator it = core_process_next.begin();
            core_process_next.erase(it);
        }
    }
};
MRM Mem_Request_manager;
void simulate_lookahead();
void simulate_basic();
int GetIndexofChar(string str, char c);
template <class T>
string to_string(T t, ios_base &(*f)(ios_base &));
bool chk_empty(string instruction);
string remove_extra_whitespaces(const string &input);
bool check_dep(int core, Instruction current);
int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);                
    cout.tie(NULL);
    cout << endl;
    for(int cor = 1; cor <= N; cor++){
        cout << endl << "Core #" << cor << endl;
        ifstream infile;
        string test2;
        string file = "./Inputs/t" + to_string(cor) + ".txt"; 
        infile.open(file.c_str(), ios::in);
        while (getline(infile, test2))
        {
            if (chk_empty(test2) == true){
                continue;
            }
            cout << test2 << endl;
            Instruction currIst;
            currIst.setInstruction(test2);
            instructionVector[cor-1].push_back(currIst);
        }
        Instruction endIst;
        endIst.setInstruction("EOF");
        cout << "EOF" << endl;
        instructionVector[cor-1].push_back(endIst);
    }
    cout << endl << "Instructions read successfully" << endl << endl;
    for (int core = 0; core < N; core++){
        for (int i = 0; i < instructionVector[core].size(); i++){
            memory.storeInstr(instructionVector[core][i].line, core);
        }
    }
    cout << "Instructions stored successfully" << endl << endl;
    Dram.DramQueue.clear();
    Mem_Request_manager.MRM_Storage.clear();
    simulate_basic();
    print_clock_cycleinfo(cycleinfoofalln, DRAM_cycle_info);
    cout << "Program Execution successful" << endl << endl;
    registerFile.printRegisters();
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
bool check_dep(int core, Instruction current){
    
    //vector<string> parametersVec = parseInstr(currInstr);
    //string Rdest = parametersVec[1];              j, beq, bne, lw, sw, addi, add, mul, sub, slt, 
    if (current.kind == "beq" || current.kind == "bne"){
        
        if (reg_hold_lw[core][current.attributes[0]] != 0 || reg_hold_lw[core][current.attributes[1]] != 0){
            return false;
        }
        return true;
    }
    if (current.kind == "j") return true;
    if (reg_hold_lw[core][current.attributes[0]] != 0 || reg_hold_sw[core][current.attributes[0]] != 0){
        return false;
    }
    if (current.kind == "addi"){
        if (reg_hold_lw[core][current.attributes[1]] != 0){
            return false;
        }
        return true;
    }
    if (reg_hold_lw[core][current.attributes[1]] != 0 || reg_hold_lw[core][current.attributes[2]] != 0){
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
int GetIndexofChar(string str, char c){
    int a = 0;
    while (str[a] != c){
        a++;
    }
    return a;
}
void insert_empty(int chk){
    if (chk != -1){
        vector<string> v;
        cycleinfoofalln[chk].push_back(v);
        return;
    }
    for (int i = 0; i < N; i++){
        vector<string> v;
        cycleinfoofalln[i].push_back(v);
    }
}
void simulate_basic(){
    bool complete = false;
    int cores_executed = 0;
    bool cores_execution_finished[N] = {false};
    while(cores_executed < N){
        bool alu_this_cycle[N] = {false};
        //if (clock_cycle > 50) break;
        clock_cycle++;
        bool DRAM_issued = false; //only one DRAM request issued in this cycle
        for (int core = 0; core < N; core++){
            vector<string> this_cycle_info;
            string curr = memory.getCurrInstr(programCounter[core], core);
            Instruction current;
            current.setInstruction(curr);
            //Instruction current = instructionVector[core][programCounter[core]];
            if (current.kind == "EOF"){
                if (cores_execution_finished[core] == false){
                    cores_execution_finished[core] = true;cores_executed++;
                }
                continue;
            }
            if (current.kind == "lw" || current.kind == "sw"){
                if (DRAM_issued) {
                    insert_empty(core);
                    continue;
                }
                current.attributes[1] = current.attributes[1] + registerFile.get_register_data(core, current.attributes[2]);
                Mem_Request_manager.insert_to_MRM(core, current);
                if (current.kind == "lw"){
                    reg_hold_lw[core][current.attributes[0]]++;
                }
                else{
                    reg_hold_sw[core][current.attributes[0]]++;
                }
                DRAM_issued = true;
                //cout << clock_cycle << "dram issue " << current.line << endl;
                this_cycle_info.push_back("DRAM Request Issued for " + current.line);
                programCounter[core]++;
            }
            else if (current.kind == "j"){
                processInstruction(core, current);
                this_cycle_info.push_back("Instruction executed : " + current.line);
            }
            else if (check_dep(core, current) == false){
                //cout << clock_cycle << " dependednt " << current.line << endl;
                vector<int>::iterator it = find(core_process_next.begin(),core_process_next.end(),core);
                if(it == core_process_next.end()){
                    //cout << core_process_next.size() << "PUSHED" << core << endl;
                    core_process_next.push_back(core);
                }
            }
            else{
                processInstruction(core, current);
                alu_this_cycle[core] = true;
                //cout << clock_cycle << " ind - dependednt " << current.line << endl;
                this_cycle_info.push_back("Instruction executed : " + current.line);
            }
            cycleinfoofalln[core].push_back(this_cycle_info);
        }
        pair<int, string> state = Dram.process_DRAM();
        if (state.second == ""){
            continue;
        }
        if (state.first == true){
            if (alu_this_cycle[stoi(state.second.substr(0, GetIndexofChar(state.second, '.')))] == true){
                clock_cycle++; //TO avoid 2 register writes in same cycle
                insert_empty(-1);
                //cout << "clock --OO " << clock_cycle << state.second << endl;
                DRAM_cycle_info.push_back(make_pair(clock_cycle, state.second));
            }else{
                //cout << "clock -- " << clock_cycle << state.second << endl;
                DRAM_cycle_info.push_back(make_pair(clock_cycle, state.second));
            }
        }
        else{
            //cout << "clock -- " << clock_cycle << state.second << endl;
            DRAM_cycle_info.push_back(make_pair(clock_cycle, state.second));
        }
        Mem_Request_manager.assert_core_executed();
        if (!core_process_next.empty()) Mem_Request_manager.issue_Request_to_Dram(core_process_next[0]);
    }
}
void processInstruction(int core, Instruction current)
{
        if (current.kind == "add")
        {
            int ans = registerFile.get_register_data(core, current.attributes[1]) +
                            registerFile.get_register_data(core, current.attributes[2]);
            registerFile.set_register_data(core, current.attributes[0], ans);
            programCounter[core]++;
        }
        else if (current.kind == "sub")
        {
            int ans = registerFile.get_register_data(core, current.attributes[1]) -
                            registerFile.get_register_data(core, current.attributes[2]);
            registerFile.set_register_data(core, current.attributes[0], ans);
            programCounter[core]++;
        }
        else if (current.kind == "mul")
        {
            int ans = registerFile.get_register_data(core, current.attributes[1]) *
                            registerFile.get_register_data(core, current.attributes[2]);
            registerFile.set_register_data(core, current.attributes[0], ans);
            programCounter[core]++;
        }
        else if (current.kind == "beq")
        {
            if (registerFile.get_register_data(core, current.attributes[0]) == registerFile.get_register_data(core, current.attributes[1]))
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
            if (registerFile.get_register_data(core, current.attributes[0]) != registerFile.get_register_data(core, current.attributes[1]))
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
            if (registerFile.get_register_data(core, current.attributes[1]) < registerFile.get_register_data(core, current.attributes[2]))
            {
                registerFile.set_register_data(core, current.attributes[0], 1);
            }
            else
            {
                registerFile.set_register_data(core, current.attributes[0], 0);
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
            registerFile.set_register_data(core, current.attributes[0], memory.getDataAdd(current.attributes[1], core));
            reg_hold_lw[core][current.attributes[0]]--;
        }
        else if (current.kind == "sw")
        {
            memory.setDataAdd(current.attributes[1],
                registerFile.get_register_data(core, current.attributes[0]), core);
            reg_hold_sw[core][current.attributes[0]]--;
        }
        else if (current.kind == "addi")
        {
            int ans = registerFile.get_register_data(core, current.attributes[1]) + current.attributes[2];
            registerFile.set_register_data(core, current.attributes[0], ans);
            programCounter[core]++;
        }
        else
        {
            cout << "INVALID INSTRUCTION DETECTED!! " << current.line << endl;
            throw exception();
        }
}
queue<pair<int, Instruction>> reorder_function(int core, vector<pair<int, Instruction>> &MRM_storage){
    queue<pair<int, Instruction>> output;
    vector<pair<int, Instruction>> temp;
    for (auto x : MRM_storage){
        if (x.first == core){
            output.push(x);
        }
        else{
            temp.push_back(x);
        }
    }
    MRM_storage = temp;
    return output;
}
void print_clock_cycleinfo(vector<vector<string>> cycleinfoofalln[N], vector<pair<int, string>> &DRAM_cycle_info){
    int clock = 0;
    while(clock < clock_cycle - 1){
        cout << "Clock cycle " << clock + 1 << endl;
        int idot = -1;
        int icol = -1;
        int curr_core = -1;
        string extra = "";
        string core_info = "";
        if (DRAM_cycle_info.size() > 0){
            if (DRAM_cycle_info[0].first - 1 == clock){
                string out = DRAM_cycle_info[0].second;
                if (out.find(":") != string::npos){
                    idot = GetIndexofChar(out, '.');
                    icol = GetIndexofChar(out, ':');
                    curr_core = stoi(out.substr(0, icol));
                    core_info = out.substr(icol+1, idot - icol-1);
                    extra = out.substr(idot+1, out.size() - idot-1);
                }
                else{
                    extra = out;
                }
                vector<pair<int, string>> :: iterator it = DRAM_cycle_info.begin();
                DRAM_cycle_info.erase(it);
            }
        }
        for (int i = 0; i < N; i++){
            cout << "Core #" << to_string(i+1) <<":" << endl;
            if (cycleinfoofalln[i].size() > clock){
                for (auto x : cycleinfoofalln[i][clock]){
                    cout << x << endl;
                }
            }
            if ( i == curr_core-1){
                cout << core_info << endl;
            }
        }
        if (extra != "") cout << extra << endl;
        clock++;
        cout << endl;
    }
}