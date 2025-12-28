# QTGlobal C Web Server

Minimal end-to-end HTTP server implemented in C, designed to demonstrate low-level networking, HTTP request handling, logging, and integration with HAProxy for TLS termination. It includes a backup automation script for server logs.

## Project Objective

Build a small end-to-end system demonstrating:

- Low-level networking
- HTTP request handling
- TLS termination
- Reverse proxy configuration

## Folder Structure
qtglobal-system-test/
├── haproxy
│   ├── certs
│   │   ├── qtglobal.test.crt
│   │   ├── qtglobal.test.key
│   │   └── qtglobal.test.pem
│   └── haproxy.cfg
├── scripts
│   └── backup_logs.sh
└── server
    ├── Makefile
    ├── config
    │   └── server.conf
    ├── logs
    │   └── server.log
    ├── nano.5529.save
    ├── server
    └── src
        ├── config.c
        ├── config.h
        ├── logger.c
        ├── logger.h
        └── main.c
└── README.md

## Task A – Minimal C Web Server

### Features

- Creates TCP socket using `socket()`.
- **Binds** to a fixed port (default 80, configurable in `config/server.conf`) using `bind()`.
- **Listens** for incoming connections with `listen()`.
- Accepts clients using `accept()`.
- Reads HTTP requests using `read()`.
- Parses minimal HTTP to extract:
  - Method (`GET` or `POST`)
  - Request URL
- Responds:
  - `GET` → HTML page with greeting
  - `POST` → HTML page showing the requested URL
  - Other methods → silently close connection
- Logs requests with `log_request()` function.
- Graceful shutdown using signals (`SIGINT`, `SIGTERM`).

### Sample Usage

```c
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
bind(server_fd, ...);
listen(server_fd, 5);
client_fd = accept(server_fd, ...);
read(client_fd, buffer, sizeof(buffer));
log_request(method, url);
write(client_fd, response, strlen(response));
close(client_fd);

## Task B – Backup Script & Cron

Script: scripts/backup_logs.sh

Compresses server logs into /home/backups/YYYY-MM-DD_backup.tar.gz.

Keeps only the last 7 backups, deletes older files.

Logs each operation to /home/backups/backup.log.

Handles errors gracefully (missing directories, permissions, tar failures).

Cron Entry
0 2 * * * /home/desire/qtglobal-system-test/scripts/backup_logs.sh

