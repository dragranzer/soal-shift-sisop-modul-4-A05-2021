# soal-shift-sisop-modul-4-A05-2021

## Soal 1
Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :

NOTE : 
Semua file yang berada pada direktori harus ter-encode menggunakan Atbash cipher(mirror).
Misalkan terdapat file bernama kucinglucu123.jpg pada direktori DATA_PENTING
“AtoZ_folder/DATA_PENTING/kucinglucu123.jpg” → “AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg”
Note : filesystem berfungsi normal layaknya linux pada umumnya, Mount source (root) filesystem adalah directory /home/[USER]/Downloads, dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di-encode.
Referensi : https://www.dcode.fr/atbash-cipher

### Poin a
Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

Strategi Penyelesaian:

Untuk mengecek apakah suatu direktori merupakan direktori ter-encode atau tidak, kami membuat fungsi `isAtoZ` yang mengembalikan value boolean. True jika direktori merupakan direktori ter-encode dan False jika direktori merupakan direktori ter-decode. Direktori yang ter-encode mempunyai substring "AtoZ" pada direktorinya.

Pseudocode:
```c
dir = "/home/user/Downloads/AtoZ_folder"
if isAtoZ(dir)
  // True
else
  // False
```

Direktori yang baru saja dibuat adalah kosong sehingga program tidak perlu melakukan apapun.

### Poin b
Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

Strategi Penyelesaian:

Ketika suatu direktori atau file di-rename, fungsi fuse `xmp_rename` akan terpanggil. Dengan menggunakan fungsi ini, kita dapat melakukan pengecekan apakah suatu direktori diubah menjadi direktori ter-encode atau ter-decode.

Pada fungsi `xmp_rename`, kami mendeklarasikan string `fpath` dan `tpath` yang merupakan direktori sebelum rename dan direktori sesudah rename. Dengan kata lain, nama folder lama dan nama folder baru. 

Jika `fpath` mempunyai substring "AtoZ_", maka `fpath` merupakan direktori ter-encode. Jika tidak, maka `fpath` merupakan direktori ter-decode. Untuk mengecek apakah `fpath` merupakan direktori ter-encode atau ter-decode, fungsi `isAtoZ(fpath)` digunakan.

Jika `tpath` mempunyai substring "AtoZ_", maka `tpath` merupakan direktori ter-encode. Jika tidak, maka `tpath` merupakan direktori ter-decode. Untuk mengecek apakah `tpath` merupakan direktori ter-encode atau ter-decode, fungsi `isAtoZ(tpath)` digunakan.

Untuk mengecek apakah suatu direktori sedang di-encode atau di-decode. Perbandingan return value dari fungsi `isAtoZ(fpath)` dan `isAtoZ(tpath)` digunakan. Misalkan:

```c
A = isAtoZ(fpath)
B = isAtoZ(tpath)
```

Jika A = true dan B = false, maka proses decode dilakukan, sedangkan jika A = false dan B = true, maka proses encode dilakukan.
```c
if A and !B
  decode()
else if !A and B
  encode()
```

### Poin c
Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

Strategi Penyelesaian:
Penjelasan terkait strategi penyelesaian poin c telah dijelaskan pada poin b.

### Poin d
Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]

Strategi Penyelesaian:

Untuk melakukan log, prosedur `logEncode` dibuat. Prosedur ini berfungsi untuk mencatat suatu kegiatan ke sebuah log. Prosedur ini terpanggil sebelum proses decode atau encode selesai.

```c
A = isAtoZ(fpath)
B = isAtoZ(tpath)
if A and !B
  logEncode(fpath, tpath)
  decode()
else if !A and B
  logEncode(fpath, tpath)
  encode()
```

Selain itu, prosedur `logEncode` dipanggil ketika sebuah folder dengan direktori yang mempunyai substring "AtoZ" dibuat. Untuk melakukan hal ini, fungsi fuse `xmp_mkdir` digunakan. Fungsi ini akan terpanggil ketika sebuah folder dibuat pada file system. Pada fungsi ini.

```c
xmp_mkdir(path, ...)
if isAtoZ(path)
  logEncode("", path)
```

### Poin e
Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

Strategi Penyelesaian:

