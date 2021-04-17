#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <signal.h>

int running = 1;

void usageinfo()
{
    printf("Usage: ./soal3 [-z | -x]\n\n");
    printf("to determine program behavior on exit, use either:\n");
    printf("\t-z\t to terminate all processes on exit, or\n");
    printf("\t-x\t to terminate main program while letting operations in existing directories finish on exit\n");
}

void sighandler(int sig)
{
    running = 0;
}

void makescript(char str[])
{
    FILE* script = fopen("killer.sh", "w");
    fputs(str,script);
    fclose(script);
}

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        char script[100];
        if (strcmp(argv[1], "-z") == 0)
        {
            strcpy(script,
            "#!/bin/bash \n killall -9 ./soal3 \n rm $0 \n");
        }
        else if (strcmp(argv[1], "-x") == 0)
        {
            strcpy(script,
            "#!/bin/bash \n killall -15 ./soal3 \n rm $0 \n");
        }
        else
        {
            usageinfo();
            return 0;
        }
        makescript(script);
        signal(SIGTERM, sighandler);
    }
    else
    {
        usageinfo();
        return 0;
    }

    while (running)
    {
        char dname[100];
        char path[100];
        pid_t child_id;

        time_t dt = time(NULL);
        struct tm dir_time = *localtime(&dt);
        strftime(dname, sizeof(dname), "%Y-%m-%d_%T", &dir_time);

        child_id = fork();

        if (child_id < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (child_id == 0)
        {
            if (fork() == 0)
            {
                char* arg[] = {"mkdir", "-p", dname, NULL};
                execv("/usr/bin/mkdir", arg);
            }
        }
        else
        {
            int i;
            for (i = 0; i < 10; i++)
            {
                if (fork() == 0)
                {
                    sleep(5);
                    continue;
                }

                char fname[100];
                time_t ft = time(NULL);
                struct tm file_time = *localtime(&ft);
                strftime(fname, sizeof(fname),"%Y-%m-%d_%T", &file_time);

                strcpy(path, dname);
                strcat(path, "/");
                strcat(path, fname);
                // strcat(path, ".jpg");

                char url[100] = "https://picsum.photos/";
                int size = ((long)mktime(&file_time) % 1000) + 50;
                char sizestr[10];
                sprintf(sizestr, "%d", size);
                strcat(url, sizestr);

                char* arg[] = {"wget", "-q", "-O", path, url, NULL};
                execv("/usr/bin/wget", arg);
            }

            char msg[] = "Download Success";
            for (i = 0; i < strlen(msg); i++)
            {
                if ((msg[i] >= 'a' && msg[i] <= 'z') ||
                    (msg[i] >= 'A' && msg[i] <= 'Z'))
                {
                    msg[i] += 5;

                    if ((msg[i] > 'z') || (msg[i] > 'Z' && msg[i] < 'a'))
                    {
                        msg[i] -= 26;
                    }
                }
            }
            strcpy(path, dname);
            strcat(path, "/status.txt");
            FILE* txt = fopen(path, "w");
            fputs(msg, txt);
            fclose(txt);

            char* arg[] = {"zip", "-m", "-q", "-r", dname, dname, NULL};
            execv("/usr/bin/zip", arg);
        }

        sleep(40);
    }
}
