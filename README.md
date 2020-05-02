## Angelita Titiandes Br. Silalahi ( 05111840000088 )
## Rasyid Ridlo W ( 05111840000135 )

  4. Log system:

  Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan. Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING. Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink. Sisanya, akan dicatat dengan level INFO. Format untuk logging yaitu:
  
  [LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]

LEVEL    : Level logging
yy       : Tahun dua digit
mm       : Bulan dua digit
dd       : Hari dua digit
HH       : Jam dua digit
MM       : Menit dua digit
SS       : Detik dua digit
CMD      : System call yang terpanggil
DESC     : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::)

Contoh format logging nantinya seperti:

INFO::200419-18:29:28::MKDIR::/iz1
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg

        void Generator(char *desc, char *path, int check, int check2, char *path2){
          char result[1024];
          char flag[10];
          FILE *demo;
          if(check==0) {
                 strcpy(flag,"WARNING");
          }
            else{
                  strcpy(flag,"INFO");
          }
            time_t t = time(NULL); 
            struct tm tm = *localtime(&t); //nampilin waktu sekarang
            if(check2==0) {
                  snprintf(result,1024,"%s::%02d%02d%02d%02d:%02d:%02d::%s::%s", flag, tm.tm_year%100, tm.tm_mon+1, tm.tm_mday, 
                  tm.tm_hour, tm.tm_min, tm.tm_sec, desc, path);
          }
            else{
                  snprintf(result,1024,"%s::%02d%02d%02d%02d:%02d:%02d::%s::%s::%s", flag, tm.tm_year%100, tm.tm_mon+1, tm.tm_mday,       
                  tm.tm_hour, tm.tm_min, tm.tm_sec, desc, path, path2);
          }
          demo=fopen("/home/ridlo28/fs.log", "a");
          fprintf(demo, "%s\n", result);
          fclose(demo);
          }
          
Penjelasan  :
-  if(check==0) {
                 strcpy(flag,"WARNING");
          }
            else{                                     = Kondisi untuk memabgi log menjadi 2 level yaitu WARNING dan INFO
                  strcpy(flag,"INFO");
          }
-  if(check2==0) {      = Pengondisian  penyesuaian loging dengan jenis sistem yang berjalan 
                  snprintf(result,1024,"%s::%02d%02d%02d%02d:%02d:%02d::%s::%s",                          
                  flag, tm.tm_year%100, tm.tm_mon+1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec, desc, path);
          }
            else{
                  snprintf(result,1024,"%s::%02d%02d%02d%02d:%02d:%02d::%s::%s::%s",                      
                  flag, tm.tm_year%100, tm.tm_mon+1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec, desc, path, path2);
          }
      
- demo=fopen("/home/ridlo28/fs.log", "a"); = untuk membuat file fs.log
- fprintf(demo, "%s\n", result);           = Untuk mencetak isi result pada file fs.log