Untuk melakukan encode pada suatu direktori secara rekursif hingga folder terdalam, fungsi-fungsi dari library `dirent.h` digunakan. Sebuah fungsi `encodeFolderRecursively` dan `decodeFolderRecursively` dibuat untuk melakukan encode dan decode pada suatu folder. Return value dari fungsi-fungsi tersebut adalah banyaknya file yang telah berhasil di encode/decode. Jika return value dari fungsi adalah nol, maka tidak ada file yang berhasil di decode/encode.

Di dalam fungsi `encodeFolderRecursively` dan `decodeFolderRecursively`, proses scan file dan folder di dalamnya dilakukan. Jika sebuah folder ditemukan, maka proses decode/encode dilanjutkan pada folder tersebut.

### Screenshot

Proses compile dan run program fuse.

![image](https://user-images.githubusercontent.com/8071604/121290374-b2b81400-c910-11eb-8904-9868a7cf450e.png)

![image](https://user-images.githubusercontent.com/8071604/121290429-cbc0c500-c910-11eb-87c3-67407fe07175.png)

Proses encode dan decode sebuah folder.

![image](https://user-images.githubusercontent.com/8071604/121290552-07f42580-c911-11eb-8cf4-e5bd5aef4a63.png)

## Soal 2

Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang dirancang oleh Sei

### Poin a

Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).

Strategi penyelesaian:

Untuk mengecek apakah nama directory dibuat dengan awalan “RX_[Nama]” atau tidak adalah dengan membuat fungsi `isRX` yang akan me return true jika namanya dibuat dengan awalan “RX_[Nama]” dan false jika tidak

```c
bool isRX(const char *path) {
    int len = strlen(path);
    for (int i = 0; i < len - 3 + 1; i++) {
        if (path[i] == 'R' && path[i+1] == 'X' && path[i+2] == '_') return true;
    }
    return false;
}
```

Dibuat juga fungsi enkripsi `Atbash` dan `ROT13` untuk encode dan decode
```c
void encodeAtbash(char *s) {
    // Encode string using Atbash Cipher
    for (int i = 0; i < strlen(s); i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'Z'-s[i]+'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'z'-s[i]+'a';
    }
}

void encodeROT13(char *s) {
    // Decode Atbash Cipher string
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = ((s[i]-'A'+13)%26)+'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = ((s[i]-'a'+13)%26)+'a';
    }
}

void decodeAtbash(char *s) {
    // Decode Atbash Cipher string
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'A'-s[i]+'Z';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'a'-s[i]+'z';
    }
}

void decodeROT13(char *s) {
    // Decode ROT13 Cipher string
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = ((s[i]-'A'-13)%26)+'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = ((s[i]-'a'-13)%26)+'a';
    }
}
```

### Poin b

Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).

Strategi Penyelesaian:

Tetap menggunakan fungsi `isRX` untuk mengidentifikasi apakah directory di-rename dengan awalan “RX_[Nama]” atau tidak dengan cara

```c
if(!isRX(fpath) && isRX(tpath)){
    encodeFolderRecursivelyRXrn(fpath, INF);
    logEncode(fpath, tpath);
}
```
dimana `fpath` adalah nama sebelum di rename dan `tpath` adalah nama setelah di rename.

Pada poin b juga ditambah fungsi enkripsi `Viginere` dengan key "SISOP" untuk melakukan encode menggunakan Atbash + Viginere.
```c
void encodeVig(char *s) {
    // Encode Viginere Cipher string
    char key[] = "SISOP";
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = ((s[i]-'A'+(key[i%((sizeof(key)-1))]-'A'))%26)+'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = ((s[i]-'a'+(key[i%((sizeof(key)-1))]-'A'))%26)+'a';
    }
}

```

Untuk fungsi `encodeFolderRecursivelyRXrn()` adalah fungsi yang digunakan untuk melakukan encode pada seluruh isi dari directory yang di-rename dengan awalan “RX_[Nama]”

```c
int encodeFolderRecursivelyRXrn(char *basePath, int depth) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            if (depth > 0) {
                count += encodeFolderRecursivelyRXrn(path, depth - 1);
                encodeFolderNameRXrn(basePath, dp->d_name);
            }
        }
        else {
            // File
            if (encodeFileRXrn(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}
```
terdapat fungsi `encodeFolderNameRXrn()` dan `encodeFileRXrn()` untuk melakukan encoding pada file dan folder dalam directory
```c
int encodeFileRXrn(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    encodeAtbash(fileName);
    encodeVig(fileName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int encodeFolderNameRXrn(const char *basePath, const char* folderName) {
    char encryptedName[512];
    strcpy(encryptedName, folderName);
    encodeAtbash(encryptedName);
    encodeVig(encryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, encryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}
```
### Poin c

Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.

Strategi Penyelesaian:

Dibutuhkan tambahan fungsi decode dari `Viginere`

```c
void decodeVig(char *s) {
    // Decode Viginere Cipher string
    char key[] = "SISOP";
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = ((s[i]-'A'-(key[i%((sizeof(key)-1))]-'A')+26)%26)+'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = ((s[i]-'a'-(key[i%((sizeof(key)-1))]-'A')+26)%26)+'a';
    }
}
```
Kemudian pada `xmp_rename` ditambahkan
```c
if(isRX(fpath) && !isRX(tpath)){
    decodeFolderRecursivelyRXrn(fpath, INF);
    logEncode(fpath, tpath);
}
```
untuk melakukan decode apabila nama awalnya memiliki awalan "RX_" dan nama akhirnya tidak, lalu terdapat fungsi `decodeFolderRecursivelyRXrn()` untuk melakukan decode secara rekursif
```c
int decodeFolderRecursivelyRXrn(char *basePath, int depth) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return 0;
    int count = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);

        struct stat path_stat;
        stat(path, &path_stat);
        if (!S_ISREG(path_stat.st_mode)) {
            // Folder
            if (depth > 0) {
                count += decodeFolderRecursivelyRXrn(path, INF);
                decodeFolderNameRXrn(basePath, dp->d_name);
            }
        }
        else {
            // File
            if (decodeFileRXrn(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}
```
didalam fungsi `decodeFolderRecursivelyRXrn()` terdapat `decodeFolderNameRXrn()` dan `decodeFileRXrn()` untuk melakukan decode file dan folder
```c
int decodeFileRXrn(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    decodeVig(fileName);
    decodeAtbash(fileName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

int decodeFolderNameRXrn(const char *basePath, const char* folderName) {
    char decryptedName[512];
    strcpy(decryptedName, folderName);
    decodeVig(decryptedName);
    decodeAtbash(decryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, decryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}
```


## Soal 3

Karena Sin masih super duper gabut akhirnya dia menambahkan sebuah fitur lagi pada filesystem mereka. 

### Poin a

Jika sebuah direktori dibuat dengan awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

Strategi Penyelesaian:

Untuk mengecek apakah direktori merupakan direktori spesial atau tidak, fungsi `isAisA` digunakan. Fungsi ini menerima parameter sebuah string yaitu direktori yang akan di cek. Fungsi ini mengembalikan nilai TRUE jika direktori mempunyai substring "A_is_a_" dan mengembalikan nilai FALSE jika direktori tidak mempunyai substring "A_is_a_".

Misalnya, 

```c
dir1 = "/home/ryan/Downloads/A_is_a_Photo/1.jpg"
dir2 = "/home/ryan/Downloads/Photo/2.jpg"
dir3 = "/home/ryan/Downloads/A_is_a_Photo/2019/1.jpg"

printf("%d %d %d", isAisA(dir1), isAisA(dir2), isAisA(dir3))

// Output: 1 0 1
```

Selain fungsi `isAisA`, ada fungsi `isAisA_Content(path)`. `path` merupakan string yang merujuk pada lokasi sebuah file sehingga fungsi ini bertujuan untuk mengecek apakah suatu file berada di dalam direktori spesial atau tidak. Fungsi ini mengembalikan nilai TRUE jika parent direktori pada `path` mempunyai substring "A_is_a_". Parent direktori saja yang di cek karena pada soal 3 poin d dijelaskan bahwa direktori spesial hanya berdampak pada file-file yang berada di dalamnya. Dalam kata lain, sifat direktori spesial tidak diturunkan kepada folder yang ada di dalamnya secara rekursif.

Misalnya,

```c
isAisA_Content("/home/ryan/Downloads/A_is_a_Photo/1.jpg") = True
isAisA_Content("/home/ryan/Downloads/A_is_a_Photo/2019/1.jpg") = False // Artinya, file ini bukan merupakan file yang berada di dalam direktori spesial
isAisA_Content("/home/ryan/Downloads/A_is_a_Photo") = False // A_is_a_Photo diasumsikan merupakan sebuah file tanpa ektensi yang berada di dalam folder Downloads sehingga file ini bukan merupakan file yang berada di dalam direktori spesial
isAisA_Content("/home/ryan/Downloads/A_is_a_Photo/A_is_a_Good/2.jpg") = True
isAisA_Content("/home/ryan/Downloads/A_is_a_Photo/Good/A_is_a_Best/t.jpg") = True
```

Direktori yang baru saja dibuat adalah direktori kosong sehingga tidak perlu dilakukan proses encode / decode.

### Poin b

Jika sebuah direktori di-rename dengan memberi awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

Strategi Penyelesaian:

Fungsi fuse `xmp_rename` akan terpanggil ketika sebuah direktori di-rename atau sebuah file di-rename. Dengan memanfaatkan fungsi `isAisA` yang telah dibuat pada poin a, kita dapat mengetahui apakah sebuah direktori di-rename dari direktori spesial menjadi direktori tidak spesial atau direktori tidak spesial menjadi direktori spesial dengan membandingan `isAisA(fpath)` dan `isAisA(tpath)`.

Misalnya,
```c
A = isAisA(fpath)
B = isAisA(tpath)

if A and !B
  // Substring A_is_a_ dihapus
else if !A and B
  // Substring A_is_a_ ditambahkan
```

### Poin c

Apabila direktori yang terenkripsi di-rename dengan menghapus “A_is_a_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.

Penjelasan terkait poin c telah diberikan pada strategi penyelesaian poin b.

### Poin d

Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ_” maupun “RX_” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive  “AtoZ_” dan “RX_” tetap berjalan pada subdirektori).

