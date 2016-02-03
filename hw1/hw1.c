#include <stdio.h>
#include <string.h>
#include <stdint.h>


int encrypt(uint32_t in, uint32_t key) {
    int out = in ^ key;
    return out;
}

int decrypt(uint32_t in, uint32_t key) {
    int out = in ^ key;
    return out;
}


int main ( int argc, char *argv[] )
{   

    //need 4 arguments -- encrypt, key, filein, fileout
    if ( argc < 4 ) 
    {
        printf( "Not enough arguments");

    } else {   

        uint32_t key = atoi(argv[2]);

        // Open the in and out files
        FILE *out;
        FILE *infile;

        if ((infile = fopen( argv[3], "r" )) == NULL) printf("ERROR: Could not open: %s\n",argv[2]);
        if ((out = fopen(argv[4], "wb")) == NULL) printf("ERROR: Could not open: %s\n",argv[3]);
 
        if (strcmp(argv[1], "encrypt") == 0) {
            uint32_t x;
            uint32_t wr;
            /* read one character at a time from file, stopping at EOF, which
               indicates the end of the file.  Note that the idiom of "assign
               to a variable, check the value" used below works because
               the assignment statement evaluates to the value assigned. */

            while  ( ( x = fgetc( infile ) ) != EOF ) {
                wr = encrypt(x, key);
                putc(wr, out);
            }

            fclose( infile );
            fclose( out );
        } else if (strcmp(argv[1], "decrypt") == 0) {
            uint32_t x;
            uint32_t wr;
            /* read one character at a time from file, stopping at EOF, which
               indicates the end of the file.  Note that the idiom of "assign
               to a variable, check the value" used below works because
               the assignment statement evaluates to the value assigned. */

            while  ( ( x = fgetc( infile ) ) != EOF ) {
                wr = decrypt(x, key);
                putc(wr, out);
            }

            fclose( infile );
            fclose( out );
        }
    }
}