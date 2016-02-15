#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "md5.h"

// input : ./prog key

unsigned int key;
int buf, n, infile, outfile;
MD5_CTX mdContext; 


lastbytes(int outfile, int size, int buf) // magic code for extracting last bytes of encryption without the padding
{ 
  int i = 0;
  char *last;
  last = (char*) &buf;
  for (i=0;i<size;i++) {write(outfile, &last[i], 1);} 
};

decrypt(int key)
{
  struct stat st;
  int size,fsize;
  int *temp, result;   
  int rollingkey;    
  rollingkey = key;   
 
  infile = open ("output", O_RDONLY);
  if (infile<0) { printf("input open error\n"); exit(0); }
  
  buf = 0;
  read(infile,&buf,4);
  size=buf; // get plaintext size

  

  // ciphertext has xtra 4 bytes (size) and padding 

  stat("output", &st); fsize = st.st_size; // get ciphertext size
  if ((fsize < 8)||(size>fsize)||(size<(fsize-8))) {printf("file size sanity check failed\n");}; 

  outfile = open ("output-dec", O_RDWR|O_CREAT|O_TRUNC, 0700);
  if (outfile<0) { printf("output open error\n"); exit(0); }
  
  int c = 0;

  while ((n = read(infile, &buf, 4))> 0) {

      if (c == 0) {
        char* ff = (char*) &buf;
        printf("\n%X.%X.%X.%X\n", ff[0], ff[1], ff[2], ff[3]);
      }

      buf = buf ^ rollingkey; // doing the reverse of encrypt

      if (c == 0) {
        char* ff = (char*) &buf;
        printf("\nafter xor:  %X.%X.%X.%X\n", ff[0], ff[1], ff[2], ff[3]);
      }

      c++;


      MD5Init(&mdContext);
      MD5Update(&mdContext, &rollingkey, 4);
      MD5Final(&mdContext);
      temp = (int *) &mdContext.digest[12]; 
      result = *temp; // result is 32 bits of MD5 of key
      rollingkey = rollingkey ^ result; // new key

      if (size >= 4) write(outfile, &buf, 4);  
      else lastbytes(outfile, size, buf);

      buf = 0;  // repeat, keep track of output size in size.
      size = size - 4;
  };
  close(infile);
  close(outfile);
};

main(int argc, char *argv[])
{
  int key;
  int check;
  int tfile;
  char* checker = (char*) &check;
  int status;
  int hacked_key = 0;
  int not_cracked = 1;
  char* hkey = (char*) &hacked_key;

  sscanf(argv[1], "%x", &key);

  while (not_cracked == 1) {

    //char* ff = (char*) &key;
    //printf("\nthe key is:  %X.%X.%X.%X\n", ff[0], ff[1], ff[2], ff[3]);

    decrypt (hacked_key);

    tfile = open("output-dec", O_RDONLY);
    read(tfile, &check, 4);
    //printf("first byte: %x \n", check);

    if (checker[0] == 0x25 && checker[1] == 0x50 && checker[2] == 0x44 && checker[3] == 0x46) {
      not_cracked = 0;
      printf("\n%X.%X.%X.%X\n", hkey[0], hkey[1], hkey[2], hkey[3]);
    }

    if (hacked_key%1000 == 0) {
      printf("\nkey is:%x\n", hacked_key);
    }

    hacked_key++;

    close(tfile);

  }
  // 
  //  printf("%x\n", key);
  
};