Strategi Penyelesaian:

Jika sebuah direktori X yang mempunyai awalan AtoZ_ atau RX_ di-rename menjadi direktori spesial (direktori dengan awalan A_is_a_), maka file-file yang berada di dalam direktori X tersebut akan di-decode. Akan tetapi, file-file yang berada di dalam folder yang berada di dalam direktori X tidak di-decode.

Untuk menyelesaikan persoalan ini, fungsi `xmp_rename` digunakan. Dengan memanfaatkan fungsi `isRX` dan `isAtoZ` yang telah dibuat, kita dapat mendeteksi kapan sebuah direktori RX_ di-rename menjadi direktori spesial atau kapan sebuah direktori AtoZ_ di-rename menjadi direktori spesial. Berikut ini contohnya

```c
// fpath adalah path old
// tpath adalah path new
Z1 = isAtoZ(fpath)
Z2 = isAtoZ(tpath)
R1 = isRX(fpath)
R2 = isRX(tpath)
S1 = isAisA(fpath)
S2 = isAisA(tpath)

if Z1 and S2
  // Ketika folder AtoZ_ di-rename menjadi folder spesial
else if R1 and S2
  // Ketika folder RX_ di-rename menjadi folder spesial
else if S1 and Z2
  // Ketika folder spesial di-rename menjadi folder AtoZ_
else if S1 and R2
  // Ketika folder spesial di-rename menjadi folder RX_
```

