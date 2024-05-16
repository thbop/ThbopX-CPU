#include <stdio.h>
#include "include/cpu.h"



int main() {
    initialize();
    memory[0x22] = 0x45;
    execute( INS_MOV_IM_X );
    memory[0x24] = 0x70;
    execute( INS_MOV_X_M );

    printf("$%x\n", memory[0x70]);
    return 0;
}