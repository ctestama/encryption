//char a[]={'E','N','E','N'};  //here is the line you need to change  ....this is the table password you find manually.
//if(result==0x3974cffc)    //here is the line you need to change  ....here, change the value the same as the destination hash in the search2.c
//both line in main.


#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "md5.h" 

// input : ./prog filename

char pass[4]; // 4 character password
int *pass_pointer, *temp;
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
reduce(int a)
{
   int change=0;
   if (a<0x30)
   {change=a+0x30;
   if ((change>=58)&&(change<=64))
   change=change+10;
   if ((change>=91)&&(change<=96))
   change=change-10;
   }
   if ((a>=0x30)&&(a<58))
   change=a;
   if ((a>=58)&&(a<=64))
   change=a+10;
   if ((a>64)&&(a<91))
   change=a;
   if ((a>=91)&&(a<=96))
   change=a-10;
   if ((a>96)&&(a<123))
   change=a;
   if ((a>=123)&&(a<=244))
   {change=a/2;
     if ((change>=58)&&(change<=64))
     change=change+10;
     if ((change>=91)&&(change<=96))
   change=change-10;
   }
   if (a>244)
   {change=(a-12)/2;
     if ((change>=58)&&(change<=64))
     change=change+10;
     if ((change>=91)&&(change<=96))
     change=change-10;
   }
   //printf("%x\n", change);
   //printf("%c\n", change);
   return change;
}

main(int argc, char *argv[])
{  
   FILE *fp;
   fp=fopen("table1.txt","a+");
   char b[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9'};
   char a[]={'E','N','E','N'};  //here is the line you need to change,1111111
   int i=0,j=0;
   
   for(j=0;j<100000;j++)
   {
   pass_pointer=a;
   MD5Init(&mdContext);  // compute MD5 of password
   MD5Update(&mdContext, pass_pointer, 4);
   MD5Final(&mdContext);
   temp = (int *) &mdContext.digest[12]; 
   result = *temp; // result is 32 bits of MD5 -- there is a BUG here, oh well.

   //printf("%x\n", result); // print a human readable version of hash (using hex conversion)

   if(result==0x3974cffc)    //here is the line you need to change,22222222
   {
       printf("%s\n", a);
       printf("it succeed\n");
       break;
   }
   fprintf(fp,"%x\n", result);
   byte[3]=result &0xff;
   byte[2]=(result & 0xff00) >>8;
   byte[1]=(result &0xff0000) >>16;
   byte[0]=(result & 0xff000000) >>24;
   //printf("%x\n", byte[0]);
   //printf("%x\n", byte[1]);
   //printf("%x\n", byte[2]);
   //printf("%x\n", byte[3]);
   for (i=0;i<4;i++) 
   {
      a[i]=reduce(byte[i]);
   }
   //printf("%c\n",a);
   pass_pointer=a;
   
   }
   printf("%d\n", j);
   printf("%x\n", result);
   printf("it failed");
   //printf("%x\n", result);
   //fprintf(fp,"%x\n", result);
   //fclose(fp);

};