Ketika folder AtoZ_ di-rename menjadi folder spesial, maka panggil fungsi `decodeFolderRecursively(path, depth)` dengan depth = 0 agar proses decode hanya berlaku pada file yang berada di dalamnya. Dalam kata lain, tidak perlu melakukan decode terhadap file yang berada di dalam folder yang berada di dalam folder spesial.

Hal ini juga berlaku untuk folder RX_.

Ketika folder spesial di-rename menjadi folder AtoZ_, maka panggil fungsi `encodeFolderRecursively(path, depth)` dengan depth = 0 agar proses encode hanya berlaku pada file yang berada di dalamnya.

Hal ini juga berlaku untuk folder RX_.

### Poin e

Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase insensitive dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.


Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.


Strategi Penyelesaian:

Untuk poin 3, kami belum mengimplementasikannya. Kami belum menemukan cara memanipulasi nama-nama file yang ditampilkan ketika command `ls` pada terminal dijalankan. Hal ini seharusnya bisa diimplementasikan dengan metode rename, yaitu melakukan rename pada file setiap file baru dibuat atau dipindahkan ke dalam direktori spesial. 

### Screenshot

Proses rename sebuah folder AtoZ_ menjadi folder spesial.

![image](https://user-images.githubusercontent.com/8071604/121290740-5275a200-c911-11eb-842d-1e7519db53d1.png)

Folder di dalam folder sisop tetap terencode sedangkan file README.md telah terdecode.

Proses rename sebuah folder RX_ menjadi folder spesial.

![image](https://user-images.githubusercontent.com/8071604/121290928-a3859600-c911-11eb-8915-e23029d5619e.png)


## Soal 4

* ### a - file log
  * **Problem**
    Terdapat file `SinSeiFS.log` yang disimpan di `/home/[user]/SinSeiFS.log`. File ini digunakan untuk menyimpan log setiap `system call` dijalankan
  * **Solusi**
    * Membuat const file path agar mudah diakses
      ```c
      static const char *logpath = "/home/{user}/SinSeiFS.log";
      ```
    * Buat fungsi yang dapat dipanggil untuk logging
      ```c
      void logEncode(char *dir1, char *dir2) {
        char buff[1024], cmd[32];
        if (dir1[0] != '\0') {
          strcpy(cmd, "RENAME");
          sprintf(buff, "%s::%s", dir1, dir2);
          logInfo(cmd, buff);
        }
        else {
          strcpy(cmd, "CREATE");
          sprintf(buff, "%s", dir2);
          logWarn(cmd, buff);
        }
      }
      ```
    * Manfaatkan `fopen`
      ```c
      FILE *out = fopen(logpath, "a");
      ```
     
* ### b - level di log
  * **Problem**
    Terdapat 2 level log, `WARNING` dan `INFO`
  * **Solusi**
    Dijelaskan lebih detail di subbagian selanjutnya
* ### c - level `WARNING`
  * **Problem**
    Level `WARNING` digunakan untuk logging `syscall`: `rmdir` dan `unlink`
  * **Solusi**
    * Siapkan const jenis level
      ```c
      // const for WARNING log level
      static const char warn[] = "WARNING";
      ```
    * Buatlah fungsi yang dapat dipanggil oleh fungsi `logEncode` untuk logging level `WARNING`
      ```c
      /*
      function: logWarn
      add a WARNING level log
      @param command: type of called system call
      @param desc: additional information and parameters
      @return null
      */
      void logWarn(char *command, char *desc);
      ```
* ### d - level `INFO`
  * **Problem**
    Level `INFO` digunakan untuk logging `syscall` selain dari level `WARNING`
  * **Solusi**
    * Siapkan const jenis level
      ```c
      // const for INFO log level
      static const char info[] = "INFO";
      ```
    * Buatlah fungsi yang dapat dipanggil oleh fungsi `logEncode` untuk logging level `INFO`
      ```c
      /*
      function: logWarn
      add a WARNING level log
      @param command: type of called system call
      @param desc: additional information and parameters
      @return null
      */
      void logWarn(char *command, char *desc);
      ```
* ### e - format log
  * **Problem**
    Format log tertera
    ```bash
    [Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]

    Level : Level logging, 
    dd : 2 digit tanggal, 
    mm : 2 digit bulan, 
    yyyy : 4 digit tahun, 
    HH : 2 digit jam (format 24 Jam),
    MM : 2 digit menit, 
    SS : 2 digit detik, 
    CMD : System Call yang terpanggil, 
    DESC : informasi dan parameter tambahan

    INFO::28052021-10:00:00:CREATE::/test.txt
    INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt
    ```
  * **Solusi**
    Pada fungsi `logWarn` dan `logInfo` terdapat
    ```c
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);

    char currTime[TIME_SIZE];
    strftime(currTime, TIME_SIZE, "%d%m%Y-%H:%M:%S", lt);

    char log[LOG_SIZE];
    sprintf(log, "%s::%s:%s::%s", warn, currTime, command, desc);
    ```
    * Pertama-tama, digenerate localtime
    * Kemudian waktu di format sesuai dengan format yang diminta soal
    * Tiap line log disimpan dalam variabel `log` dengan `sprintf` sesuai format
