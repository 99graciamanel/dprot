#include <stdlib.h>
#include <stdio.h>
#include <openssl/rc4.h>
#include <openssl/rand.h>

#ifdef __APPLE__
#include <sys/types.h>
#endif

u_char most_used_char(u_char s[]) {
    int i;                            //array's index
    int v;                            //auxiliary index for counting characters
    u_char c_aux;                     //auxiliary character
    int sum = 0;                      //auxiliary character's occurrence
    u_char c_max;                     //most used character
    int max = 0;                      //most used character's occurrence
    for (i = 0; s[i]; i++) {
        c_aux = s[i];
        for (v = 0; s[v]; v++) { if (c_aux == s[v]) sum++; } //responsible cycle for counting character occurrence
        if (sum > max) {              //checks if new character is the most used
            max = sum;
            c_max = c_aux;
        }
        sum = 0;                      //reset counting variable so it can counts new characters occurrence
    }
    return c_max;                     //this is the most used character!
}

void exercise1and2() {
    printf("\n--------------------------------------------\n");
    printf("----------------EXERCISE 1,2----------------\n");
    printf("--------------------------------------------\n");
    unsigned char plaintext = 0x90;
    unsigned char encrypted_0;
    unsigned char key[16] = {0x01, 0xFF, 0x00, 0x7e, 0x1a, 0x0b, 0xbc, 0x8c, 0x77, 0x06, 0x67, 0xbe, 0x44, 0xdc, 0xe1,
                             0x0c};
    int frequency = 0;
    unsigned char guessed_message_0[256];
    for (int x = 0; x <= 256; x++) {
        RC4_KEY rc4_key;
        key[2] = (unsigned char) x;
        RC4_set_key(&rc4_key, 16, &key);
        RC4(&rc4_key, 1, &plaintext, &encrypted_0);
        guessed_message_0[x] = encrypted_0 ^ (x + 0x02);
        unsigned char key_first_pos = encrypted_0 ^ plaintext;
        if ((x + 0x02) == key_first_pos) { frequency++; }
    }
    unsigned char most_common_char = most_used_char(guessed_message_0);
    printf("\tGuessed message: %02x", most_common_char);
    printf("\t(with freq.\t%d)\n", frequency);
}

void exercise3() {
    printf("--------------------------------------------\n");
    printf("-----------------EXERCISE 3-----------------\n");
    printf("--------------------------------------------");
    unsigned char plaintext = 0x90;
    unsigned char inputKey[16] = {0x01, 0xFF, 0x00, 0x7e, 0x1a, 0x0b, 0xbc, 0x8c, 0x77, 0x06, 0x67, 0xbe, 0x44, 0xdc,
                                  0xe1, 0x0c};
    unsigned char guessed_m0 = 0x90;
    unsigned char guessed_key[13] = {0};
    unsigned char matrix[13][256] = {0};
    unsigned char encrypted_0[13][256] = {0};
    for (int i = 0; i < 13; i++) {
        inputKey[0] = i + 3;
        int frequency = 0;
        for (int x = 0; x < 256; x++) {
            RC4_KEY rc4_key;
            inputKey[2] = (unsigned char) x;
            RC4_set_key(&rc4_key, 16, &inputKey);
            RC4(&rc4_key, 1, &plaintext, &encrypted_0[i][x]);

            //k[0] = ks0 -x-1-2-3
            //k[1] = ks0 -x-1-2-3-4 -k[0]
            //k[2] = ks0 -x-1-2-3-4-5 -k[0] -k[1]

            matrix[i][x] = (guessed_m0 ^ encrypted_0[i][x]) - x;
            unsigned char sum = 0;
            for (int s=0; s<=i+3; s++) { sum += s; }
            matrix[i][x] -= sum;
            for (int j = 1; j <= i; j++) { matrix[i][x] -= guessed_key[i - j]; }
            if (matrix[i][x] == inputKey[i + 3]) { frequency++; }
        }
        guessed_key[i] = most_used_char(matrix[i]);
        printf("\n\tGuessed k[%d] : %02x", i, guessed_key[i]);
        printf("\t(with freq.\t%d)", frequency);
    }
}

void exercise4(){
    printf("\n--------------------------------------------\n");
    printf("-----------------EXERCISE 4-----------------\n");
    printf("--------------------------------------------\n");
    unsigned char message[1];
    RAND_bytes(message, 1);
    printf("\tMessage: %02x\n", message[0]);

    unsigned char IV[3] = {0x01, 0xFF, 0x00};
    unsigned char LONG_KEY[13];
    RAND_bytes(LONG_KEY, 13);
    printf("\tIV: ");
    for (int i=0; i<3; i++) { printf("%02x ", IV[i]); }
    printf("\n\tLONG_KEY: ");
    for (int i=0; i<13; i++) { printf("%02x ", LONG_KEY[i]); }

    unsigned char KEY[16] = {0};
    for (int i=0; i<3; i++) { KEY[i] = IV[i]; }
    for (int i=3; i<16; i++) { KEY[i] = LONG_KEY[i-3]; }
    printf("\n\tKEY: ");
    for (int i=0; i<16; i++) { printf("%02x ", KEY[i]); }
    printf("\n--------------------------------------------\n");

    unsigned char encrypted[0] = {0};

    int frequency = 0;
    unsigned char guessed_message_0[256] = {0};
    for (int x = 0; x <= 256; x++) {
        RC4_KEY rc4_key;
        KEY[2] = (unsigned char) x;
        RC4_set_key(&rc4_key, 16, &KEY);
        RC4(&rc4_key, 1, &message, &encrypted);
        guessed_message_0[x] = encrypted[0] ^ (x + 0x02);
        unsigned char key_first_pos = encrypted[0] ^ message[0];
        if ((x + 0x02) == key_first_pos) { frequency++; }
    }
    unsigned char guessed_m = most_used_char(guessed_message_0);
    printf("\tGuessed message: %02x", guessed_m);
    printf("\t(with freq.\t%d)\n", frequency);

    unsigned char guessed_long_key[2] = {0};
    unsigned char matrix[2][256] = {0};
    unsigned char encrypted_0[2][256] = {0};
    for (int i = 0; i < 2; i++) {
        KEY[0] = i + 3;
        int frequency = 0;
        for (int x = 0; x < 256; x++) {
            RC4_KEY rc4_key;
            KEY[2] = (unsigned char) x;
            RC4_set_key(&rc4_key, 16, &KEY);
            RC4(&rc4_key, 1, &message, &encrypted_0[i][x]);

            matrix[i][x] = (guessed_m ^ encrypted_0[i][x]) - x;
            unsigned char sum = 0;
            for (int s=0; s<=i+3; s++) { sum += s; }
            matrix[i][x] -= sum;
            for (int j = 1; j <= i; j++) { matrix[i][x] -= guessed_long_key[i - j]; }
            if (matrix[i][x] == LONG_KEY[i]) { frequency++; }
        }
        guessed_long_key[i] = most_used_char(matrix[i]);
        printf("\tGuessed k[%d] : %02x\t", i, guessed_long_key[i]);
        printf("(with freq.\t%d)\n", frequency);
    }
    printf("--------------------------------------------\n");
}

int main() {
    exercise1and2();
    exercise3();
    exercise4();
    return 0;
}













