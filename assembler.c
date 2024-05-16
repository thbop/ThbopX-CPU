#include <stdio.h>
#include <string.h>
#include "include/cpu.h"

#define BUFFERSIZE 512

typedef unsigned int u32;

typedef struct insinfo {
    u8 opcode, argc;
} insinfo;

FILE* srcptr, destptr;
char buffer[BUFFERSIZE];

void insert_end_space( char* buffer ) {
    u32 len = strlen(buffer);
    buffer[len]   = ' ';
    buffer[len+1] = '\0';
}

u32 count_char( char* buffer, char chr ) {
    u32 x = 0;
    for ( u32 i = 0; i < strlen(buffer); i++ ) if ( buffer[i] == chr ) x++;
    return x;
}

u32 separate_by_char( char* buffer, char chr, int arg ) {
    u32 x = 0;
    while ( arg-- >= 0 ) while ( buffer[x++] != chr );
    x -= 2;
    return x;
}

int main() {
    srcptr = fopen( "main.s", "r" );

    while( fgets( buffer, BUFFERSIZE, srcptr ) ) {
        insert_end_space(buffer);
        u32 argc = count_char( buffer, ' ' );
        for ( u32 i = 0; i < argc; i++ ) {
            u32 argx = separate_by_char( buffer, ' ', i );
            printf( "%d\n", argx );
        }
    }

    fclose(srcptr);
    return 0;
}