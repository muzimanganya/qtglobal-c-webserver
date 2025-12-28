#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "logger.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

volatile sig_atomic_t keep_running = 1;
int server_fd_global = -1;
server_config_t global_config;  // make config accessible to threads

/* Signal */
void handle_signal(int sig) {
    log_message("INFO", "Shutdown signal received");
    keep_running = 0;

    if (server_fd_global != -1) {
        close(server_fd_global);
    }
}

/* Thread function */
void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);

    char buffer[global_config.buffer_size];
    memset(buffer, 0, sizeof(buffer));

    read(client_fd, buffer, sizeof(buffer) - 1);

    char method[16];
    char url[256];
    sscanf(buffer, "%15s %255s", method, url);

    log_request(method, url);

    if (strcmp(method, "GET") == 0) {
        const char *body = "<html><body><h1>Hello from C Web Server</h1></body></html>";
        char response[global_config.buffer_size];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n"
                 "%s",
                 strlen(body), body);
        write(client_fd, response, strlen(response));
    } else if (strcmp(method, "POST") == 0) {
        char body[512];
        snprintf(body, sizeof(body),
                 "<html><body><h1>Requested URL: %s</h1></body></html>",
                 url);

        char response[global_config.buffer_size];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n"
                 "%s",
                 strlen(body), body);
        write(client_fd, response, strlen(response));
    }

    close(client_fd);
    return NULL;
}

int main() {
    /* Load config */
    if (load_config("config/server.conf", &global_config) != 0) {
        fprintf(stderr, "Failed to load configuration\n");
        return 1;
    }

    printf("Server starting on port %d\n", global_config.port);
    printf("Log file: %s\n", global_config.log_file);

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_fd_global = server_fd;

    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    /* Bind */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(global_config.port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    /* Listen */
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return 1;
    }

    log_message("INFO", "Server started successfully");

    while (keep_running) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (*client_fd < 0) {
            free(client_fd);
            if (!keep_running) break;
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);  // Free resources when done
    }

    close(server_fd);
    log_message("INFO", "Server stopped gracefully");
    return 0;
}

