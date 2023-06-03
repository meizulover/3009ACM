#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

char slots[3][22];
int w = 0, r = 1, l = 2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *reader_thread(void *arg) {
    while(1) {
        
        if (l != r) {
            r = l;
         
            printf("%s", slots[r]);
            fflush(stdout);
            usleep(758621); //  22/29 = 758621
        } 
    }
}

char inp() {
    char c;
    c = getchar();
    if(c == EOF) exit(0);
    return c;
}

int main() {
    pthread_t readerTh;
    pthread_create(&readerTh, NULL, &reader_thread, NULL);

    while(1) {
        int j = 0;
        while((slots[w][j++] = inp()) != '*');
        slots[w][j] = 0;

        l = w;
        
        if (r != l) {
            w = 3 - r - l;
        } else {
            w = (l + 1) % 3;
        }
  
    }

    return 0;
}
