#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned char *join(unsigned char *s1, int size1, unsigned char *s2, int size2);
void create_nodes(int n);
void compute_i0(int n);
void compute_i(int n, int i);

int main() {
    const int n = 4;
    create_nodes(n);
    return 0;
}

unsigned char *join(unsigned char *s1, int size1, unsigned char *s2, int size2) {
    unsigned char *result = malloc(size1 + size2);
    memcpy(result, s1, size1);
    memcpy(result+size1, s2, size2);
    return result;
}

void create_nodes(int n){
    compute_i0(n);
    compute_i(n/2, 0);
}

void compute_i0(int n) {
    char *command = "../compute_doc.sh ";
    for (int i=0; i < n; ++i) {
        char i_to_str[1];
        sprintf(i_to_str, "%d", i);
        char *command_with_args = join(command, strlen(command), i_to_str, 1);
        system(command_with_args);
    }
}

void compute_i(int n, int i) {
    char i_to_str[1];
    sprintf(i_to_str, "%d", i);

    i += 1;
    char i_1_to_str[1];
    sprintf(i_1_to_str, "%d", i);

    for (int j=0; j < n; ++j) {
        char command[100] = {};
        char j_to_str[1];
        char x_to_str[1];
        char x_1_to_str[1];

        strcpy(command, "../compute_node.sh");
        strncat(command, " ", 1);
        strncat(command, i_to_str, 1);
        strncat(command, " ", 1);
        strncat(command, i_1_to_str, 1);

        sprintf(j_to_str, "%d", j);

        sprintf(x_to_str, "%d", j);
        strncat(command, " ", 1);
        strncat(command, x_to_str, 1);

        sprintf(x_1_to_str, "%d", j+1);
        strncat(command, " ", 1);
        strncat(command, x_1_to_str, 1);

        strncat(command, " ", 1);
        strncat(command, j_to_str, 1);

        system(command);
    }
}