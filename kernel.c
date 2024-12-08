#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib/cJSON.h"

#include "terminal.h"

char* read_config(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    char *content = NULL;
    long filesize;

    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file);

    content = (char *)malloc((filesize + 1) * sizeof(char));
    if (content == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(content, sizeof(char), filesize, file);
    content[filesize] = '\0';

    fclose(file);
    return content;
}

void kernel_start() {
    printf("Kernel has started!\n");
}

int main() {
    kernel_start();
    printf("Do you want to configure the kernel? [Y/N]: ");
    char kernel_config;
    scanf(" %c", &kernel_config);
    kernel_config = tolower(kernel_config);

    int maxprocessesint;
    int maxthreadsperprocessint;

    if (kernel_config == 'y') {
        printf("Configuring kernel...\n");
        printf("Kernel configuration will hopefully be added soon!\n");
    } else if (kernel_config == 'n') {
        printf("Kernel configuration not edited...\n");

        printf("Checking kernel configuration...\n");
        char* kerneljson = read_config("config/kernel.json");
        cJSON *json = cJSON_Parse(kerneljson);
        if (json == NULL) {
            printf("Error parsing JSON.\n");
            return 1;
        }

        // Get max processes value from kernel.json
        cJSON *maxprocesses = cJSON_GetObjectItem(json, "max-processes");
        if (cJSON_IsNumber(maxprocesses)) {
            maxprocessesint = (int)maxprocesses->valueint;
            printf("Max Processes: %d\n", maxprocessesint);
        } else {
            printf("Error: max-processes is not defined or there was an error parsing!\n");
            exit(1);
        }

        // Get max threads per process value from kernel.json
        cJSON *maxthreadsperprocess = cJSON_GetObjectItem(json, "max-threads-per-process");
        if (cJSON_IsNumber(maxthreadsperprocess)) {
            maxthreadsperprocessint = (int)maxthreadsperprocess->valueint;
            printf("Max Threads per Process: %d\n", maxthreadsperprocessint);
        } else {
            printf("Error: max-threads-per-process is not defined or there was an error parsing!\n");
            exit(1);
        }
        
        free(kerneljson);
        cJSON_Delete(json);
    }

    int processes = 1;
    osmain(&processes, maxprocessesint, maxthreadsperprocessint);

    return 0;
}