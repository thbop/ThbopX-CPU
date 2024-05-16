#ifndef CPU_H
#define CPU_H

#define MEMORYSIZE 0xFF // Bytes

typedef unsigned char u8;

enum instructions {
    INS_BRK       = 0x00, // Mov
    INS_MOV_IM_A  = 0x01,
    INS_MOV_IM_X  = 0x02,
    INS_MOV_IM_Y  = 0x03,
    INS_MOV_M_A   = 0x04,
    INS_MOV_M_X   = 0x05,
    INS_MOV_M_Y   = 0x06,
    INS_MOV_MX_A  = 0x07, // LD Addr+X           -> A
    INS_MOV_MY_A  = 0x08, // LD Ptr+Y  -> Addr   -> A
    INS_MOV_MXY_A = 0x09, // LD Ptr+Y  -> Addr+x -> A
    INS_MOV_A_M   = 0x0A,
    INS_MOV_X_M   = 0x0B,
    INS_MOV_Y_M   = 0x0C,
    INS_MOV_A_MX  = 0x0D, // ST A           -> Addr+X          
    INS_MOV_A_MY  = 0x0E, // ST A -> Ptr+Y  -> Addr  
    INS_MOV_A_MXY = 0x0F, // ST A -> Ptr+Y  -> Addr+x
                          // Increments
                          
                          // Stack operations

                          // Control flow

};

struct {
    u8 A, X, Y, PC;
} reg;

struct {
    u8 Z, C;
} flag;


u8 memory[MEMORYSIZE] = { 0 };
// $0  - $10: Hardware Calls
// $11 - $20: Stack
// $21 - $6F: Program ROM
// $70 - $FD: RAM
// $FE      : IRQ Interrupt Vector
// $FF      : BRK Vector

void initialize() {
    reg.A = reg.X = reg.Y = 0;
    reg.PC = 0x21;
}

u8* fetch_bytes( u8 length ) {
    reg.PC += length;
    return memory+reg.PC-length;
}

void execute(u8 ins) {
    reg.PC++;
    switch(ins) {
        case INS_MOV_IM_A: {
            reg.A = *fetch_bytes(1);
        } break;
        case INS_MOV_IM_X: {
            reg.X = *fetch_bytes(1);
        } break;
        case INS_MOV_IM_Y: {
            reg.Y = *fetch_bytes(1);
        } break;
        case INS_MOV_M_A: {
            reg.A = memory[*fetch_bytes(1)];
        } break;
        case INS_MOV_M_X: {
            reg.X = memory[*fetch_bytes(1)];
        } break;
        case INS_MOV_M_Y: {
            reg.Y = memory[*fetch_bytes(1)];
        } break;
        case INS_MOV_MX_A: {
            reg.A = memory[*fetch_bytes(1)+reg.X];
        } break;
        case INS_MOV_MY_A: {
            reg.A = memory[ memory[*fetch_bytes(1)]+reg.Y ];
        } break;
        case INS_MOV_MXY_A: {
            reg.A = memory[ memory[*fetch_bytes(1)+reg.X]+reg.Y ];
        } break;
        case INS_MOV_A_M: {
            memory[*fetch_bytes(1)] = reg.A;
        } break;
        case INS_MOV_X_M: {
            memory[*fetch_bytes(1)] = reg.X;
        } break;
        case INS_MOV_Y_M: {
            memory[*fetch_bytes(1)] = reg.Y;
        } break;
        case INS_MOV_A_MX: {
            memory[*fetch_bytes(1)+reg.X] = reg.A;
        } break;
        case INS_MOV_A_MY: {
            memory[ memory[*fetch_bytes(1)]+reg.Y ] = reg.A;
        } break;
        case INS_MOV_A_MXY: {
            memory[ memory[*fetch_bytes(1)+reg.X]+reg.Y ] = reg.A;
        } break;
            
    }
}

#endif