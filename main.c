#include <stdio.h>
#include "include/cpu.h"



int main() {
    initialize( "main.out" );
    while ( 1 ) {
        // printf( "%x\n", memory[reg.PC] );
        execute( memory[reg.PC] );
        if ( memory[reg.PC] == INS_BRK ) break;
    }

    printf("$%x\n", reg.A);
    return 0;
}