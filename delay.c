#include <stdio.h>
#include <unistd.h>

int main()
{
 char c;
 for(;;)
   {
     c=getc(stdin);
     if (c == EOF) return 0;
     usleep(32256); // 1/31 = 0.0322580
     printf("%c",c);
     fflush(stdout);
   }
 return 0;
}