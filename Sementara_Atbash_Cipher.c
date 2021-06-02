#include <stdio.h>
#include <string.h>

void encodeAtbash(char *s) {
    // Encode string using Atbash Cipher
    for (int i = 0; i < strlen(s); i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'Z'-s[i]+'A';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'z'-s[i]+'a';
    }
}

void decodeAtbash(char *s) {
    for (int i = 0; s[i]; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'A'-s[i]+'Z';
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'a'-s[i]+'z';
    }
}

int main() {
    char str[] = "DATA_PENTING/kucinglucu123";
    // Encode and print
    encodeAtbash(str);
    puts(str);
    // Decode and print
    decodeAtbash(str);
    puts(str);
    return 0;
}
