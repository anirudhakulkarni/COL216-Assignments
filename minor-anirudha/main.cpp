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

template <class T>
string to_string(T t, ios_base &(*f)(ios_base &)) // DONT change its name. will not work.
{
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

public:
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
        regArray[get_regno(register_name)] = data;
    }
    void printRegisters()
    {
        cout << "Contents of Register file:\n";
        for (int i = 0; i < 31; i++)
        {
            cout << regNameArray[i] << ": " << to_string<long>(regArray[i], hex) << ", ";
        }
        cout << regNameArray[31] << ": " << to_string<long>(regArray[31], hex) << endl;
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
    map<int, int> valofVars;
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

            addofLabels.insert(pair<string, int>(instruction.substr(0, instruction.size() - 1), currInstrId));
        }
        else
        {
            MemArray[currInstrId] = instruction;

            currInstrId++;
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
    void setDataWVar(string variable, int value)
    {
        if (addofVars.find(variable) == addofVars.end())
        {
            addofVars[variable] = currVarId;
            currVarId++;
        }
        valofVars[addofVars[variable]] = value;
    }
    int getDataWVar(string variable)
    {
        if (addofVars.find(variable) == addofVars.end())
            return -1;
        return valofVars[addofVars[variable]];
    }
    void setDataAdd(int address, int value)
    {
        MemArray[address + partition] = to_string(value);
    }
    int getDataAdd(int address)
    {
        string temp = MemArray[address + partition];
        stringstream geek(temp);
        int ans = 0;
        geek >> ans;
        return ans;
    }
    void printMemData()
    {
        int curr_int = 1048576 / 2;
        for (int i = curr_int; i < 1048576; i++)
        {
            if (MemArray[i] != "")
            {
                cout << i - 1048576 / 2 << "-" << 3 + i - 1048576 / 2 << ": " << MemArray[i] << endl;
            }
        }
    }
};
int getMemAdd(string add, RegisterFile &registerFile)
{

    size_t start = add.find_first_of("(");
    size_t end = add.find_first_of(")");
    int sz = end - start - 1;
    string reg = add.substr(start + 1, sz);
    stringstream ss(add.substr(0, start));
    int off;
    ss >> off;
    int add_memory = registerFile.get_register_data(reg);
    int final_add = off / 4 + add_memory;
    return final_add;
}
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
bool isDepedent(string instr1, string instr2, int rowRemaining)
{
    // instr 1 is of lw or sw type which is running in background
    vector<string> instr1parsed = parseInstr(instr1);
    vector<string> instr2parsed = parseInstr(instr2);
    string workingRegister = instr1parsed[1];
    if (instr1parsed[0] == "sw")
    {
        if (rowRemaining > 0)
        {
            if (instr2parsed[0] == "add")
            {
                if (workingRegister == instr2parsed[1])
                {
                    return true;
                }
                return false;
            }
            if (instr2parsed[0] == "addi")
            {
                if (workingRegister == instr2parsed[1])
                {
                    return true;
                }
                return false;
            }
        }
        else
        {
            if (instr2parsed[0] == "add")
            {
                if (workingRegister != instr2parsed[1] && workingRegister != instr2parsed[2] && workingRegister != instr2parsed[3])
                {
                    return false;
                }
                return true;
            }
            if (instr2parsed[0] == "addi")
            {
                if (workingRegister != instr2parsed[1] && workingRegister != instr2parsed[2])
                {
                    return false;
                }
                return true;
            }
            return true;
        }
    }
    if (instr1parsed[0] == "lw")
    {
        if (instr2parsed[0] == "add")
        {
            if (workingRegister != instr2parsed[1] && workingRegister != instr2parsed[2] && workingRegister != instr2parsed[3])
            {
                return false;
            }
            return true;
        }
        if (instr2parsed[0] == "addi")
        {
            if (workingRegister != instr2parsed[1] && workingRegister != instr2parsed[2])
            {
                return false;
            }
            return true;
        }
        return true;
    }
    return true;
}
tuple<int, int, int> getNUpdateWork(int &activeRowStart, int currAdd)
{
    tuple<int, int, int> ans = make_tuple(2 * ROW_ACCESS_DELAY, COL_ACCESS_DELAY, 1);
    if (activeRowStart == -1024)
    {
        ans = make_tuple(ROW_ACCESS_DELAY, COL_ACCESS_DELAY, 1);
    }
    else if (int(currAdd / 1024) * 1024 == activeRowStart)
    {
        ans = make_tuple(0, COL_ACCESS_DELAY, 0);
    }
    activeRowStart = currAdd;
    return ans;
}
void printCycles(vector<tuple<string, int, int>> &cycles)
{
    // <cycle data, cycle number, no of cycles required>
    for (int i = cycles.size() - 1; i >= 0; i--)
    {
        if (get<2>(cycles[i]) > 1)
        {
            cycles.push_back(make_tuple(": Copying buffer to DRAM", max(get<1>(cycles[cycles.size() - 1]) + 1, get<2>(cycles[i]) + get<1>(cycles[i])), ROW_ACCESS_DELAY));
            break;
        }
    }

    for (int i = 0; i < cycles.size(); i++)
    {
        if (i == cycles.size() - 1)
        {
            cout << "cycle " + to_string(get<1>(cycles[i])) + "-" + to_string(get<2>(cycles[i]) + get<1>(cycles[i]) - 1) + get<0>(cycles[i]) << endl;
        }
        else if (get<2>(cycles[i]) > 1)
        {
            if (get<2>(cycles[i]) == 2 * ROW_ACCESS_DELAY + COL_ACCESS_DELAY)
            {
                cout << "cycle " + to_string(get<1>(cycles[i])) + "-" + to_string(get<1>(cycles[i]) + ROW_ACCESS_DELAY - 1) + get<0>(cycles[i]) + " : Copying back ROW BUFFER to DRAM" << endl;
                cout << "cycle " + to_string(get<1>(cycles[i]) + ROW_ACCESS_DELAY) + "-" + to_string(get<1>(cycles[i]) + 2 * ROW_ACCESS_DELAY - 1) + get<0>(cycles[i]) + " : Copying row from DRAM to ROW BUFFER" << endl;
                cout << "cycle " + to_string(get<1>(cycles[i]) + 2 * ROW_ACCESS_DELAY) + "-" + to_string(get<2>(cycles[i]) + get<1>(cycles[i]) - 1) + get<0>(cycles[i]) + " : col access in ROW BUFFER" << endl;
            }
            else if (get<2>(cycles[i]) == ROW_ACCESS_DELAY + COL_ACCESS_DELAY)
            {
                cout << "cycle " + to_string(get<1>(cycles[i])) + "-" + to_string(get<1>(cycles[i]) + ROW_ACCESS_DELAY - 1) + get<0>(cycles[i]) + " : Initial Copying row from DRAM to ROW BUFFER" << endl;
                cout << "cycle " + to_string(get<1>(cycles[i]) + ROW_ACCESS_DELAY) + "-" + to_string(get<2>(cycles[i]) + get<1>(cycles[i]) - 1) + get<0>(cycles[i]) + " : col access in ROW BUFFER" << endl;
            }
            else if (get<2>(cycles[i]) == COL_ACCESS_DELAY)
            { // only accessing buffer
                cout << "cycle " + to_string(get<1>(cycles[i])) + "-" + to_string(get<2>(cycles[i]) + get<1>(cycles[i]) - 1) + get<0>(cycles[i]) + " : col access in same ROW BUFFER" << endl;
            }
        }
        else
            cout << "cycle " + to_string(get<1>(cycles[i])) + get<0>(cycles[i]) << endl;
    }
}
void processInstructions(vector<string> instructionVector, RegisterFile &registerFile, MemoryUnit &memory)
{
    int executionOfInstructionCount[999];
    int instructionsSoFar = 1;
    int programCounter = 0;
    int activeRowStart = -1024;
    vector<tuple<string, int, int>> cycles;
    string lastInstruction = "add $t1, 0";
    int currCycle = 0;
    int rowBufferUpdate = 0;
    int rowRemaining = 0;
    int colRemaining = 0;
    int spareCycles = rowRemaining + colRemaining;

    ////////////////////////////////////////
    for (int i = 0; i < instructionVector.size(); i++)
    {
        string temp = instructionVector[i];
        memory.storeInstr(temp);
    }
    string currentInstr = memory.getCurrInstr(0);
    while (currentInstr != "")
    {
        cout << "Instruction number executed: " << instructionsSoFar << endl;

        cout << "rowBuffer " << rowRemaining << " colBuffer " << colRemaining << " CurrentCycle " << currCycle << endl;
        executionOfInstructionCount[programCounter]++;
        // cout << currentInstr << endl;

        vector<string> parametersVec = parseInstr(currentInstr);
        // Last instruction is remaining and blocking
        if (spareCycles > 0 && isDepedent(lastInstruction, currentInstr, rowRemaining))
        {
            cout << lastInstruction;
            cout << " is blocking " << currentInstr << endl;
            currCycle += spareCycles;
            spareCycles = 0;
            rowRemaining = 0;
            colRemaining = 0;
            continue;
        }
        // Last instruction is remaining and chance for improvement
        else if (spareCycles > 0)
        {
            cout << "Non-blocking Execution" << endl;
            spareCycles--;
            if (rowRemaining > 0)
            {
                rowRemaining--;
            }
            else
            {
                colRemaining--;
            }
        }
        //Last instruction is over proceed as normal
        currCycle++;

        if (parametersVec[0] == "add")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2], Src = parametersVec[3];
            int ans = registerFile.get_register_data(Rsrc) + registerFile.get_register_data(Src);
            registerFile.set_register_data(Rdest, ans);
            programCounter++;
            cycles.push_back(make_tuple(": " + Rdest + " = " + Rsrc + " + " + Src, currCycle, 1));
        }
        else if (parametersVec[0] == "addi")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2];
            int ans = registerFile.get_register_data(Rsrc) + stoi(parametersVec[3]);
            registerFile.set_register_data(Rdest, ans);
            programCounter++;
            cycles.push_back(make_tuple(": " + Rdest + " = " + Rsrc + " + " + parametersVec[3], currCycle, 1));
        }
        else if (parametersVec[0] == "lw")
        {
            string Rdest = parametersVec[1], mem = parametersVec[2];
            if (mem.find("(") == string::npos || mem.find(")") == string::npos)
            {
                //Case 1, instruction of the type "lw var_name or address";
                if (isdigit(mem[0]))
                {
                    // this is working case
                    registerFile.set_register_data(Rdest, memory.getDataAdd(stoi(mem)));
                    auto [r, c, rbf] = getNUpdateWork(activeRowStart, stoi(mem));
                    rowRemaining = r;
                    colRemaining = c;
                    // cout << "INNER row " << rowRemaining << " col " << colRemaining << " curr " << currCycle << endl;
                    rowBufferUpdate += rbf;

                    spareCycles = rowRemaining + colRemaining;
                    cycles.push_back(make_tuple(": DRAM request issued", currCycle, 1));
                    cycles.push_back(make_tuple(": " + Rdest + " = " + to_string(memory.getDataAdd(stoi(mem))), currCycle + 1, spareCycles));
                }
                else
                {
                    registerFile.set_register_data(Rdest, memory.getDataWVar(mem));
                }
            }
            else
            {
                //Case 2, instruction of the type "lw offset($Reg)";
                int mem_add = getMemAdd(mem, registerFile);
                registerFile.set_register_data(Rdest, memory.getDataAdd(mem_add));
            }
            programCounter++;
            lastInstruction = currentInstr;
        }
        else if (parametersVec[0] == "sw")
        {
            string Rdest = parametersVec[1], mem = parametersVec[2];
            if (mem.find("(") == string::npos || mem.find(")") == string::npos)
            {
                //Case 1, instruction of the type "sw var_name";
                if (isdigit(mem[0]))
                {
                    // this is working case
                    memory.setDataAdd(stoi(mem), registerFile.get_register_data(Rdest));
                    auto [r, c, rbf] = getNUpdateWork(activeRowStart, stoi(mem));
                    rowRemaining = r;
                    colRemaining = c;
                    rowBufferUpdate += rbf;
                    spareCycles = rowRemaining + colRemaining;
                    // cout << "INNER row " << rowRemaining << " col " << colRemaining << " curr " << currCycle << endl;

                    cycles.push_back(make_tuple(": DRAM request issued", currCycle, 1));
                    cycles.push_back(make_tuple(": memory address " + mem + "-" + to_string(stoi(mem) + 3) + " = " + to_string(registerFile.get_register_data(Rdest)), currCycle + 1, spareCycles));
                }
                else
                {
                    memory.setDataWVar(mem, registerFile.get_register_data(Rdest));
                }
            }
            else
            {
                //Case 2, instruction of the type "sw offset($Reg)";
                int mem_add = getMemAdd(mem, registerFile);
                memory.setDataAdd(mem_add, registerFile.get_register_data(Rdest));
            }
            programCounter++;
            lastInstruction = currentInstr;
        }

        //NOT NEEDED FOR THIS TASK
        else if (parametersVec[0] == "sub")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2], Src = parametersVec[3];
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) - registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (parametersVec[0] == "mul")
        {
            string Rdest = parametersVec[1], Rsrc = parametersVec[2], Src = parametersVec[3];
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) * registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (parametersVec[0] == "beq")
        {
            string Rsrc1 = parametersVec[1], Src2 = parametersVec[2], label = parametersVec[3];
            if (registerFile.get_register_data(Rsrc1) == registerFile.get_register_data(Src2))
            {

                if (label[0] == '-')
                {
                    programCounter += stoi(label) / 4;
                }
                else
                {
                    programCounter = memory.getAddOfLabel(label);
                }
            }
            else
            {
                programCounter++;
            }
        }
        else if (parametersVec[0] == "bne")
        {
            cout << programCounter << endl;
            string Rsrc1 = parametersVec[1], Src2 = parametersVec[2], label = parametersVec[3];

            if (registerFile.get_register_data(Rsrc1) != registerFile.get_register_data(Src2))
            {
                if (label[0] == '-')
                {
                    programCounter += stoi(label) / 4;
                }
                else
                {
                    programCounter = memory.getAddOfLabel(label);
                }
            }
            else
            {
                programCounter++;
            }
            cout << programCounter << endl;
        }
        else if (parametersVec[0] == "slt")
        {
            string Rdest = parametersVec[1], Rsrc1 = parametersVec[2], Src2 = parametersVec[3];
            if (registerFile.get_register_data(Rsrc1) < registerFile.get_register_data(Src2))
            {
                registerFile.set_register_data(Rdest, 1);
            }
            else
            {
                registerFile.set_register_data(Rdest, 0);
            }
            programCounter++;
        }
        else if (parametersVec[0] == "j")
        {
            // cout.flush();
            string label = parametersVec[1];

            if (label.find("$") != string::npos)
            {
                programCounter = getMemAdd(label, registerFile);
            }
            else if (isdigit(label[0]))
            {
                stringstream ss(label);
                int instr_add;
                ss >> instr_add;
                programCounter = instr_add;
            }
            else
            {
                programCounter = memory.getAddOfLabel(label);
            }
            // cout << "HUT" << programCounter << endl;
        }
        else
        {
            cout << endl;
            cout << "INVALID INSTRUCTION DETECTED!! " << parametersVec[0] << endl;
            throw exception();
        }

        // registerFile.printRegisters();
        currentInstr = memory.getCurrInstr(programCounter);
        instructionsSoFar++;
    }

    cout << "===========================================" << endl;
    cout << "Program execution completed" << endl;
    cout << "Printing cycles: " << endl;
    printCycles(cycles);
    cout << "===========================================" << endl;
    cout << "Printing Memory data: " << endl;
    memory.printMemData();
    cout << "===========================================" << endl;
    cout << "Total RowBuffer Updates: " << rowBufferUpdate << endl;
    // will not work now
    // cout << "Number of times each instruction was executed: " << endl;
    // int j = 0;
    // for (int i = 0; i < instructionVector.size(); i++)
    // {
    //     if (instructionVector[i].find(":") == string::npos)
    //     {
    //         cout << instructionVector[i] << endl;
    //         cout << executionOfInstructionCount[j] << endl;
    //         j++;
    //     }
    // }
}

int main(int argc, char const *argv[])
{
    fio;
    // Input file section
    fstream infile;
    fstream outfile;
    outfile.open("../testcases_off/testcase1_out.txt", ios::out);
    if (argc < 2)
    {
        infile.open("../testcases_off/testcase1.txt", ios::in);
    }
    else
    {
        infile.open(argv[1]);
    }
    vector<string> instructionVector;
    string test2;
    while (getline(infile, test2))
    {
        instructionVector.push_back(test2.substr(0, test2.length() - 1));
    }

    for (int i = 0; i < instructionVector.size(); i++)
    {
        outfile << instructionVector[i] << instructionVector[i][instructionVector[i].length() - 1] << endl;
    }
    // Declarations
    ROW_ACCESS_DELAY = strtol(argv[2], NULL, 10);
    COL_ACCESS_DELAY = strtol(argv[3], NULL, 10);
    RegisterFile registerFile;
    MemoryUnit memory;
    processInstructions(instructionVector, registerFile, memory);
    return 0;
}