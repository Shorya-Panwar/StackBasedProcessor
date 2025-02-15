module processor(
    input wire clk,
    input wire reset,
    output reg signed [31:0] top
);

    reg [9:0] pc;
    reg [31:0] instructionMem [0:1023];
    
    reg signed [7:0] sp;
    reg signed [31:0] stack [0:255];

    reg [31:0] instruction;
    reg [4:0] opcode;
    reg signed [31:0] immediate;
    reg [9:0] memoryAddr;
    
    reg signed [31:0] dataMem [0:1023];

    reg signed [31:0] op1, op2, result;

    initial begin
        $readmemb("InstructionMem.txt",instructionMem);
        pc = 0;
        sp = -1;
    end

    always @(posedge clk or posedge reset) begin
        if(reset) begin
            pc = 0;
            sp = -1;
        end
        else begin
            instruction = instructionMem[pc];
            pc = pc + 1;

            opcode = instruction[31:27];
            immediate = {{5{instruction[26]}},instruction[26:0]};
            memoryAddr = instruction[9:0];

            case(opcode)
                5'b10000: begin // ADD
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    stack[sp] = op1 + op2;
                end
                5'b10001: begin // SUB
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    stack[sp] = op1 - op2;
                end
                5'b10010: begin // MUL
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    stack[sp] = op1 * op2;
                end
                5'b10011: begin // DIV
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    stack[sp] = op1 / op2;
                end
                5'b10100: begin // MOD
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    stack[sp] = op1 % op2;
                end
                5'b10101: begin // CMP
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    if(op1 == op2) stack[sp] = 0;
                    else if(op1 > op2) stack[sp] = 1;
                    else stack[sp] = -1;
                end
                5'b10110: begin // AND
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    stack[sp] = op1 & op2;
                end
                5'b10111: begin // OR
                    op2 = stack[sp];
                    sp = sp - 1;
                    op1 = stack[sp];
                    stack[sp] = op1 | op2;
                end
                5'b11000: begin // NOT
                    op1 = stack[sp];
                    stack[sp] = ~op1;
                end
                5'b11001: begin // LSL
                    op1 = stack[sp];
                    stack[sp] = op1 << 1;
                end
                5'b11010: begin // LSR
                    op1 = stack[sp];
                    stack[sp] = op1 >> 1;
                end
                5'b11011: begin // ASR
                    op1 = stack[sp];
                    stack[sp] = op1 >>> 1;
                end
                5'b01000: begin // PUSH imm
                    sp = sp + 1;
                    stack[sp] = immediate;
                end
                5'b01001: begin // PUSHM [addr]
                    sp = sp + 1;
                    stack[sp] = dataMem[memoryAddr];
                end
                5'b01010: begin // POP
                    sp = sp - 1;
                end
                5'b01011: begin // POPM [addr]
                    dataMem[memoryAddr] = stack[sp];
                    sp = sp - 1;
                end
                5'b01100: begin // DUP
                    sp = sp + 1;
                    stack[sp] = stack[sp - 1];
                end
                5'b00100: begin // JMP [addr]
                    pc = memoryAddr;
                end
                5'b00101: begin // JZ [addr]
                    if(stack[sp] == 0) pc = memoryAddr;
                    sp = sp - 1;
                end
                5'b00110: begin // JNZ [addr]
                    if(stack[sp] != 0) pc = memoryAddr;
                    sp = sp - 1;
                end
                5'b00000: begin // LOAD [addr]
                    stack[sp] = dataMem[memoryAddr];
                end
                5'b00001: begin // STORE [addr]
                    dataMem[memoryAddr] = stack[sp];
                end
                default: begin
                end
            endcase

            top = (sp >= 0) ? stack[sp] : -1;

        end
    end

endmodule