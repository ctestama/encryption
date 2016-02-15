#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int IP_Table[32]={        //the permute table used when encryption
    15,6,19,20,28,11,27,16,
    0,14,22,25,4,17,30,9,
    1,7,23,13,31,26,2,8,
    18,12,29,5,21,10,3,24
};

uint32_t keys[4] = {0, 0, 0, 0};




//for use in bit scrambling
uint32_t swap(uint32_t* v, int i, const int j) {
  uint32_t t;
  t = v[i];
  v[i] = v[j];
  v[j] = t;

  return *v;

}




uint32_t encrypt(uint32_t in, uint32_t* keys) {

    uint32_t out = in;

    for (int i = 0; i < 4; i++) {

       int sone = (keys[i] % ((i + 1) * 17)) % 32; 
       int stwo = (keys[i] % ((i + 1) * 11)) % 32; 

       if (sone < 0) {
        sone = sone + (2*sone);
       }

       if (stwo < 0) {
        stwo = stwo + (2*stwo);
       }

       out = (uint32_t) swap(&out, IP_Table[sone], IP_Table[stwo]);
       out = out ^ keys[i];
        
    }

    return out;
}

uint32_t decrypt(uint32_t in, uint32_t* keys) {

    uint32_t out = in;

    for (int i = 3; i >= 0; i--) {

        int sone = (keys[i] % ( (i + 1) * 17)) % 32; 
        int stwo = (keys[i] % ( (i + 1)* 11)) % 32; 

        if (sone < 0) {
        sone = sone + (2*sone);
       }

       if (stwo < 0) {
        stwo = stwo + (2*stwo);
       }

        out = out ^ keys[i];
        out = (uint32_t) swap(&out, IP_Table[stwo], IP_Table[sone]);

    }

    return out;
}


void make_keys(uint32_t* keys, uint32_t key) {

    keys[0] = key;

    for (int i = 1; i < 4; i++) {
        keys[i] = keys[i-1] >> 4;
        keys[i] = keys[i-1] - keys[i];
    }
}


int main ( int argc, char *argv[] )
{   
    //need 4 arguments -- encrypt, key, filein, fileout
    if ( argc < 4 ) 
    {
        printf( "Not enough arguments");

    } else {   

        uint32_t key = (uint32_t) strtol(argv[2], NULL, 16);
        //atoi(argv[2]); 

        make_keys(keys, key);

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
                wr = encrypt(x, keys);
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
                wr = decrypt(x, keys);
                putc(wr, out);
            }

            fclose( infile );
            fclose( out );
        }
    }
}