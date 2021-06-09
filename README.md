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

...

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
