#include "defs.h"

/* print the arguments -- most commonly used to print values of variables 
   actual variable substitution is handled elsewhere */
void print(char job[])
{
  int nread;
  char junk[MAXVAR], rest[MAXCOMM];

  nread = sscanf(job, "%s %[^\n]", junk, rest);
  if (nread == 2) printf("%s\n", rest);

}  
