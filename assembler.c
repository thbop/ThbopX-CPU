#include <stdio.h>
#include <string.h>
#include "include/cpu.h"

#define BUFFERSIZE 256

typedef unsigned int u32;

typedef struct insinfo { // Use this info to determine opcode
    u8 argc;
    char args[3][8]; // ins arg1 arg2
    // "$70", "rX"  = $70,rX
    // "$70", "sXY" = $70,sXY
} insinfo;

void print_insinfo( insinfo ins ) {
    printf("C: %d INS: %s ARG1: %s ARG2: %s\n", ins.argc, ins.args[0], ins.args[1], ins.args[2]);
}

u8 str_compare( char* a, char* b ) {
    if ( strlen(a) != strlen(b) ) return 0;
    u8 equal = 1;
    for ( int i = 0; i < strlen(a); i++ ) {
        if ( a[i] != b[i] ) equal = 0;
    }
    return equal;
}

FILE* srcptr, *destptr;
char buffer[BUFFERSIZE];
u8 total_bytes = 0;

void putbyte( u8 byte ) {
    fputc(byte, destptr);
    total_bytes++;
    if ( total_bytes > 0x4E ) printf("WARNING: Max ROM limit surpassed! %d bytes over limit ($4E)!\n", total_bytes - 0x4E);

}

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
    if ( x == strlen(buffer) ) return --x;
    // x -= 2;
    return x;
}


int main() {
    srcptr  = fopen( "main.s",   "r"  );
    destptr = fopen( "main.out", "wb" );

    while( fgets( buffer, BUFFERSIZE, srcptr ) ) {
        // Get insinfo
        insert_end_space(buffer);
        u32 argc = count_char( buffer, ' ' );
        u32 arga, argb = 0;
        insinfo ins;
        u8 found_comment = 0;
        for ( u32 i = 0; i < argc; i++ ) {
            arga = argb;
            argb = separate_by_char( buffer, ' ', i );

            if ( !found_comment ) {
                if ( buffer[arga] == ';' ) {found_comment = 1;}
                else {
                    for ( u32 d = arga; d < argb; d++ ) if ( !(d-arga >= 3 || buffer[d] == ' ') ) ins.args[i][d-arga] = buffer[d];
                    ins.argc++;
                }
            }
        }

        if ( str_compare( "mov", ins.args[0] ) ) {
            printf("hi\n");
        }

    }

    fclose(srcptr);
    fclose(destptr);
    return 0;
}