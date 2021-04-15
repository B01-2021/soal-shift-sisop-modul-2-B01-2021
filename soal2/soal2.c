#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int it = 0;
char type[60][20];
char name[60][20];
float age[60];
char filename[60][100];

void getAnimal();

int main() 
{
  int status;
  
  pid_t cid1, cid2, cid3,  cid4;
  cid1 = fork();

  if (cid1 < 0) 
    exit(EXIT_FAILURE); 

  if (cid1 == 0) 
  { 
    if (fork() == 0) 
    {
      // soal 2.a
      char *argv[] = {"unzip", "-j", "/root/sisop2021/modul2/pets.zip", "*.jpg", "-d", "/root/modul2/petshop", NULL};
      execv("/bin/unzip", argv);
    }
    else 
    {
      while ((wait(&status)) > 0); 
      //soal 2.b
      cid2 = fork();

      if (cid2 < 0) 
      exit(EXIT_FAILURE); 
      if (cid2 == 0) 
      {
        getAnimal();
        
        int i;
        for(i = 0; i < it; i++) 
        {
          char target[100];
          sprintf(target, "/root/modul2/petshop/%s", type[i]);
          // printf("%s %s\n", target, type[i]);
          
          if(fork() == 0)
          {
            char *argv[] = {"mkdir", "-p", target, NULL};
            execv("/bin/mkdir", argv);
          }
        }
      }
      else 
      {
        //soal 2c 2d
        while ((wait(&status)) > 0);  

        cid4 = fork();

        if (cid4 < 0) 
        exit(EXIT_FAILURE); 
        if (cid4 == 0) 
        {
          int i;
          getAnimal();
          for(i = 0; i < it; i++)
          {
            if(fork() == 0)
            {
              char dir_name[100];
              char src[200];

              chdir("/root/modul2/petshop");
              sprintf(dir_name, "%s/%s.jpg", type[i], name[i+1]);
              sprintf(src, "%s", filename[i]);
              char s[100];
              char *argv[] = {"cp", src, dir_name, NULL};
              execv("/bin/cp", argv);
            }
          }
        }
      }
    }
  } 
  else 
  {
    while ((wait(&status)) > 0);
    if (fork() == 0) 
    {
      int i;
      getAnimal();

      FILE *fpw;
      char str[100];
      fpw = fopen("keterangan.txt", "w");

      if (fpw== NULL)
      {
        puts("Issue in opening the Output file");
      }
      for(i = 0; i < it; i++)
      {
        char str[200];
        sprintf(str, "nama : %s\numur : %.1f tahun\n\n", name[i+1], age[i+1]);
        fputs(str, fpw);
      }
    }
  }
}

void getAnimal() 
{
  DIR *dp;
  struct dirent *ep;

  dp = opendir ("/root/modul2/petshop");
  if (dp != NULL)
  {
    while (ep = readdir (dp)) 
    { 
      if(strstr(ep->d_name, "jpg")) 
      {
        char *pch, *pch2;
        char *rest = ep->d_name;
        char *dir = ep->d_name;
        
        strcpy(filename[it], dir);
        if(strstr(ep->d_name, "_")) strcpy(filename[it + 1], dir);
        
        int count = 0;
        while ((pch = strtok_r(rest, "_", &rest))) 
        {
          char* rest2 = pch;
          while ((pch2 = strtok_r(rest2, ";", &rest2))) 
          {
            count++;
            if (count == 1 | count == 4) 
            {
              strcpy(type[it], pch2);
              it++;
            }
            else if (count == 2 | count == 5) 
            {
              strcpy(name[it], pch2);
            }
            else if (count == 3 | count == 6) 
            {
              if(strstr(pch2, "jpg")) 
              {
                pch2[strlen(pch2) - 4] = 0;
                // puts(pch2);
              }
              age[it] = atof(pch2);
            }
          }
        }
      } 
    }
  }
  else
    perror ("Couldn't open the directory");
}
