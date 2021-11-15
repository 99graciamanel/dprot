#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>


char *join(char *s1, int size1, char *s2, int size2);
unsigned int gethexword32(const char *str);
void set_ctx(MD5_CTX *pctx, const char *digest, unsigned long nblocks);

int main() {
    unsigned char digest[16] = {};
    char* key = "79a2571c8073b4b37233d8e9074061e9";
    char* mess1 = "What about joining me tomorrow for dinner?";
    int block_len = 32; // message blocks: 32B, hash size: 16B

    char *key_mess1 = join(key, strlen(key), mess1, strlen(mess1));
    int num_of_blocks = strlen(key_mess1)/block_len + 1;
    //unsigned char padding0[] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    //                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x50};

    MD5_CTX context_digest;
    MD5_Init(&context_digest);
    //char *key_mess1_padding0 = join(key_mess1, strlen(key_mess1), padding0, 22);
    //MD5_Update(&context_digest, key_mess1_padding0, strlen(key_mess1)+22);
    //MD5_Update(&context_digest, padding0, 22);
    MD5_Update(&context_digest, key_mess1, strlen(key_mess1));
    MD5_Final(digest, &context_digest);

    for(int i = 0; i < 16; ++i)
        printf("%02x", (unsigned int)digest[i]);
    printf("\n");

    char* mess2 = "Oops, Sorry, I just remember that I have a meeting very soon in the morning.";
    unsigned char forgery_tag[16] = {};
    MD5_CTX context_forgery;
    MD5_Init(&context_forgery);
    set_ctx(&context_forgery, &digest, num_of_blocks);
    MD5_Update(&context_forgery, mess2, strlen(mess2));
    MD5_Final(forgery_tag, &context_forgery);
    for(int i = 0; i < 16; ++i)
        printf("%02x", (unsigned int)forgery_tag[i]);
    printf("\n");

    //unsigned char padding[] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //unsigned char padding[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    //                           0x02, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char padding[] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x50};
    //unsigned char padding[] = {0x50, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
    //unsigned char padding[] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //unsigned char padding[] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    char *key_mess1_padding = join(key_mess1, strlen(key_mess1), padding, 22);
    char *key_mess1_padding_mess2 = join(key_mess1_padding, strlen(key_mess1) + 22, mess2, strlen(mess2));
    unsigned char prove[16] = {};
    MD5_CTX context_prove;
    MD5_Init(&context_prove);
    MD5_Update(&context_prove, key_mess1_padding_mess2, 172);
    MD5_Final(prove, &context_prove);
    for(int i = 0; i < 16; ++i)
        printf("%02x", (unsigned int)prove[i]);
    printf("\n");

    return 0;
}

char *join(char *s1, int size1, char *s2, int size2) {
    char *result = malloc(size1 + size2 + 1);
    memcpy(result, s1, size1);
    memcpy(result+size1, s2, size2);
    return result;
}

// "12efc5ca" -> 0xcac5ef12
unsigned int gethexword32(const char *str) {
    return *(unsigned int *)str;
}

void set_ctx(MD5_CTX *pctx, const char *digest, unsigned long nblocks) {
    unsigned int *casted_ptr = (unsigned int *)digest;
    pctx->A = casted_ptr[0];
    pctx->B = casted_ptr[1];
    pctx->C = casted_ptr[2];
    pctx->D = casted_ptr[3];
    nblocks <<= 9; // converting into bits
    pctx->Nh = nblocks >> 32;
    //pctx->Nh = 592;
    pctx->Nl = nblocks & 0xFFFFFFFFul;
}