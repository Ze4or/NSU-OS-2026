#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int main(void) {
    int pfd[2];
    pid_t pid;

    if (pipe(pfd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pfd[0]);
        close(pfd[1]);
        return 1;
    }

    if (pid == 0) {
        char buf[BUFSIZ];
        ssize_t r;

        close(pfd[1]);

        while ((r = read(pfd[0], buf, sizeof(buf))) > 0) {
            for (ssize_t i = 0; i < r; i++) {
                buf[i] = (unsigned char)toupper((unsigned char)buf[i]);
            }
            if (write(1, buf, (size_t)r) == -1) {
                perror("write");
                close(pfd[0]);
                return 1;
            }
        }

        if (r == -1) {
            perror("read");
            close(pfd[0]);
            return 1;
        }

        close(pfd[0]);
        return 0;
    }

    close(pfd[0]);

    const char text[] = "Hello, World! Text 123.\n";
    if (write(pfd[1], text, sizeof(text) - 1) == -1) {
        perror("write");
        close(pfd[1]);
        waitpid(pid, NULL, 0);
        return 1;
    }

    close(pfd[1]);
    
    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    return 0;
}
