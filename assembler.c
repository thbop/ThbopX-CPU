#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/cpu.h"

#define BUFFERSIZE 256

typedef unsigned int u32;

typedef struct commaarg {
    char a[4], b[4];
} commaarg;

typedef struct insinfo { // Use this info to determine opcode
    u8 argc;
    commaarg args[3]; // ins arg1 arg2
    // "$70", "rX"  = $70,rX
    // "$70", "sXY" = $70,sXY
} insinfo;

void print_insinfo( insinfo ins ) {
    printf("C: %d INS: %s 1a: %s 1b: %s 2a: %s 2b: %s\n", ins.argc, ins.args[0].a, ins.args[1].a, ins.args[1].b, ins.args[2].a, ins.args[2].b);
}

void print_error( char* ins, char* reason ) {
    printf( "ERROR: Failure processing %s instruction! %s\n", ins, reason );
}

u8 str_compare( char* a, char* b ) {
    if ( strlen(a) != strlen(b) ) return 0;
    u8 equal = 1;
    for ( int i = 0; i < strlen(a); i++ ) {
        if ( a[i] != b[i] ) equal = 0;
    }
    return equal;
}

u8 atoh( char* a ) {
    return (u8)strtol(a, NULL, 16);
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

int find_w_lim( char* buffer, char chr, u32 lim ) {
    int x = 0;
    while ( buffer[x++] != chr && x < lim );
    if ( x == lim ) return -1;
    return --x;
}

u8 axy( char arg ) {
    switch (arg) {
        case 'A': return 0;
        case 'X': return 1;
        case 'Y': return 2;
    }
}

u8 xy( char arg ) {
    return ( arg == 'Y' && arg != 'X' );
}

void mov( insinfo ins ) {

    if ( ins.args[2].a[0] == 'r' ) { // Going into register
        u8 axyv = axy( ins.args[2].a[1] );
        u8 xyv  = xy ( ins.args[1].b[1] );
        if      ( ins.args[1].a[0] == '#' ) { putbyte( INS_MOV_IM_A+axyv ); putbyte( atoh(ins.args[1].a+1) ); }
        else if ( ins.args[1].a[0] == '$' ) {
            if      ( ins.args[1].b[0] == 0   ) { putbyte( INS_MOV_M_A +axyv ); putbyte( atoh(ins.args[1].a+1) ); }
            else if ( ins.args[1].b[0] == 'r' ) { putbyte( INS_MOV_MX_A+xyv  ); putbyte( atoh(ins.args[1].a+1) ); }
        }
    }
    else if ( ins.args[2].a[0] == '$' ) { // Going into memory
        u8 axyv = axy( ins.args[1].a[1] );
        if ( ins.args[1].a[0] == 'r' ) { putbyte( INS_MOV_A_M+axyv ); putbyte( atoh(ins.args[2].a+1) ); }
    }

    else print_error( "mov", "Incorrect formatting!" );
}


int main() {
    srcptr  = fopen( "main.s",   "r"  );
    destptr = fopen( "main.out", "wb" );

    while( fgets( buffer, BUFFERSIZE, srcptr ) ) {
        // Get insinfo
        insert_end_space(buffer);
        u32 argc = count_char( buffer, ' ' );
        u32 arga, argb = 0;
        insinfo ins = { 0, (commaarg){"",""}, (commaarg){"",""} };
        u8 found_comment = 0;
        for ( u32 i = 0; i < argc; i++ ) {
            arga = argb;
            argb = separate_by_char( buffer, ' ', i );

            if ( !found_comment ) {
                if ( buffer[arga] == ';' ) {found_comment = 1;}
                else {
                    for ( u32 d = arga; d < argb; d++ ) if ( !(d-arga >= 7 || buffer[d] == ' ') ) {
                        if ( argb-arga <= 4 ) {
                            ins.args[i].a[d-arga] = buffer[d];
                            ins.args[i].b[d-arga] = 0;
                        } else {
                            int commaloc = find_w_lim( buffer+arga, ',', argb-arga ); // bad, ik but idc
                            if      ( d-arga < commaloc ) ins.args[i].a[d-arga]            = buffer[d];
                            else if ( d-arga > commaloc ) ins.args[i].b[d-arga-commaloc-1] = buffer[d];
                        }
                    }
                    ins.argc++;
                }
            }
        }
        ins.args[0].a[3] = '\0';
        print_insinfo( ins );
        if      ( str_compare( "brk", ins.args[0].a ) ) putbyte( INS_BRK );
        else if ( str_compare( "mov", ins.args[0].a ) ) mov( ins );

    }
    fputc(EOF, destptr); fputc(EOF, destptr);
    fclose(srcptr);
    fclose(destptr);
    return 0;
}