#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STOCKS 30

char random_char() {
    return 'A' + rand() % 26;
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < MAX_STOCKS; i++) {
        printf("%c%c%c%c%03d*\n", random_char(), random_char(), random_char(), random_char(), rand() % 1000);
        fflush(stdout);
        usleep(500000);
    }
    // Send EOF to signal the end of input.
    fclose(stdout);
    return 0;
}
