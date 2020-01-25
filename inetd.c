#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>

noreturn void err(const char *fn) {
    perror(fn);
    exit(1);
}

void sigchld_handler(int signal) {
    int old_err = errno;
    errno = 0;
    while (errno != ECHILD) {
        int status;
        waitpid(-1, &status, 0);
    }
    errno = old_err;
}

int main() {
    signal(SIGCHLD, sigchld_handler);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) err("socket");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = {0},
        .sin_port = htons(8080),
    };

    int e = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (e < 0) err("bind");

    e = listen(sock, 1);
    if (e < 0) err("listen");

    while (true) {
        struct sockaddr_in recv = {0};
        socklen_t recv_len = sizeof(recv);

        int conn = accept(sock, (struct sockaddr *)&recv, &recv_len);
        if (conn < 0) err("accept");
        if (recv_len != sizeof(recv)) err("accept len");

        if (fork()) {
            close(conn);
        } else {
            dup2(conn, 0);
            dup2(conn, 1);

            execl("./httpd", "./httpd", (char *)NULL);
        }
    }
}
