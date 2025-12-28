#ifndef CONFIG_H
#define CONFIG_H

#define MAX_PATH 256

typedef struct {
    int port;
    int buffer_size;
    char log_file[MAX_PATH];
} server_config_t;

/* Load configuration from file */
int load_config(const char *filename, server_config_t *config);

#endif

