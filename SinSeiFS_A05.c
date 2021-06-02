/*
    A05
    Sistem Operasi A 2021
    Praktikum Modul 4
 */
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdbool.h>

#define DEBUG_MODE

static const char *dirpath = "/home/ryan/Downloads";

/*
    Function to check if a path is decoded or not.
    True = Encoded
    False = Decoded
 */
bool isAtoZ(const char *path) {
    int len = strlen(path);
    for (int i = 0; i < len - 5 + 1; i++) {
        if (path[i] == 'A' && path[i+1] == 't' && path[i+2] == 'o' && path[i+3] == 'Z' && path[i+4] == '_') return true;
    }
    return false;
}

/*
    Procedure to encode string using Atbash Cipher.
 */
void encodeAtbash(char *s) {
    // Encode string using Atbash Cipher
    for (int i = 0; i < strlen(s); i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'Z'-s[i]+'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'z'-s[i]+'a';
    }
}

/*
    Procedure to decode Atbash Cipher string.
 */
void decodeAtbash(char *s) {
    // Decode Atbash Cipher string
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'A'-s[i]+'Z';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'a'-s[i]+'z';
    }
}

/*
    Procedure to log encoding activity.
 */
void logEncode(char *dir1, char *dir2) {
    char filePath[1024];
    sprintf(filePath, "%s/encode_log.txt", dirpath);
    FILE *fptr = fopen(filePath, "a");
    fprintf(fptr, "%s -> %s\n", dir1, dir2);
    fclose(fptr);
}

/*
    Procedure to get file name and extension.
 */
void getFileDetail(const char *completeFileName, char *name, char *ext) {
    int index = 0;
    int i = 0;
    while(completeFileName[i]) {
        if (completeFileName[i] == '.') {
            break;
        }
        name[index++] = completeFileName[i];
        i++;
    }
    name[index] = '\0';
    index = 0;
    while (completeFileName[i]) {
        ext[index++] = completeFileName[i];
        i++;
    }
    ext[index] = '\0';
}

/*
    Function to encode folder name.
    Return 0 : successful
 */
int encodeFolderName(const char *basePath, const char* folderName) {
    char encryptedName[1024];
    strcpy(encryptedName, folderName);
    encodeAtbash(encryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, encryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

/*
    Function to decode folder name.
    Return 0 : successful
 */
int decodeFolderName(const char *basePath, const char* folderName) {
    char decryptedName[1024];
    strcpy(decryptedName, folderName);
    decodeAtbash(decryptedName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, folderName);
    sprintf(t_path, "%s/%s", basePath, decryptedName);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

/*
    Function to encode file.
    Return 0 : successful
*/
int encodeFile(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    encodeAtbash(fileName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

/*
    Function to decode file.
    Return 0 : succesful
*/
int decodeFile(char *basePath, char *name) {
    char fileName[1024], ext[64];
    getFileDetail(name, fileName, ext);
    decodeAtbash(fileName);
    char f_path[1024], t_path[1024];
    sprintf(f_path, "%s/%s", basePath, name);
    sprintf(t_path, "%s/%s%s", basePath, fileName, ext);
    int res = rename(f_path, t_path);
    if (res == -1) return -errno;
    return 0;
}

/*
    Function to encode folder recursively.
    Return number of encoded file.
*/
int encodeFolderRecursively(char *basePath) {
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
            count += encodeFolderRecursively(path);
            encodeFolderName(basePath, dp->d_name);
        }
        else {
            // File
            if (encodeFile(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

/*
    Function to decode folder.
    Return number of decoded file.
*/
int decodeFolderRecursively(char *basePath) {
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
            count += decodeFolderRecursively(path);
            decodeFolderName(basePath, dp->d_name);
        }
        else {
            // File
            if (decodeFile(basePath, dp->d_name) == 0) count++;
        }
    }
    closedir(dir);
    return count;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    // Concat path
    sprintf(fpath, "%s%s", dirpath, path);
    res = lstat(fpath, stbuf);
    if (res == -1) return -errno;
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else sprintf(fpath, "%s%s", dirpath, path);

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if(filler(buf, de->d_name, &st, 0)) break;
    }
    closedir(dp);
    return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else sprintf(fpath, "%s%s", dirpath, path);

    int fd;
    int res;
    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static int xmp_rename(const char *from, const char *to, unsigned int flags) {
    int res;
    if (flags) return -EINVAL;
    char fpath[1000], tpath[1000];
    if (strcmp(from, "/") == 0) {
        from = dirpath;
        sprintf(fpath, "%s", from);
    }
    else sprintf(fpath, "%s%s", dirpath, from);
    if (strcmp(to, "/") == 0) {
        to = dirpath;
        sprintf(tpath, "%s", to);
    }
    else sprintf(tpath, "%s%s", dirpath, to);

    // Check if it is folder
    struct stat path_stat;
    stat(fpath, &path_stat);
    if (!S_ISREG(path_stat.st_mode)) {
        // If the folder is encoded and will be decoded
        if (isAtoZ(fpath) && !isAtoZ(tpath)) {
            #if defined DEBUG_MODE
            printf("[QAQ] Decoding %s.\n", fpath);
            #endif
            int count = decodeFolderRecursively(fpath);
            #if defined DEBUG_MODE
            printf("[QAQ] Total decoded file : %d\n", count);
            #endif
        }
        // Else if the folder is decoded and will be encoded
        else if (!isAtoZ(fpath) && isAtoZ(tpath)) {
            #if defined DEBUG_MODE
            printf("[QAQ] Encoding %s.\n", fpath);
            #endif
            logEncode(fpath, tpath);
            int count = encodeFolderRecursively(fpath);
            #if defined DEBUG_MODE
            printf("[QAQ] Total encoded file : %d\n", count);
            #endif
        }
    }

    res = rename(fpath, tpath);
    if (res == -1) return -errno;
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode) {
    int res;
    char fpath[1000];
    if (strcmp(path, "/") == 0) {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else sprintf(fpath, "%s%s", dirpath, path);
    if (isAtoZ(fpath))
        logEncode("", fpath);
    res = mkdir(fpath, mode);
    if (res == -1) return -errno;
    return 0;
}

static const struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
