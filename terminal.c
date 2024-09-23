#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int osmain(int *processes, int maxprocesses, int maxthreadsperprocess) {
    char cmd[100];
    while (1) {
        printf("> ");
        scanf("%99s", cmd);

        for (int i = 0; cmd[i]; i++) {
            cmd[i] = tolower(cmd[i]);
        }

        if (strcmp(cmd, "shutdown") == 0 || strcmp(cmd, "exit") == 0) {
            printf("Shutting down...\n");
            exit(0);
        } else if (strcmp(cmd, "processes") == 0) {
            printf("Processes: %d\n", processes[0]);
        } else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "commands") == 0) {
            printf("List of commands:\n");
            printf("1: shutdown\n");
            printf("2: processes\n");
        } else {
            printf("Unknown command: %s\n", cmd);
        }
    }
}