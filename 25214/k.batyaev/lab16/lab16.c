#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main(void) {
    struct termios old_tio, new_tio;

    if (tcgetattr(STDIN_FILENO, &old_tio) == -1) {
        perror("tcgetattr");
        return 1;
    }

    new_tio = old_tio;

    new_tio.c_lflag &= ~(ICANON | ECHO);
    new_tio.c_cc[VMIN] = 1;
    new_tio.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_tio) == -1) {
        perror("tcsetattr");
        return 1;
    }

    printf("Continue? (y/n): ");
    fflush(stdout);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    printf("\nAnswer: %c\n", ch);
    return 0;
}
