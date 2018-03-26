#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
  struct stat statBuffer;
  int n = 0;
  int nRtn = 1;
  char ss[2048];
  strcpy(ss,argv[0]);
  if(ss[0] == '/')
  {
    n = strlen(ss);
    while(ss[n] != '/') { --n; }
    ++n;
  }
  strcpy(&ss[n],"folder.sh");
  if(!stat(ss,&statBuffer))
  {
    char sCmd[2048];
    strcpy(sCmd,"/bin/bash \"");
    strcat(sCmd,ss);
    strcat(sCmd,"\"");
    nRtn = system(sCmd);
  }
  return nRtn;
}
