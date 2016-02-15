#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#define bool int       //define bool type
#define false 0
#define true 1

const char IP_Table[32]={        //the permute table used when encryption
    15,6,19,20,28,11,27,16,
    0,14,22,25,4,17,30,9,
    1,7,23,13,31,26,2,8,
    18,12,29,5,21,10,3,24
};

const char IP_Tabletwo[32]={     //the permute table used when decryption
    8,16,22,30,12,27,1,17,
    23,15,29,5,25,19,9,0,
    7,13,24,2,3,28,10,18,
    31,11,21,6,4,26,14,20
};

int encrypt(uint32_t in, uint32_t key) {
    int out = in ^ key;
    return out;
}

int decrypt(uint32_t in, uint32_t key) {
    int out = in ^ key;
    return out;
}

void BitsCopy(bool *DataOut,bool *DataIn,int Len)     
//copy data from one array to another
{
    int i=0;
    for(i=0;i<Len;i++)
    {
        DataOut[i]=DataIn[i];
    }
}

void TablePermute(bool *DataOut,bool *DataIn,const char *Table,int Num)
//permute action
{
    int i=0;
    static bool Temp[256]={0};
    for(i=0;i<Num;i++)
     {
         Temp[i]=DataIn[Table[i]];
     }
     BitsCopy(DataOut,Temp,Num);
}

void ByteToBit(bool *DatOut,char *DatIn,int Num)
//the function that change Byte into Bit data
{
     int i=0;
     for(i=0;i<Num;i++)
     {
         DatOut[i]=(DatIn[i/8]>>(i%8))&0x01; 
     }
}

void BitToByte(char *DatOut,bool *DatIn,int Num)
//the function that change Bit into Byte data
{
    int i=0;
    for(i=0;i<(Num/8);i++)
    {
        DatOut[i]=0; 
    }
    for(i=0;i<Num;i++)
    {
     DatOut[i/8]|=DatIn[i]<<(i%8);
    }
}


void PlayPermute(char MesOut[4],char MesIn[4])
//combine several functions(for encrytion)
{
    static bool MesBit[32]={0};
    ByteToBit(MesBit,MesIn,32);
    TablePermute(MesBit,MesBit,IP_Table,32);
    BitToByte(MesOut,MesBit,32);
}

void Recover(char MesOut[4],char MesIn[4])
//combine several functions(for decryption)
{
     static bool MesBit[32]={0};
     ByteToBit(MesBit,MesIn,32);
     TablePermute(MesBit,MesBit,IP_Tabletwo,32);
     BitToByte(MesOut,MesBit,32);
}

int main ( int argc, char *argv[] )
{   
    int i=0;
    char middlestring[4]={0};
    char MesHex[4]={0};
    //need two array to put in the values
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
                middlestring[i]=wr;
                i++;
                if (i==3) {
                  i = 0;
                }
                //putc(wr, out);
                   
            }
            PlayPermute(MesHex,middlestring);
            for(i=0;i<4;i++)
            {
               putc(MesHex[i],out); 
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
                //wr = decrypt(x, key);
                middlestring[i]=x;
                i++;
                if (i == 3) {
                  i = 0;
                }
                //putc(wr, out);
            }
            Recover(MesHex,middlestring);
            for(i=0;i<4;i++)
            {  
               wr = decrypt(MesHex[i], key);
               putc(wr,out); 
            }
            fclose( infile );
            fclose( out );
        }
    }
}
