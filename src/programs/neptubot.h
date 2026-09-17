#ifndef NEPTUBOT_H
#define NEPTUBOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static inline void neptubot() {
    char prompt[100];

    printf("Hello! I'm Neptubot! Ask me anything!\n");
    printf("Type 'exit' to exit Neptubot!\n");
    while (1) {
        // Flush stdin to prevent leftover input from previous commands
        int c;

        printf("Prompt: ");
        fgets(prompt, sizeof(prompt), stdin);
        prompt[strcspn(prompt, "\n")] = '\0';

        for (int i = 0; prompt[i]; i++) {
            prompt[i] = tolower(prompt[i]);
        }

        if (strcmp(prompt, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        } else if (strcmp(prompt, "what is neptune os") == 0) {
            printf("Neptune OS is a fake operating system written in C.\n");
            printf("It's not a real OS but is meant to somewhat resemble one.\n");
        }

        else {
            printf("I'm sorry, I don't know the answer to your question. Please try asking something else!\n");
        }
    }
}

#endif // NEPTUBOT_H