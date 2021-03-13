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
    for (int i = 0; i < output.length() - 1; i++)
    {
        if (output[i] == ',' && output[i + 1] != ' ')
        {
            output = output.substr(0, i + 1) + " " + output.substr(i + 1);
        }
    }

    return output;
}
vector<string> getInstrucVec(ifstream &infile)
{
    vector<string> temp;
    string test2;
    while (getline(infile, test2))
    {
        temp.push_back(remove_extra_whitespaces(test2));
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
        if (register_name[0] == '$')
            return regArray[get_regno(register_name)];
        else
            return stoi(register_name);
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
class MemoryArray
{
private:
    // named instructions : label
    // named data: given name of memory location return address of location. can use map
public:
    void storeInstr(string instruction)
    {
        return;
    }
    int getAddOfLabel(string label)
    {
        return 0;
    }
    string getCurrInstr(int current)
    {
        return "";
    }
    void setData(string variable, int value)
    {
        return;
    }
    int getData(string variable)
    {
        return 0;
    }
};

void processInstructions(vector<string> instructionVector, RegisterFile &registerFile, MemoryArray &memory)
{
    int instructionsSoFar = 1;
    int programCounter = 0;
    for (int i = 0; i < instructionVector.size(); i++)
    {
        memory.storeInstr(instructionVector[i]);
    }
    string currentInstr = memory.getCurrInstr(0);
    while (currentInstr != "")
    {
        // Assume that instructions are in format instruction_register,_register etc. only 1 space and 1 comma
        if (currentInstr.substr(0, 3) == "add")
        {
            string Rdest = currentInstr.substr(4, 3), Rsrc = currentInstr.substr(9, 3), Src = currentInstr.substr(14);
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) + registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (currentInstr.substr(0, 3) == "sub")
        {
            string Rdest = currentInstr.substr(4, 3), Rsrc = currentInstr.substr(9, 3), Src = currentInstr.substr(14);
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) - registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (currentInstr.substr(0, 3) == "mul")
        {
            string Rdest = currentInstr.substr(4, 3), Rsrc = currentInstr.substr(9, 3), Src = currentInstr.substr(14);
            registerFile.set_register_data(Rdest, registerFile.get_register_data(Rsrc) * registerFile.get_register_data(Src));
            programCounter++;
        }
        else if (currentInstr.substr(0, 3) == "beq")
        {
            string Rsrc1 = currentInstr.substr(4, 3), Src2 = currentInstr.substr(9, 3), label = currentInstr.substr(14);
            if (registerFile.get_register_data(Rsrc1) == registerFile.get_register_data(Src2))
            {
                programCounter = memory.getAddOfLabel(label);
            }
            else
            {
                programCounter++;
            }
        }
        else if (currentInstr.substr(0, 3) == "bne")
        {
            string Rsrc1 = currentInstr.substr(4, 3), Src2 = currentInstr.substr(9, 3), label = currentInstr.substr(14);
            if (registerFile.get_register_data(Rsrc1) != registerFile.get_register_data(Src2))
            {
                programCounter = memory.getAddOfLabel(label);
            }
            else
            {
                programCounter++;
            }
        }
        else if (currentInstr.substr(0, 3) == "slt")
        {
            string Rdest = currentInstr.substr(4, 3), Rsrc1 = currentInstr.substr(9, 3), Src2 = currentInstr.substr(14);
            if (registerFile.get_register_data(Rsrc1) < registerFile.get_register_data(Src2))
            {
                registerFile.set_register_data(Rdest, 1);
            }
            else
            {
                registerFile.set_register_data(Rdest, 0);
            }
        }
        else if (currentInstr.substr(0, 1) == "j")
        {
            string label = currentInstr.substr(2);
            programCounter = memory.getAddOfLabel(label);
        }
        else if (currentInstr.substr(0, 2) == "lw")
        {
            string Rdest = currentInstr.substr(3, 3), mem = currentInstr.substr(8);
            registerFile.set_register_data(Rdest, memory.getData(mem));
            programCounter++;
        }
        else if (currentInstr.substr(0, 2) == "sw")
        {
            string Rdest = currentInstr.substr(3, 3), mem = currentInstr.substr(8);
            memory.setData(mem, registerFile.get_register_data(Rdest));
            programCounter++;
        }
        else
        {
            cout << "Raise exception. Unknown instruction found" << endl;
        }

        /* PROCESS INSTRUCTION*/

        cout << "Instruction number executed: " << instructionsSoFar << endl;
        registerFile.printRegisters();
        currentInstr = memory.getCurrInstr(programCounter);
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
    vector<string> instructionVector = getInstrucVec(infile);
    for (int i = 0; i < instructionVector.size(); i++)
    {
        cout << instructionVector[i] << endl;
    }

    // Declarations
    RegisterFile registerFile;
    MemoryArray memory;
    processInstructions(instructionVector, registerFile, memory);
    registerFile.set_register_data("t0", INT32_MAX + 1);
    cout << registerFile.get_register_data("t0") << endl;
    registerFile.printRegisters();

    return 0;
}
