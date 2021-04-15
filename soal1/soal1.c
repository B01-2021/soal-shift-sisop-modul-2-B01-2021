#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); 
  }

  if (child_id == 0) {
    char *argv[] = {"mkdir", "-p", "Musyik", "Fylm",  "Pyoto", NULL};
    execv("/bin/mkdir", argv);
  } else {
    while ((wait(&status)) > 0);
    char *argv[] = {"wget", "--no-check-certificate","https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "-O", "Foto_for_Stevany.zip", NULL};
    execv("/usr/bin/wget", argv);
  }
}