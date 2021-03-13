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
            return -1;
        else if (s == "gp")
            return 28;
        else if (s == "sp")
            return 29;
        else if (s == "fp")
            return 30;
        else if (s == "ra")
            return 31;
        else
            return -1;
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
        // BUG : Assumed that $zero register is $ze everywhere due to parsing convinience as len of each register name is 3 example $s1, $t7
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
class MemoryUnit
{
public:
    string MemArray[100];
    int currInstrId = 0;
    int currVarId = 0;
    int partition = 100 / 2;
    map<string, int> addofLabels;
    map<string, int> addofVars;
    map<int, int> valofVars;
    MemoryUnit()
    {
        for (int it = 0; it < 100; it++)
            MemArray[it] = "";
        currInstrId = 0;
        currVarId = 100 / 2;
        partition = 100 / 2;
    }
    void storeInstr(string instruction)
    {

        if (instruction.find(":") != string::npos)
        {
            // label
            addofLabels.insert(pair<string, int>(instruction, currInstrId));
        }
        else
        {
            cout << instruction << endl;
            cout << currInstrId << endl;
            MemArray[currInstrId] = instruction;

            currInstrId++;
        }
    }
    int getAddOfLabel(string label)
    {
        if (addofLabels.find(label) == addofLabels.end())
            return -1;
        return addofLabels[label];
    }
    string getCurrInstr(int current)
    {
        return MemArray[current];
    }
    void setData(string variable, int value)
    {
        if (addofVars.find(variable) == addofVars.end())
        {
            addofVars[variable] = currVarId;
            currVarId++;
        }
        valofVars[addofVars[variable]] = value;
    }
    int getData(string variable)
    {
        if (addofVars.find(variable) == addofVars.end())
            return -1;
        return valofVars[addofVars[variable]];
    }
    void setDataAdd(int address, int value)
    {
        valofVars[address + partition] = value;
    }
    int getDataAdd(int address)
    {
        return valofVars[address];
    }
    void printMemContent()
    {
        int curr_int = 0;
        if (MemArray[curr_int] == "")
        {
            cout << "fault" << endl;
        }
        else
        {
            cout << "good" << endl;
        }
        while (MemArray[curr_int] != "")
        {
            cout << "Memory Instruction at Address: " << curr_int << " is: " << MemArray[curr_int] << endl;
        }
        curr_int = 100 / 2;
        while (MemArray[curr_int] != "")
        {
            cout << "Memory Instruction at Address: " << curr_int << " is: " << MemArray[curr_int] << endl;
        }
    }
};

