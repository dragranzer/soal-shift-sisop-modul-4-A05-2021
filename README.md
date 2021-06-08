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


