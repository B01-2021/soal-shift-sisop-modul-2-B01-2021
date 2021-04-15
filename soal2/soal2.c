#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int it = 0;
char animal[60][20];

void getAnimalName();

int main() 
{
  // pid_t pid, sid;
  
  // pid = fork();
  
  // if(pid < 0) 
  //   exit(EXIT_FAILURE);
  
  // if(pid > 0) 
  //   exit(EXIT_SUCCESS);
  
  // umask(0);
  
  // sid = setsid();
  // if(sid < 0) 
  //   exit(EXIT_FAILURE);
  
  // close(STDIN_FILENO);
  // close(STDOUT_FILENO);
  // close(STDERR_FILENO);

  // while (1) 
  // {
    int status;
    
    pid_t cid, cid2;
    cid = fork();

    if (cid < 0) 
      exit(EXIT_FAILURE); 

    if (cid == 0) 
    {
      
      cid2 = fork();
      if (cid2 < 0) 
        exit(EXIT_FAILURE); 
    
      if (cid2 == 0) 
      {
        //soal 2.a
        char *argv[] = {"unzip", "-j", "/root/sisop2021/modul2/pets.zip", "*.jpg", "-d", "/root/modul2/petshop", NULL};
        execv("/bin/unzip", argv);
      }
      else 
      {
        //soal 2.b
        getAnimalName();
        
        while ((wait(&status)) > 0);
        
        int i;
        for(i = 0; i < it; i++) 
        {
          char target[100];
          sprintf(target, "/root/modul2/petshop/%s", animal[i]);
          printf("%s %s\n", target, animal[i]);
          
          pid_t cid3 = fork();
          if(cid3 < 0) 
            exit(EXIT_FAILURE);
          
          if(cid3 == 0)
          {
            char *argv[] = {"mkdir", "-p", target, NULL};
            execv("/bin/mkdir", argv);
          }
        }
      }
    } 
    else 
    {
      while ((wait(&status)) > 0);
      printf("done\n");
    }
  // }
  
}

void getAnimalName() 
{
  DIR *dp;
  struct dirent *ep;
  char animal[60][20];
  int it = 0;

  dp = opendir ("/root/modul2/petshop");
  if (dp != NULL)
  {
    while (ep = readdir (dp)) 
    { 
      if(strstr(ep->d_name, "jpg")) 
      {
        char *pch, *pch2;
        char *rest = ep->d_name;
        int count = 0;
        while ((pch = strtok_r(rest, "_", &rest))) 
        {
          char* rest2 = pch;
          while ((pch2 = strtok_r(rest2, ";", &rest2))) 
          {
            if (count == 0 | count == 3) 
            {
              strcpy(animal[it], pch2);
              printf("%s\n", animal[it]);
              it++;// run(target);
            }
            count++;
          }
        }
      } 
    }
  }
  else
    perror ("Couldn't open the directory");
}