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

        // Remove every character that isn't a letter or a space
        int j = 0;
        for (int i = 0; prompt[i]; i++) {
            if ((prompt[i] >= 'a' && prompt[i] <= 'z') || prompt[i] == ' ') {
                prompt[j++] = prompt[i];
            }
        }
        prompt[j] = '\0';

        if (strcmp(prompt, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        } else if (strcmp(prompt, "what is neptune os") == 0) {
            printf("Neptune OS is a fake operating system written in C.\n");
            printf("It's not a real OS but is meant to somewhat resemble one.\n");
        } else if (strcmp(prompt, "what is neptubot") == 0) {
            printf("Neptubot is a simple chatbot written for Neptune OS.\n");
            printf("It uses hardcoded responses.\n");
            printf("Its made to answer question you may have about Neptune OS.\n");
        } else if (strcmp(prompt, "who made neptune os") == 0) {
            printf("Neptune OS was made by an indie dev who goes by Puppet online.\n");
            printf("You can find her projects at https://puppet57.xyz\n");
        }

        else {
            printf("I'm sorry, I don't know the answer to your question. Please try asking something else!\n");
        }
    }
}

#endif // NEPTUBOT_H