#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main ()
{
  char animal[60][20];
  int it = 0;
 
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    int i;
    printf("%d\n", it);
    for(i = 0; i < it; i++) {
      char target[100];
      sprintf(target, "/root/modul2/petshop/%s", animal[i]);
      printf("%s %s\n", target, animal[i]);
      pid_t cid3 = fork();
      if(cid3 < 0) exit(0);
      if(cid3 == 0)
      {
        char *ag[] = {"mkdir", "-p", target, NULL};
        execv("/bin/mkdir", ag);
      }
    }
  } 
  while ((wait(&status)) > 0);
    
  return 0;
}