void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out)
{
    //splitting string on the basis of delim
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

void filter_instruction(string &currentInstr, std::vector<std::string> vectReg, std::vector<std::string> vectInstr)
{
    const char delim1 = ',';
    const char delim2 = ' ';
    tokenize(currentInstr, delim1, vectReg);
    tokenize(vectReg[0], delim2, vectInstr);
    string reg1 = vectInstr[1];
    vectInstr.pop_back();
    vectReg.erase(vectReg.begin());
    vectReg.insert(vectReg.begin(), reg1);
}
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
    int final_add = off + add_memory;
    return final_add;
}
void processInstructions(vector<string> instructionVector, RegisterFile &registerFile, MemoryUnit &memory)
{
    int instructionsSoFar = 1;
    int programCounter = 0;
    for (int i = 0; i < instructionVector.size(); i++)
    {
        string temp = instructionVector[i];
        memory.storeInstr(temp);
    }
    string currentInstr = memory.getCurrInstr(0);
    cout << currentInstr << endl;
    cout.flush();
    cout << "XXXX";
    cout << memory.getCurrInstr(1) << endl;
    while (currentInstr != "")
    {
        cout << currentInstr;
        cout.flush();
        cout << "Current Instruction : " << currentInstr << endl;
        cout << "NOT HERE\n";
        cout.flush();
        cout.flush();
        vector<string> vectReg;
        cout << "NOT HERE\n";

        vector<string> vectInstr;
        cout << "NOT HERE\n";
        filter_instruction(currentInstr, vectReg, vectInstr);
        cout << "NOT HERE2\n";
        for (auto &reg : vectReg)
        {
            reg.erase(remove(reg.begin(), reg.end(), ' '), reg.end());
        }
        if (vectInstr[0] == "add")
        {
            string Rdest = vectReg[0], Rsrc = vectReg[1], Src = vectReg[2];
            cout << "MOTHERF\n";
            cout << Rdest << Rsrc << Src << endl;
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) + registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (vectInstr[0] == "sub")
        {
            string Rdest = vectReg[0], Rsrc = vectReg[1], Src = vectReg[2];
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) - registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (vectInstr[0] == "mul")
        {
            string Rdest = vectReg[0], Rsrc = vectReg[1], Src = vectReg[2];
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) * registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (vectInstr[0] == "beq")
        {
            string Rsrc1 = vectReg[0], Src2 = vectReg[1], label = vectReg[2];
            if (registerFile.get_register_data(Rsrc1) == registerFile.get_register_data(Src2))
            {
                programCounter = memory.getAddOfLabel(label);
            }
            else
            {
                programCounter++;
            }
        }
        else if (vectInstr[0] == "bne")
        {
            string Rsrc1 = vectReg[0], Src2 = vectReg[1], label = vectReg[2];
            if (registerFile.get_register_data(Rsrc1) != registerFile.get_register_data(Src2))
            {
                programCounter = memory.getAddOfLabel(label);
            }
            else
            {
                programCounter++;
            }
        }
        else if (vectInstr[0] == "slt")
        {
            string Rdest = vectReg[0], Rsrc1 = vectReg[1], Src2 = vectReg[2];
            if (registerFile.get_register_data(Rsrc1) < registerFile.get_register_data(Src2))
            {
                registerFile.set_register_data(Rdest, 1);
            }
            else
            {
                registerFile.set_register_data(Rdest, 0);
            }
        }
        else if (vectInstr[0] == "j")
        {
            string label = vectReg[1];
            programCounter = memory.getAddOfLabel(label);
        }
        else if (vectInstr[0] == "lw")
        {
            string Rdest = vectReg[0], mem = vectReg[1];
            if (mem.find("(") == string::npos || mem.find(")") == string::npos)
            {
                //Case 1, instruction of the type "sw var_name";
                registerFile.set_register_data(Rdest, memory.getData(mem));
            }
            else
            {
                int mem_add = getMemAdd(mem, registerFile);
                registerFile.set_register_data(Rdest, memory.getDataAdd(mem_add));
            }
            programCounter++;
        }
        else if (vectInstr[0] == "sw")
        {
            string Rdest = vectReg[0], mem = vectReg[1];
            if (mem.find("(") == string::npos || mem.find(")") == string::npos)
            {
                //Case 1, instruction of the type "sw var_name";

                memory.setData(mem, registerFile.get_register_data(Rdest));
            }
            else
            {
                //Case 2, instruction of the type "sw offset($Reg)";
                int mem_add = getMemAdd(mem, registerFile);
                memory.setDataAdd(mem_add, registerFile.get_register_data(Rdest));
            }
            programCounter++;
        }
        else if (vectInstr[0] == "addi")
        {
            string Rdest = vectReg[0], Rsrc = vectReg[1];
            // int val;
            // stringstream ss(vectReg[2]);
            // val >> ss;
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) + stoi(vectReg[2]));
            programCounter++;
        }
        else
        {
            cout << "Raise exception. Unknown instruction found" << endl;
        }
        cout << "Instruction number executed: " << instructionsSoFar << endl;
        registerFile.printRegisters();
        currentInstr = memory.getCurrInstr(programCounter);
        instructionsSoFar++;
    }
}

int main(int argc, char const *argv[])
{
    fio;
    // Input file section
    fstream infile;
    fstream outfile;
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
    while (getline(infile, test2))
    {
        instructionVector.push_back(test2);
    }
    outfile.open("../output.txt", ios::out);
    for (int i = 0; i < instructionVector.size(); i++)
    {
        outfile << instructionVector[i] << instructionVector[i].length() << endl;
    }
    cout << "Printing instruction vector over" << endl;
    // Declarations
    RegisterFile registerFile;
    MemoryUnit memory;
    processInstructions(instructionVector, registerFile, memory);
    return 0;
}
