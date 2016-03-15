//this file is used to create rainbow table, the created file name table4.txt.
//my uploaded table4.txt is the already output.
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "md5.h" 

// input : ./prog filename

char pass[4]; // 4 character password
int *temp;
char* pass_pointer;
int n, result, *temp;
int byte[]={' ',' ',' ',' '};
char byte2[]={' ',' ',' ',' '};
MD5_CTX mdContext;  // needed to compute MD5

check_pw(char * pass)
{
  int i = 0;
  for (i=0;i<4;i++) { 
       if (!(((pass[i] >= 'a') && (pass [i] <= 'z'))
             || ((pass[i] >= 'A') && (pass [i] <= 'Z'))
             || ((pass[i] >= '0') && (pass [i] <= '9')))) {
                   printf("Password not as per specifications\n");
                   exit(0);
       };
  };
};


char* reduce(int byte[]) {
   char res[]={' ',' ',' ',' '};
   int temp = 0;

   for (int k=0; k < 4; k++) {
      while ( ( byte[k] < 48 || byte[k] > 57 ) && ( byte[k] < 65 || byte[k] > 91 ) && ( byte[k] < 97 || byte[k] > 122) ) {
        byte[k] = (byte[k] + 17) % 128;
      }
      
      res[k] = (char) byte[k];
   }
   
   return &res;
}

void findPass(char* tstring, int hash) {

  
  char a[4] = {' ', ' ', ' ', ' '};

  for(int j=0;j<5000;j++) {

     char* ppoint= (char*) tstring;

     printf("now trying %s\n", tstring);
     
     MD5Init(&mdContext);  // compute MD5 of password
     MD5Update(&mdContext, ppoint, 4);
     MD5Final(&mdContext);
     temp = (int *) &mdContext.digest[12]; 
     result = *temp; // result is 32 bits of MD5 -- there is a BUG here, oh well.

     printf("the original hash: %d\n", hash);
     printf("new hash:%d\n", result);

     if (result == hash) {
        printf("The Password is: %s\n", tstring);
        exit(0);
     }

     //printf("%x\n", result); // print a human readable version of hash (using hex conversion)

     byte[3]=result & 0xff;
     byte[2]=(result & 0xff00) >>8;
     byte[1]=(result & 0xff0000) >>16;
     byte[0]=(result & 0xff000000) >>24;

     memcpy(a, reduce(byte), 4);

     tstring[0] = a[0];
     tstring[1] = a[1];
     tstring[2] = a[2];
     tstring[3] = a[3];
     tstring[4] = '\0';
   }
}

main(int argc, char *argv[])
{  
   FILE *fp;
   fp=fopen("table4.txt","a+");
   char b[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
   'p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J',
   'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4',
   '5','6','7','8','9'};

   char a[4]={' ',' ',' ',' '};
   int i=0,j=0;
   int k=0,l=0,h=0;

  if (argc < 2) { 
     for(k=0;k<62;k++) {
        for(l=0;l<62;l++) {

             fp=fopen("table4.txt","a+");
                    a[0]=b[k];
                    a[1]=b[l];
                    a[2]=b[k];
                    a[3]=b[l];
           fprintf(fp,"%c", a[0]);
           fprintf(fp,"%c", a[1]);
           fprintf(fp,"%c", a[2]);
           fprintf(fp,"%c ", a[3]);
     
           for(j=0;j<50;j++) {

             pass_pointer=a;
             MD5Init(&mdContext);  // compute MD5 of password
             MD5Update(&mdContext, pass_pointer, 4);
             MD5Final(&mdContext);
             temp = (int *) &mdContext.digest[12]; 
             result = *temp; // result is 32 bits of MD5 -- there is a BUG here, oh well.

             //printf("%x\n", result); // print a human readable version of hash (using hex conversion)

             byte[3]=result & 0xff;
             byte[2]=(result & 0xff00) >>8;
             byte[1]=(result & 0xff0000) >>16;
             byte[0]=(result & 0xff000000) >>24;

             memcpy(a, reduce(byte), 4);
           }

       fprintf(fp,"%c%c%c%c\n", a[0],a[1],a[2],a[3]);

       fclose(fp);
     }
    }
  } else {
    char* hexstring = (char* ) argv[1];
    char temp[512];
    
    int number = (int)strtol(hexstring, NULL, 16);
    printf("the original when passed: %d\n", number);
    int original = number;
    int notfound = 0;
    char tstring[4];
    int *tempe;

    int counter = 0;

    while (notfound == 0 && counter < 50) {
      FILE *fp;
      fp=fopen("table4.txt","a+");

      byte[3]=number & 0xff;
      byte[2]=(number & 0xff00) >>8;
      byte[1]=(number & 0xff0000) >>16;
      byte[0]=(number & 0xff000000) >>24;

      memcpy(a, reduce(byte), 4);
      
      tstring[0] = a[0];
      tstring[1] = a[1];
      tstring[2] = a[2];
      tstring[3] = a[3];
      tstring[4] = '\0';

      printf("%d : size of tstring\n", sizeof(tstring));

      printf("tstring is this: %s\n", tstring);

      int line_num = 0;
      int find_result = 0;

      while(fgets(temp, 512, fp) != NULL) {

        if((strstr(temp, tstring)) != NULL) {
          notfound = 1;
          printf("A match found on line: %d\n", line_num);
          printf("\n%s\n", temp);
          printf("\n%s\n", tstring);

          tstring[0] = temp[0];
          tstring[1] = temp[1];
          tstring[2] = temp[2];
          tstring[3] = temp[3];
          tstring[4] = '\0';

          findPass(tstring, original);
          find_result++;
        }

        line_num++;
      }
      
      //if we didn't find it, hash, reduce and then search again
      if (notfound == 0) {
        //int newpass = (int) tstring;
        pass_pointer = (char*) tstring;

        MD5Init(&mdContext);  // compute MD5 of password
        MD5Update(&mdContext, pass_pointer, 4);
        MD5Final(&mdContext);
        tempe = (int *) &mdContext.digest[12]; 
        number = *tempe; 
        printf("%x\n", number);
      }

      counter++;
    }
    //int result=0x19fbc7c1; //here is the line you need to change.

  }

};

