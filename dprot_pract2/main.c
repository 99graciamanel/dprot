#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <malloc.h>


unsigned char *join(unsigned char *s1, int size1, unsigned char *s2, int size2);
unsigned int get_hex_word32(const char *str);
void set_ctx(MD5_CTX *pctx, const char *digest, unsigned long nblocks);

int main() {

    unsigned char mess1_tag[MD5_DIGEST_LENGTH] = {};

    int key_length = 16;
    unsigned char key[] = {
            0xd0,0xab,0xdc,0x3c,0xb6,0x5d,0x2a,0x21,
            0x5e,0x71,0xce,0x4f,0x1f,0x32,0x1e,0xae
    };

    int mess1_length = 42;
    // What about joining me tomorrow for dinner? -> In hexadecimal
    unsigned char mess1[] = {
            0x57,0x68,0x61,0x74,0x20,0x61,0x62,0x6f,
            0x75,0x74,0x20,0x6a,0x6f,0x69,0x6e,0x69,
            0x6e,0x67,0x20,0x6d,0x65,0x20,0x74,0x6f,
            0x6d,0x6f,0x72,0x72,0x6f,0x77,0x20,0x66,
            0x6f,0x72,0x20,0x64,0x69,0x6e,0x6e,0x65,
            0x72,0x3f
    };
    int block_length = MD5_CBLOCK / 2;

    int key_mess1_length = key_length + mess1_length;
    unsigned char *key_mess1 = join(key, key_length, mess1, mess1_length);

    MD5_CTX mess1_tag_context;
    MD5_Init(&mess1_tag_context);
    MD5_Update(&mess1_tag_context, key_mess1, key_mess1_length);
    MD5_Final(mess1_tag, &mess1_tag_context);

    printf("Tag of the first message (k + mess1):\n"
           "What about joining me tomorrow for dinner? -> In hexadecimal\n\t");
    for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        printf("%02x", (unsigned int)mess1_tag[i]);
    printf("\n\n");

    int num_of_blocks = key_mess1_length / block_length + 1;

    int mess_extra_length = 76;
    // Oops, Sorry, I just remember that I have a meeting very soon in the morning. -> In hexadecimal
    unsigned char mess_extra[] = {
            0x4f,0x6f,0x70,0x73,0x2c,0x20,0x53,0x6f,
            0x72,0x72,0x79,0x2c,0x20,0x49,0x20,0x6a,
            0x75,0x73,0x74,0x20,0x72,0x65,0x6d,0x65,
            0x6d,0x62,0x65,0x72,0x20,0x74,0x68,0x61,
            0x74,0x20,0x49,0x20,0x68,0x61,0x76,0x65,
            0x20,0x61,0x20,0x6d,0x65,0x65,0x74,0x69,
            0x6e,0x67,0x20,0x76,0x65,0x72,0x79,0x20,
            0x73,0x6f,0x6f,0x6e,0x20,0x69,0x6e,0x20,
            0x74,0x68,0x65,0x20,0x6d,0x6f,0x72,0x6e,
            0x69,0x6e,0x67,0x2e
    };

    unsigned char forgery_tag[MD5_DIGEST_LENGTH] = {};
    MD5_CTX forgery_tag_context;
    MD5_Init(&forgery_tag_context);
    set_ctx(&forgery_tag_context, &mess1_tag, num_of_blocks);
    MD5_Update(&forgery_tag_context, mess_extra, mess_extra_length);
    MD5_Final(forgery_tag, &forgery_tag_context);

    printf("Tag of the forgery (k + mess1 + (padding) + mess_extra) without knowing message 1 nor they key:\n"
           "What about joining me tomorrow for dinner? -> In hexadecimal\n\t");
    for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        printf("%02x", (unsigned int)forgery_tag[i]);
    printf("\n\n");

    int padding_length = 70;
    unsigned char padding[] = {
                      0x80,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0xd0,0x01,0x00,0x00,0x00,0x00,0x00,0x00
    };

    int key_mess1_padding_length = key_mess1_length + padding_length;
    unsigned char *key_mess1_padding = join(key_mess1, key_mess1_length, padding, padding_length);
    int key_mess1_padding_mess_extra_length = key_mess1_padding_length + mess_extra_length;
    unsigned char *key_mess1_padding_mess_extra = join(key_mess1_padding, key_mess1_padding_length, mess_extra, mess_extra_length);

    unsigned char prove[MD5_DIGEST_LENGTH] = {};
    MD5_CTX context_prove;
    MD5_Init(&context_prove);
    MD5_Update(&context_prove, key_mess1_padding_mess_extra, key_mess1_padding_mess_extra_length);
    MD5_Final(prove, &context_prove);

    printf("Prove of the previous tag, knowing message 1, the key and the required padding:\n\t");
    for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        printf("%02x", (unsigned int)prove[i]);
    printf("\n");

    return 0;
}

unsigned char *join(unsigned char *s1, int size1, unsigned char *s2, int size2) {
    unsigned char *result = malloc(size1 + size2);
    memcpy(result, s1, size1);
    memcpy(result+size1, s2, size2);
    return result;
}

unsigned int get_hex_word32(const char *str) {
    return ((unsigned int *)str)[0];
}

void set_ctx(MD5_CTX *pctx, const char *digest, unsigned long nblocks) {
    pctx->A = get_hex_word32(digest);
    pctx->B = get_hex_word32(digest + 4);
    pctx->C = get_hex_word32(digest + 8);
    pctx->D = get_hex_word32(digest + 12);
    nblocks <<= 9;
    pctx->Nh = nblocks >> 32;
    pctx->Nl = nblocks & 0xFFFFFFFFul;
}