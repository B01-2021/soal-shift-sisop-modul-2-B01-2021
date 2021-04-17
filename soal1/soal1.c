#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <wait.h>
#include <dirent.h>

void direktori(){
    char *argv[] = {"mkdir", "Pyoto", "Fylm", "Musyik", NULL};
    execv("/bin/mkdir", argv); 
}

void download(){
    int i, status[3];
    char *argv[3][200]={{"wget", "--no-check-certificate", "drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "-q", "-o", "/dev/null", "-O", "Musik_for_Stevany.zip", NULL},
                          {"wget", "--no-check-certificate", "drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "-q", "-o", "/dev/null", "-O", "Foto_for_Stevany.zip", NULL},
                          {"wget", "--no-check-certificate", "drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "-q", "-o", "/dev/null", "-O", "Film_for_Stevany.zip", NULL}};                      
    for(i=0; i<3; i++)
    {
        if (0==fork()){
            while ((wait(&status[i])) > 0);
            continue;
        }
        else        
            execve("/usr/bin/wget", &argv[i][0], NULL);
    }
}

void unzip(){
    char *argv2[3][100]={{"unzip", "-q", "Musik_for_Stevany.zip", NULL},
                         {"unzip", "-q", "Foto_for_Stevany.zip", NULL},
                         {"unzip", "-q", "Film_for_Stevany.zip", NULL}};
    int i, status[3];
    for(i=0; i<3; i++){
        if(0==fork()){
            while ((wait(&status[i])) > 0);
            continue;
        }
        else
            execve("/usr/bin/unzip", &argv2[i][0], NULL);              
    }
}

void move_foto(char *basePath){
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        char *sub;
        int status;

        sub = strstr(dp->d_name, ".jpg");
        if (sub && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, ".." ) != 0)
        {
            char file[20];
            // printf("%s\n", dp->d_name);

            strcpy(file, "FOTO/");
            strcat(file, dp->d_name);
            
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if(0==fork()){
                char *argv[]={"mv", file, "Pyoto", NULL};
                execv("/bin/mv", argv);
            }
            while ((wait(&status)) > 0);
            
        }
    }

    closedir(dir);
}

void move_musik(char *basePath){
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, ".." ) != 0)
        {
            char file[20];
            int status;
            // printf("%s\n", dp->d_name);

            strcpy(file, "MUSIK/");
            strcat(file, dp->d_name);
            
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if(0==fork())
            {
                char *argv[]={"mv", file, "Musyik", NULL};
                execv("/bin/mv", argv);
            }
            while ((wait(&status)) > 0);
        }
    }
    closedir(dir);
}

void move_film(char *basePath){
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, ".." ) != 0)
        {
            char file[20];
            int status;

            strcpy(file, "FILM/");
            strcat(file, dp->d_name);
            
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if(0==fork())
            {
                char *argv[]={"mv", file, "Fylm", NULL };
                execv("/bin/mv", argv);
            }
            while ((wait(&status)) > 0);
        }
    }
    closedir(dir);
}

void move()
{   
    
    int i, status[3];
    for(i=0; i<3; i++){
        if(0==fork()){
            while ((wait(&status[i])) > 0);
            continue;
        }
        else{
            if(i==0){
                move_foto("/home/bunga/modul-2/soal1/FOTO");
            }
            else if(i==1){
                move_musik("/home/bunga/modul-2/soal1/MUSIK");
            }
            else if(i==2){
                move_film("/home/bunga/modul-2/soal1/FILM");
            }
        }
                          
    }

}

void zip(){
    char *argv[] = {"zip", "-q", "-r", "Lopyu_Stevany.zip", "Fylm", "Musyik", "Pyoto", NULL};
    execv("/usr/bin/zip", argv);
}

void del(){
    char *argv[]={"rm", "-r", "FILM", "FOTO", "MUSIK", "Fylm", "Pyoto", "Musyik", NULL};
    execv("/bin/rm", argv);
}

int main(void)
{
    int status, status2, status3, done=0, done2=0, done3=0;
    struct tm *ptr, *ptr2;
    time_t lt, lt2;
 
    while(1){
        lt = time(NULL);
        ptr = localtime(&lt);
        
        // sleep(1);
        //printf("%d %d %d %d %d %d\n", ptr->tm_year, ptr->tm_mon, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
        if(!done && ptr->tm_year==121 && ptr->tm_mon==3 && ptr->tm_mday==9 && ptr->tm_hour==16 && ptr->tm_min==21 && ptr->tm_sec==59)
        {
            done=1;
            int child_id, child_id2, child_id3, child_id4, child_id5;
            child_id =fork();
            if(child_id==0){
                sleep(20);
                child_id2 = fork();
                if(child_id2==0){
                    child_id3=fork();
                    if(child_id3==0){
                        
                        // sleep(15);
                        while(1){
                            lt2 = time(NULL);
                            ptr2 = localtime(&lt2);
                            if(ptr2->tm_year==121 && ptr2->tm_mon==3 && ptr2->tm_mday==9 && ptr2->tm_hour==22 && ptr2->tm_min>=21 )
                                break;
                            // sleep(10);
                        }

                            // printf("%s", asctime(ptr));
                            
                                child_id4 = fork();
                                if(child_id4==0){
                                    while ((wait(&status)) > 0);
                                    sleep(15);
                                    child_id5 = fork();
                                    if(child_id5==0){
                                        sleep(15);
                                        while ((wait(&status)) > 0);
                                        del();
                                    }
                                    else{
                                        zip();
                                    }
                                }
                                else{
                                    move();
                                }
                            //}
                           
                       // }
                    }
                    else{
                        unzip();
                    }
                }
                else{
                    direktori();
                }
            }
            else{
                download();
            }
        }
        if(done==1){
            break;
        }
    }
    return 0;
}
