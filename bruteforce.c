
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "md5.h" 

// input : ./prog filename

char pass[4]; // 4 character password
int *pass_pointer, *temp;
int n, result, *temp;
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

main()
{  int i,j,k,l;
   //long int c;
   //scanf("%x",&c);
   //printf("%x",c);
   char b[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9'};
   char a[]={' ',' ',' ',' '};
   (char*) pass_pointer;
   pass_pointer=malloc(sizeof(a));
   for(i=0;i<64;i++)
   {
      for(j=0;j<64;j++)
      {
         for(k=0;k<64;k++)
            {
                for(l=0;l<64;l++)
                 {
                  a[0]=b[i];
                  a[1]=b[j];
                  a[2]=b[k];
                  a[3]=b[l];
                  pass_pointer=a;

   MD5Init(&mdContext);  // compute MD5 of password
   MD5Update(&mdContext, pass_pointer, 4);
   MD5Final(&mdContext);
   temp = (int *) &mdContext.digest[12]; 
   result = *temp; // result is 32 bits of MD5 -- there is a BUG here, oh well.
   //printf("%x\n",result);
   //printf("%x\n", result); // print a human readable version of hash (using hex conversion)
   //if(result==00011001111110111100011111000001)
   //if(result==0xcce27119)
if(result==0x8984570)
{
   printf("%s\n",a);
}
                 }

            }

      }

   }
   

 
};

