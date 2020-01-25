
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    char *headers[64] = {0};
    int header_count = 0;

    while (true) {
        char *buffer = malloc(1024);
        char *e = fgets(buffer, 1024, stdin);

        if (e == NULL) break;
        if (strcmp(buffer, "\r\n") == 0) break;

        headers[header_count++] = buffer;
    }

    for (int i=0; i<header_count; i++) {
        fprintf(stderr, "%s", headers[i]);
    }

    char content[] = "Hello World\n";

    printf("HTTP/1.1 200 OK\r\n");
    printf("Content-Length: %li\r\n", sizeof(content) - 1);
    printf("Content-Type: text/plain\r\n");
    printf("Connection: Closed\r\n");
    printf("\r\n");

    printf("%s", content);

    fclose(stdout);
    fclose(stdin);
}

