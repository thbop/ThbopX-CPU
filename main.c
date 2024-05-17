#include <stdio.h>
#include "include/cpu.h"



int main() {
    initialize( "main.out" );
    printf("EXECUTE:\n");
    while ( 1 ) {
        // printf( "%x\n", memory[reg.PC] );
        execute( memory[reg.PC] );
        if ( memory[reg.PC] == INS_BRK ) break;
    }
    // print_memory();

    print_registers();
    return 0;
}