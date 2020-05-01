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





