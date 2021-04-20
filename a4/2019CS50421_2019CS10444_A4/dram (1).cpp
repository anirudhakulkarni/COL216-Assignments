#include "stdio.h"
#include "iostream"
#include "fstream"
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;
int row_access_delay = 10;
int col_access_delay = 2;
int partition = 1048576 / 2;
int Clock_cycle = 1;
queue<string> glob;
int rg_hold_lw[32] = {0};
int rg_hold_sw[32] = {0};
map<int, string> clock_instr;
void lineshow(){
    cout << "==============================================================" << endl;
}
void show(){
    for(int it = 0; it < 32; it++){
        cout << rg_hold_sw[it] << " ";
        cout << rg_hold_lw[it] << " ";
    }
    cout << endl;
}
template <class T>
string to_string(T t, ios_base &(*f)(ios_base &)) // DONT change its name. will not work.
{
    ostringstream oss;
    oss << f << t;
    return oss.str();
}
int getIndexofChar(string str, char c){
    int a = 0;
    while (str[a] != c){
        a++;
    }
    return a;
}
int count_wht(string s){
    int cnt = 0;
    for(int it=0; it<s.length(); it++){
        s[it] == ' ' ? cnt+=1:cnt+=0;
    }
    return cnt;
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
vector<string> getInstrucVec(fstream &infile)
{
    vector<string> temp;
    string test2;
    while (getline(infile, test2))
    {
        temp.push_back(test2);
    }
    return temp;
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
    string regNameArray[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

public:    
    int get_regno(string reg)
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
        // cout << "=" << regno << endl;
        if (regno == -1)
        {
            cout << endl;
            cout << "INVALID REGISTER DETECTED!! : " << reg << endl;
            throw exception();
        }
        return regno;
    }

    RegisterFile()
    {
        for (int it = 0; it < 32; it++)
            regArray[it] = 0;
    }
    int get_register_data(string register_name)
    {
        // if (register_name[0] == '$')
        return regArray[get_regno(register_name)];
        // else
        //     return stoi(register_name);
    }
    void set_register_data(string register_name, int data)
    {
        if (register_name == "$zero")
        {
            cout << endl;
            cout << "$zero can not be set  " << endl;
            throw exception();
        }
        // cout << "register number: " << register_name << " " << data << " " << get_regno(register_name) << endl;
        regArray[get_regno(register_name)] = data;
    }
    void printRegisters()
    {
        cout << "Contents of Register file:\n";
        for (int i = 0; i < 31; i++)
        {
            cout << regNameArray[i] << ": " << to_string<long>(regArray[i], hex) << ", ";
            //cout << "$" << i  << ": " << to_string<long>(regArray[i], hex) << " ";
        }
        cout << regNameArray[31] << ": " << to_string<long>(regArray[31], hex) << endl;
        //cout << "$" << 31  << ": " << to_string<long>(regArray[31], hex) << endl;
    }
};
class MemoryUnit
{
public:
    string *MemArray = new string[1048576];
    // string MemArray[100];
    int currInstrId = 0;
    int currVarId = 0;
    int partition = 1048576 / 2;
    map<string, int> addofLabels;
    map<string, int> addofVars;
    //map<int, int> valofVars;
    MemoryUnit()
    {
        for (int it = 0; it < 1048576; it++)
            MemArray[it] = "";
        currInstrId = 0;
        currVarId = 1048576 / 2;
        partition = 1048576 / 2;
    }
    void storeInstr(string instruction)
    {
        
        if (instruction.find(":") != string::npos)
        {
            //cout << "instr -> " << "XX" << (instruction.substr(0, getIndexofChar(instruction, ':'))) << "XX" << " id-> " << currInstrId << endl;
            addofLabels.insert(pair<string, int>(instruction.substr(0, getIndexofChar(instruction, ':')), currInstrId));
        }
        else
        {
            MemArray[currInstrId] = instruction;

            currInstrId+=1;
        }
    }
    int getAddOfLabel(string label)
    {
        if (addofLabels.find(label) == addofLabels.end())
        {
            cout << endl;
            cout << "INVALID Branch Detected!! : " << label << endl;
            throw exception();
        }
        return addofLabels.at(label);
    }
    string getCurrInstr(int current)
    {
        return MemArray[current];
    }
    void setData(string variable, int value)                //Modified
    {
        if (addofVars.find(variable) == addofVars.end())
        {
            addofVars[variable] = currVarId;
            currVarId+=4;
        }
        //valofVars[addofVars[variable]] = value;
        MemArray[addofVars[variable]] = to_string(value);
    }
    int get_var_add(string variable){
        return addofVars[variable];
    }
    int getData(string variable)
    {
        if (addofVars.find(variable) == addofVars.end())
            return -1;
        
        //return ValofVars[addofVars[variable]];
        stringstream ss(MemArray[addofVars[variable]]);
        int val; ss >> val;
        return val;
    }
    void setDataAdd(int address, int value)
    {
        MemArray[address + partition] = to_string(value);
    }
    int getDataAdd(int address)
    {
        if (MemArray[address + partition] == ""){
            return 0;
        }
        stringstream ss(MemArray[address + partition]);
        int val; ss >> val;
        return val;
    }
    void printMemDataContent()
    {
        cout << "Memory content at the end of the execution:" << endl;
        for (int y = partition; y < 2 * partition; y+=4)
        {
            if (MemArray[y] == "") continue;
            stringstream ss(MemArray[y]); int val; ss >> val; 
        cout << y - partition << '-' << y+3 - partition <<":" << ' ' << to_string<long>(val, hex) << endl;
        } 
    }
};
class RowBuffer
{
public:
    int row_no; // current row
    int back_no; //write back
    int col_no;
    string message;
    map<int, int> M;
    RowBuffer(){
        row_no = -1;
        col_no = -1;
        back_no = -1;
        message = "";
    }
    void UpdateRowBuffer(int row,int col, int br){
        if (row != row_no) {
            M[row] += 1;
        }
        row_no = row;
        col_no = col;
        back_no = br;
    }
    void print_buffers(){
        for (const auto &p : M){
            cout << "Row with starting address " << p.first * 1024 << " was loaded " << p.second << " times." << endl; 
        } 
    }
};
void print_req() {
    //cout << "==============================================================" << endl;
    //cout << "Clock cycle executed: " << Clock_cycle << endl;
    cout << "DRAM Request Issued" << endl; 
}
void print_rl(int r){
    cout << "Loading Row " << r << " in Row Buffer." << endl;
}
void print_wl(int r){
    cout << "Writing Back Row " << r << " in DRAM from Row Buffer." << endl;
}
void print_cl(int c){
    cout << "Accessing Column " << c << " in Row Buffer." << endl;
}
pair<int, string> getMemAdd(string add, RegisterFile &registerFile)
{

    size_t start = add.find_first_of("(");
    size_t end = add.find_first_of(")");
    int sz = end - start - 1;
    string reg = add.substr(start + 1, sz);
    stringstream ss(add.substr(0, start));
    int off;
    ss >> off;
    int add_memory = registerFile.get_register_data(reg);
    int final_add = off + add_memory;
    return make_pair(final_add, reg);
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
bool chk_empty(string instruction){
    for(int i = 0; i < instruction.length(); i++){
        if ((instruction[i]!=' ') && (instruction[i] != '\t') && (instruction[i] != '\n')) {
            return false;
        }
    }
    return true;
}
void msg_lw(string reg, RowBuffer &rowbuff, RegisterFile &regis){
    int address = rowbuff.row_no * 1024 + rowbuff.col_no;
    string s = "Loaded value from Memory address " + to_string(address) + "-" + to_string(address+3) + " in the register " + reg + "." + "\nValue in Register " + reg + " is: " + to_string(regis.get_register_data(reg));
    cout << s << endl;
}
void msg_sw(string reg, RowBuffer &rowbuff, MemoryUnit &mem){
    int address = rowbuff.row_no * 1024 + rowbuff.col_no;
    string s = "Saved value from Register " + reg + " in the Memory address " + to_string(address) + "-" + to_string(address+3) + "." +"\nValue at Memory address " + to_string(address) + "-" + to_string(address+3) + " is: " + to_string(mem.getDataAdd(address));
    cout << s << endl;
}
void print_currInstr(string s){

    cout << "Current Instructiion being executed: " << s << endl;
}
void print_clk(){
    cout << "==============================================================" << endl;
        //cout << "Current Instruction : " << currentInstr << endl;
    cout << "Clock cycle executed: " << Clock_cycle << endl;
    Clock_cycle++;
}
int get_address(string mem, RegisterFile registerFile, MemoryUnit memory){
    if (mem.find("(") == string::npos || mem.find(")") == string::npos)
    {
        if (isdigit(mem[0]) == true){
            stringstream ss(mem);
            int val; ss >> val;
            return val;
        }
        else return memory.get_var_add(mem);
    }
    else return getMemAdd(mem, registerFile).first;
}
void helper(int x, int r[], int m[], queue<string> &ans, vector<bool> &b, vector<string> vec, RegisterFile registerFile, MemoryUnit memory){
    for(int i = 0; i< vec.size(); i++){
        if (((get_address (parseInstr(vec[i])[2], registerFile, memory)) / 1024 == x) && (b[i] == false)){
            if (parseInstr(vec[i])[0] == "sw"){
                if (r[registerFile.get_regno(parseInstr(vec[i])[1])] == 0){
                    ans.push(vec[i]);
                    b[i] = true;
                }
            }
            else{
                if (m[get_address(parseInstr(vec[i])[2], registerFile, memory)] == 0){
                    ans.push(vec[i]);
                    b[i] = true;
                }
            }
        }
    }
}
queue<string> reorder_instructions(int curr, queue<string> &q, RegisterFile registerFile, MemoryUnit memory){
    int register_hold[32] = {0};
    int memory_hold[1048576 / 2] = {0};
    vector<string> main;
    vector<bool> executed;
    map<int, queue<string>> spock;
    //vector<string> vec;
        queue<string> ans;
    if(q.empty()){
        return ans;
    }
    vector<string> vs = parseInstr(q.front());
    curr = stoi(vs[2])/ 1024;
    while (!q.empty()){
        string instr = q.front();
        vector<string> parametersVec = parseInstr(instr);
        if (parametersVec[0] == "sw"){
            if (register_hold[registerFile.get_regno(parametersVec[1])] == 0){
                spock[get_address(parametersVec[2], registerFile, memory) / 1024].push(instr);
            }
            else {
                main.push_back(instr);
                executed.push_back(false);
            }
            memory_hold[get_address(parametersVec[2], registerFile, memory)]++;  
        }
        else{
            if (memory_hold[get_address(parametersVec[2], registerFile, memory)] == 0){
                spock[get_address(parametersVec[2], registerFile, memory) / 1024].push(instr);
            }
            else {
                main.push_back(instr);
                executed.push_back(false);
            }
            register_hold[registerFile.get_regno(parametersVec[1])]++;
        }
        q.pop();
    }
    if (spock.find(curr) != spock.end()){
        while(!spock[curr].empty()){
            string ins = spock[curr].front();
            if (parseInstr(ins)[0] == "lw"){
                register_hold[registerFile.get_regno(parseInstr(ins)[1])]--;
            }else{
                memory_hold[get_address(parseInstr(ins)[2], registerFile, memory)]--;
            }
            ans.push(ins);
            spock[curr].pop();
        }
        helper(curr, register_hold, memory_hold, ans, executed, main, registerFile, memory);
    }
    for (auto it = spock.begin(); it != spock.end(); it++){
        while(!it->second.empty()){
            if (parseInstr(it->second.front())[0] == "lw"){
                register_hold[registerFile.get_regno(parseInstr(it->second.front())[1])]--;
            }else{
                memory_hold[get_address(parseInstr(it->second.front())[2], registerFile, memory)]--;
            }
            ans.push(it->second.front());
            it->second.pop();
        }
        helper(it->first, register_hold, memory_hold, ans, executed, main, registerFile, memory);
    }
    int cf = 0;
    for(bool b : executed){
        (b == true) ? cf++ : cf+=0;
    }
    while (cf < main.size()){
    for (int it = 0; it < main.size(); it++){
        if (executed[it] == false){
            if (parseInstr(main[it])[0] == "lw"){
                if (memory_hold[get_address(parseInstr(main[it])[2], registerFile, memory)] == 0){
                    ans.push(main[it]);
                    register_hold[registerFile.get_regno(parseInstr(main[it])[1])]--;
                    executed[it] = true;
                    cf++;
                }
            }
            else{
                if (register_hold[registerFile.get_regno(parseInstr(main[it])[1])] == 0){
                    ans.push(main[it]);
                    memory_hold[get_address(parseInstr(main[it])[2], registerFile, memory)]--;
                    executed[it] = true;
                    cf++;
                }
            }
        }
    }
    }
    return ans;
}
void complete_dram_activity(RowBuffer &rowbuff, queue<string> &q, RegisterFile &reg_fl, MemoryUnit &memory){
    queue<string> re = reorder_instructions(rowbuff.row_no, glob, reg_fl, memory);
    while (!re.empty()){
        cout << "==============================================================" << endl;
        vector<string> parametersVec = parseInstr(re.front());
        if (parametersVec[0] == "lw")
        {
            string Rdest = parametersVec[1], mem = parametersVec[2];
            int add = stoi(mem);
            //cout << "Current address is : " << add << endl;
            reg_fl.set_register_data(Rdest, memory.getDataAdd(add));
            //cout << "HELLO" << add << "KAK" << reg_fl.get_register_data(Rdest) << "KFL" << endl;
            if (add / 1024 != rowbuff.row_no){
            if (rowbuff.row_no == -1){
                rowbuff.UpdateRowBuffer(add / 1024, add %1024, -1 );
                //rowbuff.UpdateDelays(row_access_delay, col_access_delay);
                cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + row_access_delay-1 << endl;
                print_rl(add / 1024);
                Clock_cycle += row_access_delay;
                lineshow();
                cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + col_access_delay-1 << endl;
                print_cl(add % 1024);
                Clock_cycle += col_access_delay;
            }
            else{
                //cout << "Hello3"<< endl;
                int curr = rowbuff.row_no;
                
                //rowbuff.UpdateDelays(2 * row_access_delay, col_access_delay);
                cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + row_access_delay-1 << endl;
                print_wl(rowbuff.row_no);
                Clock_cycle += row_access_delay;lineshow();
                cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + row_access_delay-1 << endl;
                print_rl(add / 1024);
                Clock_cycle += row_access_delay;lineshow();
                cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + col_access_delay-1 << endl;
                print_cl(add % 1024);
                Clock_cycle += col_access_delay;
                rowbuff.UpdateRowBuffer(add / 1024, add %1024, curr );
                }
            }
            else{
                //COLUMN
                rowbuff.UpdateRowBuffer(rowbuff.row_no, add %1024, -1 );
                //rowbuff.UpdateDelays(0, col_access_delay);lineshow();
                cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + col_access_delay-1 << endl;
                print_cl(add % 1024);
                Clock_cycle += col_access_delay;
            }
            msg_lw(Rdest, rowbuff, reg_fl);
            rg_hold_lw[reg_fl.get_regno(Rdest)]--;
        }
        else if (parametersVec[0] == "sw")
        {
            string Rdest = parametersVec[1], mem = parametersVec[2];
            int add = stoi(mem);
            //cout << "Current address is : " << add << endl;
                    if (add / 1024 != rowbuff.row_no){
                        if (rowbuff.row_no == -1){
                            rowbuff.UpdateRowBuffer(add / 1024, add %1024, -1 );
                            //rowbuff.UpdateDelays(row_access_delay, col_access_delay);
                            cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + row_access_delay-1 << endl;
                            print_rl(add / 1024);
                            Clock_cycle += row_access_delay;lineshow();
                            cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + col_access_delay-1 << endl;
                            print_cl(add % 1024);
                            Clock_cycle += col_access_delay;
                        }
                        else{
                            int curr = rowbuff.row_no;
                            
                            //rowbuff.UpdateDelays(2 * row_access_delay, col_access_delay);
                            cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + row_access_delay-1 << endl;
                            print_wl(rowbuff.row_no);
                            Clock_cycle += row_access_delay;lineshow();
                            cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + row_access_delay-1 << endl;
                            print_rl(add / 1024);
                            Clock_cycle += row_access_delay;lineshow();
                            cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + col_access_delay-1 << endl;
                            print_cl(add % 1024);
                            Clock_cycle += col_access_delay;
                            rowbuff.UpdateRowBuffer(add / 1024, add %1024, curr );
                        }
                    }
                    else{
                        //COLUMN
                        //print_req();
                        rowbuff.UpdateRowBuffer(rowbuff.row_no, add %1024, -1 );
                        //rowbuff.UpdateDelays(0, col_access_delay);
                        cout << "Clock cycles executed: " << Clock_cycle << "-" << Clock_cycle + col_access_delay-1 << endl;
                        print_cl(add % 1024);
                        Clock_cycle += col_access_delay;
                    }
            memory.setDataAdd(add, reg_fl.get_register_data(Rdest));
            //cout << "HELLO" << add << "KAK" << reg_fl.get_register_data(Rdest) << "KFL" << endl;
            msg_sw(Rdest, rowbuff, memory);
            rg_hold_sw[reg_fl.get_regno(Rdest)]--;
        }
        re.pop();
    }
}
bool check_dep(RegisterFile &registerFile, string currInstr){
    
    vector<string> parametersVec = parseInstr(currInstr);
    string Rdest = parametersVec[1];
    if (rg_hold_lw[registerFile.get_regno(Rdest)] != 0 || rg_hold_sw[registerFile.get_regno(Rdest)] != 0){
        return false;
    }
    if (parametersVec[0] == "beq" || parametersVec[0] == "bne"){
        string r1 = parametersVec[1];
        string r2 = parametersVec[2];
        if (rg_hold_lw[registerFile.get_regno(r1)] != 0 || rg_hold_lw[registerFile.get_regno(r2)] != 0){
            return false;
        }
        return true;
    }
    for (int i = 2; i < parametersVec.size(); i++){
        if (rg_hold_lw[registerFile.get_regno(parametersVec[i])] != 0){
            return false;
        }
    }
    return true;
}
void processInstructions(vector<string> instructionVector, RegisterFile &registerFile, MemoryUnit &memory, RowBuffer &rowbuff)
{
    int executionOfInstructionCount[999] = {0};
    int programCounter = 0;
    for (int i = 0; i < instructionVector.size(); i++)
    {
        string temp = instructionVector[i];
        memory.storeInstr(temp);
    }
    string currentInstr = memory.getCurrInstr(0);
    while (currentInstr != "EOF")
    {
        //if (Clock_cycle > 200)  break;
        //cout << "hello233" << endl;
        vector<string> parametersVec = parseInstr(currentInstr);
        if (parametersVec[0] == "add")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2], Src = parametersVec[3];
            //Check if Independent
            if (check_dep(registerFile, currentInstr)){
                print_clk();
                print_currInstr(currentInstr);
                executionOfInstructionCount[programCounter]++;
                int ans = registerFile.get_register_data(Rsrc) + registerFile.get_register_data(Src);
                registerFile.set_register_data(Rdest, ans);
                cout << "Updated value of " << Rdest << " to "<< ans << '.' << endl;
                programCounter++;
                
            }
            else{
                    complete_dram_activity(rowbuff,  glob, registerFile, memory);
                    //rowbuff.UpdateHault(true);
                }
        }
        else if (parametersVec[0] == "sub")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2], Src = parametersVec[3];
            //Check if Independent
            if (check_dep(registerFile, currentInstr)){
                print_clk();
                print_currInstr(currentInstr);
                executionOfInstructionCount[programCounter]++;
                int ans = registerFile.get_register_data(Rsrc) - registerFile.get_register_data(Src);
                registerFile.set_register_data(Rdest, ans);
                cout << "Updated value of " << Rdest << " to "<< ans << '.' << endl;
                programCounter++;
                
            }
            else{
                    complete_dram_activity(rowbuff,  glob, registerFile, memory);
                    //rowbuff.UpdateHault(true); //complete dram activity
            }
        }
        else if (parametersVec[0] == "mul")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2], Src = parametersVec[3];
            //Check if Independent
            if (check_dep(registerFile, currentInstr)){
                print_clk();
                print_currInstr(currentInstr);
                executionOfInstructionCount[programCounter]++;
                int ans = registerFile.get_register_data(Rsrc) * registerFile.get_register_data(Src);
                registerFile.set_register_data(Rdest, ans);
                cout << "Updated value of " << Rdest << " to "<< ans << '.' << endl;
                programCounter++;
                
            }
            else{
                complete_dram_activity(rowbuff,  glob, registerFile, memory);
                //rowbuff.UpdateHault(true);
            }
        }
        else if (parametersVec[0] == "beq")
        {
            string Rsrc1 = parametersVec[1], Src2 = parametersVec[2], label = parametersVec[3];
            //Check if Independent
            if (check_dep(registerFile, currentInstr)){
                print_clk();
                print_currInstr(currentInstr);
                executionOfInstructionCount[programCounter]++;
                if (registerFile.get_register_data(Rsrc1) == registerFile.get_register_data(Src2))
                {  
                    programCounter = memory.getAddOfLabel(label);
                    cout << "Jumping to label: " << label << endl;
                }
                else {programCounter++;}
                
            }
            else{
                complete_dram_activity(rowbuff,  glob, registerFile, memory);
                //rowbuff.UpdateHault(true);
            }
        }
        else if (parametersVec[0] == "bne")
        {
            string Rsrc1 = parametersVec[1], Src2 = parametersVec[2], label = parametersVec[3];
            //Check if Independent
            if (check_dep(registerFile, currentInstr)){
                print_clk();
                print_currInstr(currentInstr);
                executionOfInstructionCount[programCounter]++;
                if (registerFile.get_register_data(Rsrc1) != registerFile.get_register_data(Src2))
                {
                    programCounter = memory.getAddOfLabel(label);
                    cout << "Jumping to label: " << label << endl;
                }
                else
                {
                    programCounter++;
                }
            }
            else{
                complete_dram_activity(rowbuff,  glob, registerFile, memory);
                //rowbuff.UpdateHault(true);
            }
        }
        else if (parametersVec[0] == "slt")
        {
            string Rdest = parametersVec[1], Rsrc1 = parametersVec[2], Src2 = parametersVec[3];
            //Check if Independent
            if (check_dep(registerFile, currentInstr)){
                print_clk();
                print_currInstr(currentInstr);
                executionOfInstructionCount[programCounter]++;
                if (registerFile.get_register_data(Rsrc1) < registerFile.get_register_data(Src2))
                {
                    registerFile.set_register_data(Rdest, 1);
                    cout << "Updated value of " << Rdest << " to "<< 1 << '.' << endl;
                }
                else
                {
                    registerFile.set_register_data(Rdest, 0);
                    cout << "Updated value of " << Rdest << " to "<< 0 << '.' << endl;
                }
                programCounter++;
            }
            else{
                complete_dram_activity(rowbuff,  glob, registerFile, memory);
                //rowbuff.UpdateHault(true);
            }
        }
        else if (parametersVec[0] == "j")
        {
            string label = parametersVec[1];
            print_clk();
            print_currInstr(currentInstr);
            executionOfInstructionCount[programCounter]++;
            if (label.find("$") != string::npos){
                programCounter = getMemAdd(label, registerFile).first;
            }
            else if (isdigit(label[0])){
                stringstream ss(label);
                int instr_add; 
                ss >> instr_add; 
                programCounter = instr_add;
                cout << "Jumping to instruction address: " << instr_add << endl;
            }
            else{
                programCounter = memory.getAddOfLabel(label);
                cout << "Jumping to label: " << label << endl;
            }
        }
        else if (parametersVec[0] == "lw")
        {
            string Rdest = parametersVec[1], mem = parametersVec[2];
            print_clk();
            print_req();
            int add = get_address(mem,registerFile, memory);
            //cout << "Current address is : " << add << endl;
            //registerFile.set_register_data(Rdest, memory.getDataAdd(add));
            rg_hold_lw[registerFile.get_regno(Rdest)]++;
            string to_send = "lw " + Rdest + ","+to_string(add)+","+to_string(registerFile.get_register_data(Rdest));
            glob.push(to_send);
            executionOfInstructionCount[programCounter]++;
            programCounter++;
        }
        else if (parametersVec[0] == "sw")
        {
            string Rdest = parametersVec[1], mem = parametersVec[2];
            print_clk();
            int add = get_address(mem, registerFile, memory);
            //memory.setDataAdd(add, registerFile.get_register_data(Rdest));
            executionOfInstructionCount[programCounter]++;
            programCounter++;
            print_req();
            rg_hold_sw[registerFile.get_regno(Rdest)]++;
            string to_send = "sw " + Rdest + ","+to_string(add)+","+to_string(registerFile.get_register_data(Rdest));
            glob.push(to_send);
        }
        else if (parametersVec[0] == "addi")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2];
            //Check if Independent
            if (rg_hold_lw[registerFile.get_regno(Rdest)] == 0 && rg_hold_sw[registerFile.get_regno(Rdest)] == 0 && rg_hold_lw[registerFile.get_regno(Rsrc)] == 0){
                print_clk();
                print_currInstr(currentInstr);
                executionOfInstructionCount[programCounter]++;
                int ans = registerFile.get_register_data(Rsrc) + stoi(parametersVec[3]);
                registerFile.set_register_data(Rdest, ans);
                cout << "Updated value of " << Rdest << " to "<< ans << '.' << endl;
                programCounter++;
            }
            else{
                //cout << "hello333" << endl;
                complete_dram_activity(rowbuff,  glob, registerFile, memory);
                //rowbuff.UpdateHault(true);
            }
        }
        else
        {
            cout << endl;
            cout << "INVALID INSTRUCTION DETECTED!! " << parametersVec[0] << endl;
            throw exception();
        }
        //registerFile.printRegisters();
        currentInstr = memory.getCurrInstr(programCounter);
        //show();
    }
    //cout << "hello123" << endl;
    complete_dram_activity(rowbuff,  glob, registerFile, memory);
    cout << "===========================================" << endl;
    if (rowbuff.row_no != -1){
        print_wl(rowbuff.row_no);
        cout << Clock_cycle << "-" << Clock_cycle + row_access_delay -1 << endl;
        Clock_cycle+=row_access_delay;
    }
    cout << "===========================================" << endl;
    cout << "Program execution completed" << endl;
    cout << "Total clock cycles consumed: " << Clock_cycle-1 << endl;
    cout << "===========================================" << endl;
    registerFile.printRegisters();
    cout << "===========================================" << endl;
    memory.printMemDataContent();
    cout << "===========================================" << endl;
    cout << "Count of row buffer updates: "<< endl;
    rowbuff.print_buffers();
    cout << "===========================================" << endl;
    cout << "Number of times each instruction was executed: " << endl;
    int j = 0;
    for (int i = 0; i < instructionVector.size(); i++)
    {
        if (instructionVector[i].find(":") == string::npos && (chk_empty(instructionVector[i])==false) && instructionVector[i]!="EOF")
        {
            cout << instructionVector[i] << endl;
            cout << executionOfInstructionCount[j] << endl;
            j++;
        }
    }
}
int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);                
    cout.tie(NULL);
    // Input file section
    fstream infile;
    fstream outfile;
    outfile.open("../output.txt", ios::out);
    if (argc < 2)
    {
        infile.open("../input.txt", ios::in);
    }
    else
    {
        infile.open(argv[1]);
    }
    vector<string> instructionVector;
    string test2;
    // cout << "XX" << '\t' << "YY" << endl;
    // cout << "XX" << '\n' << "YY" << endl;
    // cout << "XX" << ' ' << "YY" << endl;
    // cout << "XX" << '\r' << "YY" << endl;
    while (getline(infile, test2))
    {
        //cout << test2.length() << endl;
        if (chk_empty(test2) == true){
            continue;
        }
        //cout << "BB" << test2 << "BB" << endl;
        instructionVector.push_back(test2);
    }

    for (int i = 0; i < instructionVector.size(); i++)
    {
        outfile << instructionVector[i] << instructionVector[i][instructionVector[i].length() - 1] << endl;
    }
    // Declarations
    instructionVector.push_back("EOF");
    RegisterFile registerFile;
    MemoryUnit memory;
    RowBuffer rowbuff;
    processInstructions(instructionVector, registerFile, memory, rowbuff);
    return 0;
}
