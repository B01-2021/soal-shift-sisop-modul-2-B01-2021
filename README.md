# lapres-sisop-modul-2-B01-2021

Laporan resmi berisi dokumentasi soal shift Sisop Modul 2 (Proses dan Daemon).
---
Kelompok B01:
- [Shidqi Dhaifullah](https://github.com/shdkong1)	05111940000108
- [Fiqey Indriati Eka Sari](https://github.com/finesaaa)	05111940000015 
- [Bunga Fairuz Wijdan](https://github.com/bungafairuz)	05111940000030

---

### Rangkuman Jawaban 
- [Soal 1](https://github.com/B01-2021/soal-shift-sisop-modul-2-B01-2021/blob/main/soal1/soal1.c)
- [Soal 2](https://github.com/B01-2021/soal-shift-sisop-modul-2-B01-2021/blob/main/soal2/soal2.c)
- [Soal 3](https://github.com/B01-2021/soal-shift-sisop-modul-2-B01-2021/blob/main/soal3/soal3.c)
---


# Soal 1
Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi.

**Note:**
- Ulang Tahun Stevany : 09 April Pukul 22.22 WIB
- Semua poin dijalankan oleh 1 script di latar belakang, termasuk mendownload file zip-nya. Jadi cukup jalankan script 1x serta ubah time dan date untuk check hasilnya.
- Preview : https://drive.google.com/drive/folders/1NzRiPPoVlR_H8P51cxN4jaceeFQGk4un
- Tidak boleh menggunakan fungsi system(), mkdir(), dan rename() (Yang di bahasa C) .... FORBIDDENNN!!
- Tidak boleh pake cron !!!
- Menggunakan fork dan exec.
- Link
  - Foto  : https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD/view
  - Musik : https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J/view
  - Film  : https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp/view

## 1.a
(a) Dikarenakan Stevany sangat menyukai huruf Y, Steven ingin nama folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg 
```
void direktori(){
    char *argv[] = {"mkdir", "Pyoto", "Fylm", "Musyik", NULL};
    execv("/bin/mkdir", argv); 
}
```

## 1.b
(b) untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). 
```
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
```

## 1.c
(c) Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu meng-extract-nya setelah didownload serta 
```
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
```

## 1.d
(d) memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).
```
void move_foto(char *basePath){
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        char *sub;

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
            if(0==fork())
                move_foto(path);
            else{
                char *argv[]={"mv", "-q", file, "Pyoto", NULL};
                execv("/bin/mv", argv);
            }
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
            // printf("%s\n", dp->d_name);

            strcpy(file, "MUSIK/");
            strcat(file, dp->d_name);
            
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if(0==fork())
                move_foto(path);
            else{
                char *argv[]={"mv", "-q", file, "Musyik", NULL};
                execv("/bin/mv", argv);
            }
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
            // printf("%s\n", dp->d_name);

            strcpy(file, "FILM/");
            strcat(file, dp->d_name);
            
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if(0==fork())
                move_foto(path);
            else{
                char *argv[]={"mv", file, "Fylm", NULL };
                execv("/bin/mv", argv);
            }
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
```

## 1.e
(e) Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany). (f) Setelah itu pada waktu ulang tahunnya Stevany, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).
```
void zip(){
    char *argv[] = {"zip", "-q", "-r", "Lopyu_Stevany.zip", "Fylm", "Musyik", "Pyoto", NULL};
    execv("/usr/bin/zip", argv);
}

void del(){
    char *argv[]={"rm", "-r", "FILM", "FOTO", "MUSIK", "Fylm", "Pyoto", "Musyik", NULL};
    execv("/bin/rm", argv);
}
```

**Proses Daemon**
```
 pid_t pid, sid;        // Variabel untuk menyimpan PID

    pid = fork();     // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/home/bunga/modul-2/soal1/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
```

**Full Code:**
```
    pid_t pid, sid;        // Variabel untuk menyimpan PID

    pid = fork();     // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/home/bunga/modul-2/soal1/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int status, status2, status3, done=0, done2=0, done3=0;
    struct tm *ptr, *ptr2;
    time_t lt, lt2;

    while(1){
        lt = time(NULL);
        ptr = localtime(&lt);
        
        // sleep(1);
        //printf("%d %d %d %d %d %d\n", ptr->tm_year, ptr->tm_mon, ptr->tm_mday, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
        if(!done && ptr->tm_year==121 && ptr->tm_mon==3 && ptr->tm_mday==9 && ptr->tm_hour==16 && ptr->tm_min==21)
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
```

---


# Soal 2
Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

## 2.a
Pertama-tama program perlu mengextract zip yang diberikan ke dalam folder `“/home/[user]/modul2/petshop”`. Karena bos Loba teledor, dalam zip tersebut bisa berisi folder-folder yang tidak penting, maka program harus bisa membedakan file dan folder sehingga dapat memproses file yang seharusnya dikerjakan dan menghapus folder-folder yang tidak dibutuhkan.
**Code :**
```
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
  ...
```

## 2.b
Foto peliharaan perlu dikategorikan sesuai jenis peliharaan, maka kamu harus membuat folder untuk setiap jenis peliharaan yang ada dalam zip. Karena kamu tidak mungkin memeriksa satu-persatu, maka program harus membuatkan folder-folder yang dibutuhkan sesuai dengan isi zip. Contoh: Jenis peliharaan kucing akan disimpan dalam `“/petshop/cat”`, jenis peliharaan kura-kura akan disimpan dalam `“/petshop/turtle”`.
**Jawaban :** Untuk keperluan menjawab keseluruhan soal nomor 2, disini akan digunakan beberapa variabel global yang digunakan untuk menyimpan nama hewan, jenis, umur dengan menggunakan fungsi `GetAnimal()`.
Sebelumnya berikut adalah variabel yang diperlukan:
```
char type[60][20];
char utype[10][20];
char name[60][20];
float age[60];
char filename[60][100];
```
**Fungsi GetAnimal() :**
```
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
              }
              age[it] = atof(pch2);
            }
          }
        }
      }
    }
    closedir(dp);
  }
  else
    perror ("Couldn't open the directory");
}
```
**Code :**
```
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
      
      if(fork() == 0)
      {
        char *argv[] = {"mkdir", "-p", target, NULL};
        execv("/bin/mkdir", argv);
      }
    }
  }
  ...
```

## 2.c dan 2.d
(c) Setelah folder kategori berhasil dibuat, programmu akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan. Contoh: `“/petshop/cat/joni.jpg”`. 
(d) Karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai. Contoh: foto dengan nama `“dog;baro;1_cat;joni;2.jpg”` dipindah ke folder `“/petshop/cat/joni.jpg”` dan `“/petshop/dog/baro.jpg”`.

**Konsep Jawaban:** jika kembali melihat fungsi `GetAnimal()`, dapat dilihat variabel `count` dimana ini yang akan menyimpan posisi argumen dari nama file sehingga `count 1 sampai 3` adalah hewan pertama di foto itu dan `count 4-6` adalah hewan kedua.

**Code:**
```
  while ((wait(&status)) > 0);  

  cid4 = fork();

  if (cid4 < 0) 
  exit(EXIT_FAILURE); 
  if (cid4 == 0) 
  {
    int i;
    getAnimal();
    for(i = 0; i < it + 1; i++)
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
```

## 2.e
Di setiap folder buatlah sebuah file `"keterangan.txt"` yang berisi nama dan umur semua peliharaan dalam folder tersebut. Format harus sesuai contoh.

  ```
  nama : joni
  umur  : 3 tahun

  nama : miko
  umur  : 2 tahun
  ```

Dalam hal ini akan menggunakan fungsi `GetUniqueAnimal()` untuk mendapatkan jenis hewannya apa saja dari folder yang telah dibuat di nomor 2.a
**Fungsi GetUniqueAnimal():**
```
void getUniqueAnimal() 
{
  DIR *dp;
  struct dirent *ep;

  dp = opendir ("/root/modul2/petshop");
  if (dp != NULL)
  {
    while (ep = readdir (dp)) 
    { 
      if(!(strstr(ep->d_name, "."))) 
      {
        strcpy(utype[uit], ep->d_name);
        uit++;
      }
    }
  }
  else
    perror ("Couldn't open the directory");
}
```

**Code:**
```
  while ((wait(&status)) > 0);

  cid5 = fork();
  if (cid5 < 0) 
    exit(EXIT_FAILURE); 
  if (cid5 == 0) 
  {
    //soal 2e
    int i, j;
    getAnimal();
    getUniqueAnimal();

    for (j = 0; j < uit; j++)
    {
      char dir_name[100];
      char src[200];

      sprintf(dir_name, "/root/modul2/petshop/%s", utype[j]);
      chdir(dir_name);

      FILE *fpw;
      char str[100];
      fpw = fopen("keterangan.txt", "w");

      if (fpw== NULL)
      {
        puts("Issue in opening the Output file");
      }

      DIR *dp;
      struct dirent *ep;

      dp = opendir (".");
      if (dp != NULL)
      {
        while (ep = readdir (dp)) 
        { 
          if(!(strstr(ep->d_name, ".txt")) && strstr(ep->d_name, ".jpg"))
          {
            for(i = 0; i < it; i++)
            {
              char str[200], curr[20];
              if ((age[i+1]-(int)age[i+1]) > 0.0)
                sprintf(str, "nama : %s\numur : %.1f tahun\n\n", name[i+1], age[i+1]);
              else
                sprintf(str, "nama : %s\numur : %.0f tahun\n\n", name[i+1], age[i+1]);
    
              strcpy(curr, name[i+1]);
              strcat(curr, ".jpg");

              if(strcmp(curr, ep->d_name) == 0) 
              {
                fputs(str, fpw);
                break;
              }
            }
          }
        }
        closedir(dp);
      }
      else
        perror ("Couldn't open the directory");
    }
  }
  else
  {
    while ((wait(&status)) > 0);
  }
```

---


# Soal 3
Ranora adalah mahasiswa Teknik Informatika yang saat ini sedang menjalani magang di perusahan ternama yang bernama “FakeKos Corp.”, perusahaan yang bergerak dibidang keamanan data. Karena Ranora masih magang, maka beban tugasnya tidak sebesar beban tugas pekerja tetap perusahaan. Di hari pertama Ranora bekerja, pembimbing magang Ranora memberi tugas pertamanya untuk membuat sebuah program.

Note:
- Tidak boleh menggunakan system() dan mkdir()
- Program utama merupakan SEBUAH PROGRAM C
- Wajib memuat algoritma Caesar Cipher pada program utama yang dibuat

## 3.a
Ranora harus membuat sebuah program C yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].
**Code:**
```
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
...
```

## 3.b
Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.
**Code:**
```
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
```

## 3.c
Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Caesar Cipher adalah Teknik enkripsi sederhana yang dimana dapat melakukan enkripsi string sesuai dengan shift/key yang kita tentukan. Misal huruf “A” akan dienkripsi dengan shift 4 maka akan menjadi “E”. Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip saja.
**Code:**
```
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
```

## 3.d
Untuk mempermudah pengendalian program, pembimbing magang Ranora ingin program tersebut akan men-generate sebuah program “Killer” yang executable, dimana program tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan. Karena Ranora menyukai sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat nantinya harus merupakan program bash.
**Code:**
```
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
...
```

## 3.e
Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete direktori).
**Code:**
```
char* arg[] = {"zip", "-m", "-q", "-r", dname, dname, NULL};
execv("/usr/bin/zip", arg);
```

**Full Code:**
```
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
```