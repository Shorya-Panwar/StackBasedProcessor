#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <bitset>

// Opcode table
std::unordered_map<std::string, std::string> opcodeTable = {
    {"ADD", "10000"},
    {"SUB", "10001"},
    {"MUL", "10010"},
    {"DIV", "10011"},
    {"MOD", "10100"},
    {"CMP", "10101"},
    {"AND", "10110"},
    {"OR", "10111"},
    {"NOT", "11000"},
    {"LSL", "11001"},
    {"LSR", "11010"},
    {"ASR", "11011"},
    {"PUSH", "01000"},
    {"PUSHM", "01001"},
    {"POPM", "01010"},
    {"DUP", "01011"},
    {"POP", "01100"},
    {"JMP", "00100"},
    {"JZ", "00101"},
    {"JNZ", "00110"},
    {"LOAD", "00000"},
    {"STORE", "00001"}
};

// Function to parse and generate the binary instruction
std::string generateInstruction(const std::string& line) {
    std::stringstream ss(line);
    std::string op;
    ss >> op; 

    if (opcodeTable.find(op) == opcodeTable.end()) {
        std::cerr << "Error: Unknown instruction " << op << std::endl;
        return "";
    }

    std::string binaryInstruction = opcodeTable[op]; // Base binary instruction

    if (op == "PUSH" || op == "PUSHM"  || op == "POPM" || op == "JMP" || op == "JZ" || op == "JNZ" 
                    || op == "LOAD" || op == "STORE") {
        std::string immediate;
        ss >> immediate; 
        if (immediate[0] == '[') {
            
            immediate = immediate.substr(1, immediate.size() - 2); // Remove brackets
        }
        
        int immediateValue = std::stoi(immediate);
        std::bitset<27> immediateBinary(immediateValue);
        binaryInstruction += immediateBinary.to_string();
    } else {
        binaryInstruction += std::string(27, '0'); 
    }

    return binaryInstruction;
}

// Function to output assembled source code
void assemble(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty() || line[0] == ';') continue;
        std::string instruction = generateInstruction(line);
        if (!instruction.empty()) {
            output << instruction << std::endl;
        }
    }

    input.close();
    output.close();
}

int main() {
    assemble("E:\\StackBasedProcessor\\Assembler\\input.txt",
             "E:\\StackBasedProcessor\\Assembler\\output.txt");
    std::cout << "Assembly complete." << std::endl;
    return 0;
}
