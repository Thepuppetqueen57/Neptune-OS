#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib/cJSON.h"

char* read_config(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    char *content = NULL;
    long filesize;

    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filepath);
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file); // Reset file pointer to the beginning

    // Allocate memory for the file contents
    content = (char *)malloc((filesize + 1) * sizeof(char));
    if (content == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read the file into the content buffer
    fread(content, sizeof(char), filesize, file);
    content[filesize] = '\0'; // Null-terminate the string

    fclose(file);
    return content;
}

void kernel_start() {
    printf("Kernel has started!\n");
}

int main() {
    kernel_start();
    printf("Do you want to configure the kernel? (Y/N): ");
    char kernel_config;
    scanf(" %c", &kernel_config);
    kernel_config = tolower(kernel_config);

    if (kernel_config == 'y') {
        printf("Configuring kernel...\n");
        printf("Kernel configuration will hopefully be added eventually!\n");
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
            printf("Max Processes: %d\n", (int)maxprocesses->valueint);
        } else {
            printf("Error: max-processes is not defined or there was an error parsing!\n");
            exit(1);
        }

        // Get max threads per process value from kernel.json
        cJSON *maxthreadsperprocess = cJSON_GetObjectItem(json, "max-threads-per-process");
        if (cJSON_IsNumber(maxthreadsperprocess)) {
            printf("Max Threads per Process: %d\n", (int)maxthreadsperprocess->valueint);
        } else {
            printf("Error: max-threads-per-process is not defined or there was an error parsing!\n");
            exit(1);
        }
        
        free(kerneljson);  // Don't forget to free the allocated memory
        cJSON_Delete(json); // Clean up the cJSON object
    }

    return 0;
}