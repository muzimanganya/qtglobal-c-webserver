#include "logger.h"
#include <stdio.h>
#include <time.h>

void log_message(const char *level, const char *msg) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s | %s | %s\n", timestamp, level, msg);
}

void log_request(const char *method, const char *url) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s | %s | %s\n", timestamp, method, url);

    FILE *fp = fopen("logs/server.log", "a");
    if (fp) {
        fprintf(fp, "%s | %s | %s\n", timestamp, method, url);
        fclose(fp);
    }
}

