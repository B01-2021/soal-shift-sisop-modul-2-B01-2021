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

void errormsg()
{
    printf("Usage: ./soal3 [-z | -x]\n\n");
    printf("to determine program behavior on exit, use either:\n");
    printf("\t-z\tto terminate all processes on exit, or\n");
    printf("\t-x\tto terminate main program while letting operations in existing directories finish on exit\n");

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
            errormsg();
            return 0;
        }
        makescript(script);
        signal(SIGTERM, sighandler);
    }
    else
    {
        errormsg();
        return 0;
    }

    while (running)
    {
        char dname[100];
        char fname[100];
        char path[100];
        struct tm timenow;
        pid_t child_id;

        time_t t = time(NULL);
        timenow = *localtime(&t);
        strftime(dname, sizeof(dname), "%Y-%m-%d_%T", &timenow);

        child_id = fork();

        if (child_id < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (child_id == 0)
        {
            if (fork() == 0)
            {
                char* arg[] = {"mkdir", dname, NULL};
                execv("/usr/bin/mkdir", arg);
            }

        }
        else
        {
            sleep(1);
            int i;
            for (i = 0; i < 10; i++)
            {
                if (fork() == 0)
                {
                    continue;
                }

                t = time(NULL);
                timenow = *localtime(&t);
                strftime(fname, sizeof(fname),"%Y-%m-%d_%T", &timenow);

                strcpy(path, dname);
                strcat(path, "/");
                strcat(path, fname);

                char url[100] = "https://picsum.photos/";
                int size = ((long)mktime(&timenow) % 1000) + 50;
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
                    if ((msg[i] + 5 > 'z') || (msg[i] + 5 > 'Z'))
                    {
                        msg[i] -= 26;
                    }

                    msg[i] += 5;
                }
            }

            char path[100];
            strcpy(path, dname);
            strcat(path, "/");
            strcat(path, "status.txt");
            FILE* txt = fopen(path, "w");
            fputs(msg, txt);
            fclose(txt);

            strcpy(path, dname);
            strcat(path, ".zip");
            char* arg[] = {"zip", "-mqr", path, dname};
            execv("usr/bin/zip", arg);
        }
        sleep(40);
    }
}
