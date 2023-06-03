#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define width 40
#define height 18
#define buf_length (width * height)
#define fox_init_x (width / 3 * 2)
#define fox_init_y (height / 2)
#define fox_init_dir 'u'
#define rabbit_init_x (width / 3)
#define rabbit_init_y (height / 2)
#define rabbit_init_dir 'r'

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char fox_dir = fox_init_dir;

struct termios tty_prepare() {
    struct termios tty_attr_old, tty_attr;
    tcgetattr(0, &tty_attr);
    tty_attr_old = tty_attr;
    tty_attr.c_lflag &= ~(ECHO | ICANON);
    tty_attr.c_cc[VMIN] = 1;
    tcsetattr(0, TCSAFLUSH, &tty_attr);
    return tty_attr_old;
}

void tty_restore(struct termios tty_attr) {
    tcsetattr(0, TCSAFLUSH, &tty_attr);
}

void *keys_thread(void *arg) {
    unsigned char d;
    while (1) {
        d = getchar();
        if (d == 0x1b) {
            d = getchar();
            if (d == 0x5b) {
                pthread_mutex_lock(&mutex);
                d = getchar();
                switch (d) {
                    case 0x41: fox_dir = 'u'; break;
                    case 0x42: fox_dir = 'd'; break;
                    case 0x43: fox_dir = 'r'; break;
                    case 0x44: fox_dir = 'l'; break;
                }
                pthread_mutex_unlock(&mutex);
            }
        }
    }
}

void update_coord(int *x_ptr, int *y_ptr, char dir) {
    switch (dir) {
        case 'u': if (*y_ptr > 1) (*y_ptr)--; break;
        case 'd': if (*y_ptr < height - 2) (*y_ptr)++; break;
        case 'l': if (*x_ptr > 1) (*x_ptr)--; break;
        case 'r': if (*x_ptr < width - 2) (*x_ptr)++; break;
    }
}

int main() {
    pthread_t keys_th;
    struct termios term_back;
    term_back = tty_prepare();

    pthread_create(&keys_th, NULL, &keys_thread, (void *)fox_init_dir);

    char rabbit_dir = rabbit_init_dir;
    int fox_x = fox_init_x, fox_y = fox_init_y;
    int rabbit_x = rabbit_init_x, rabbit_y = rabbit_init_y;

    int z = 0;

    while (1) {
        usleep(500000);//22x2=44

        pthread_mutex_lock(&mutex);
        update_coord(&fox_x, &fox_y, fox_dir);
        pthread_mutex_unlock(&mutex);

        int r = rand() % 4;
        rabbit_dir = "udlr"[r];
        update_coord(&rabbit_x, &rabbit_y, rabbit_dir);
        printf("\033[2J\033[%d;%dH@\033[%d;%dH*", fox_y, fox_x, rabbit_y, rabbit_x);
        fflush(stdout);

        if ((fox_x == rabbit_x && fox_y == rabbit_y) || z > 50) break;
        z++;
    }

    if (fox_x == rabbit_x && fox_y == rabbit_y) {
          printf("The fox caught the rabbit! Game over.\n");
}
pthread_cancel((void *) &keys_thread);
      tty_restore (term_back);
      return 0;
}

