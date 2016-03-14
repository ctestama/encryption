//this file is used to search for certain hash value, if after chain reactions, finally it find the value in the rainbow table.it return ok and the hash value in the table, manually find the password and start using the aftersearch2 file to find the final value we want.
// in main, int result=0x19fbc7c1; //here is the line you need to change.
// change to the hash value you want search for. 

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "md5.h" 
#define stringSize(string) (sizeof(string) * sizeof(char))

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

int contains(char * string, int stringSize, FILE * fp) {
    int i = 0, j, end,value=0;
    char * part = (char *)calloc(stringSize, sizeof(char));
    fseek(fp, 0L, SEEK_END);
    end = ftell(fp) - stringSize + 2;

    while(i < end) {
        j = 0;
        fseek(fp, (long)i++, SEEK_SET);
        fgets(part, stringSize, fp);
        while(*part) {
            if(*string == *part) {
                j++;
                string++;
                part++;
                continue;
            }
            break;
        }
        if(j == stringSize - 1) {
            printf("OK\n");
            value=100;
            return value;
        } else {
            string -= j;
            part -= j;
        }
    }
return value;
    //free(part);
}


char* itoa(int num,char*str,int radix)
{/*索引表*/
//int elsek;
char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
unsigned unum;/*中间变量*/
int i=0,j,k;
/*确定unum的值*/
if(radix==10&&num<0)/*十进制负数*/
{
unum=(unsigned)-num;
str[i++]='-';
}
else unum=(unsigned)num;/*其他情况*/
/*转换*/
do{
str[i++]=index[unum%(unsigned)radix];
unum/=radix;
}while(unum);
str[i]='\0';
/*逆序*/
if(str[0]=='-')k=1;/*十进制负数*/
//elsek=0;
char temp;
for(j=k;j<=(i-1)/2;j++)
{
temp=str[j];
str[j]=str[i-1+k-j];
str[i-1+k-j]=temp;
}
return str;
}


main(int argc, char *argv[])
{  
   FILE *fp,*f;
   char * fileName = "table4.txt";
   int result=0x19fbc7c1; //here is the line you need to change.
   char str[8]={' ',' ',' ',' ',' ',' ',' ',' '};
   
   //printf("%s\n",str);
   
   f=fopen("table2.txt","a+");
   
   int i=0,j=0;
   
   fp=fopen(fileName,"a+");
   for(j=0;j<10000;j++)
   {
   str[7]=' ';
   itoa(result,str,16);
   if(contains(str, stringSize(str), fp)==100)  
   {printf("%d\n",j);
    printf("find the one");
    printf("%x\n", result);
    //fprintf(f,"%x\n", result);
    }
   
   //printf("%x\n", result); // print a human readable version of hash (using hex conversion)
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
      byte2[i]=reduce(byte[i]);
   }
   //printf("%c\n",byte2);
   pass_pointer=byte2;
   MD5Init(&mdContext);  // compute MD5 of password
   MD5Update(&mdContext, pass_pointer, 4);
   MD5Final(&mdContext);
   temp = (int *) &mdContext.digest[12]; 
   result = *temp; // result is 32 bits of MD5 -- there is a BUG here, oh well.
   //
   fprintf(f,"%x\n", result);
   //printf("%d\n", j);
   }
   //printf("%x\n", result);
   //
   fclose(fp);

printf("it finished\n");

};

