#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int load_config(const char *filename, server_config_t *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open config file");
        return -1;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "PORT=", 5) == 0) {
            config->port = atoi(line + 5);
        } 
        else if (strncmp(line, "BUFFER_SIZE=", 12) == 0) {
            config->buffer_size = atoi(line + 12);
        } 
        else if (strncmp(line, "LOG_FILE=", 9) == 0) {
            strncpy(config->log_file, line + 9, MAX_PATH - 1);
            config->log_file[MAX_PATH - 1] = '\0';
        }
    }

    fclose(file);
    return 0;
}

