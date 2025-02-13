#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <set>
#include <bitset>
using namespace std;

map<string,string> opcodeTable = {
    {"ADD","10000"},
    {"SUB","10001"},
    {"MUL","10010"},
    {"DIV","10011"},
    {"MOD","10100"},
    {"CMP","10101"},
    {"AND","10110"},
    {"OR","10111"},
    {"NOT","11000"},
    {"LSL","11001"},
    {"LSR","11010"},
    {"ASR","11011"},
    {"ADD","10000"},
    {"PUSH","01000"},
    {"PUSHM","01001"},
    {"POP","01010"},
    {"POPM","01011"},
    {"DUP","01100"},
    {"JMP","00100"},
    {"JZ","00101"},
    {"JNZ","00110"},
    {"LOAD","00000"},
    {"STORE","00001"},
};

set<string> immediateInstructions = {"PUSH","PUSHM","POPM","JMP","JZ","JNZ","LOAD","STORE"};

string generateInstruction(string inst){
    stringstream ss(inst);
    string opcode;
    ss >> opcode;

    if(opcodeTable.find(opcode) == opcodeTable.end()){
        cerr << "Invalid Instruction" << endl;
        return "";
    }

    string binInst;
    binInst = opcodeTable[opcode];

    if(immediateInstructions.find(opcode) != immediateInstructions.end()){
        string imm;
        ss >> imm;

        if(imm[0] == '['){
            imm = imm.substr(1,imm.length()-2);
        }

        bitset<27> binaryImm(stoi(imm));
        binInst += binaryImm.to_string();
    }
    else{
        binInst += string(27,'0');
    }

    return binInst;
}

void assembleFile(string input, string output){
    ifstream inputFile(input);
    ofstream outputFile(output);

    if(!inputFile.is_open() || !outputFile.is_open()){
        cerr << "File Opening Error" << endl;
    }

    string inst;
    while(getline(inputFile,inst)){
        if(inst.empty() || inst[0] == '#') continue;

        string binInst = generateInstruction(inst);

        if(!binInst.empty()){
            outputFile << binInst << endl;
        }
    }
}

int main(){
    assembleFile("E:\\StackBasedProcessor\\SourceCode.txt",
                 "E:\\StackBasedProcessor\\InstructionMem.txt");
    cout << "Assembly Complete" << endl;
    return 0;
}
