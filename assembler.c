#include <stdio.h>
#include <string.h>
#include "include/cpu.h"

#define BUFFERSIZE 512

FILE* srcptr, destptr;
char buffer[BUFFERSIZE];

int separate_by_spaces( char* buffer, int arg ) {
    int x = 0;
    while ( arg-- >= 0 ) while ( buffer[x++] != ' ' );
    x -= 2;
    return x;
}

int main() {
    srcptr = fopen( "main.s", "r" );

    while( fgets( buffer, BUFFERSIZE, srcptr ) ) {
        int x = separate_by_spaces( buffer, 4 );
        printf( "%c\n", buffer[x] );
    }

    fclose(srcptr);
    return 0;
}