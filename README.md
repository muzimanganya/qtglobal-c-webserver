# QT Global C Web Server

This project demonstrates a minimal end-to-end system using a C web server with HAProxy TLS termination and automated log backups.

---

## 1. How to Build and Run the Server

### Prerequisites
- GCC compiler
- Make
- Linux environment

### Steps
1. **Clone the repository** (or extract the archive):
    ```bash
    git clone <your-repo-link>
    cd qtglobal-system-test/server
    ```

2. **Build the server**:
    ```bash
    make
    ```
    This compiles the server using:
    ```bash
    gcc src/main.c src/logger.c src/config.c -o server
    ```

3. **Run the server**:
    ```bash
    sudo ./server
    ```
    > Note: Port 80 requires root privileges; you can change the port in `config/server.conf` if necessary.

4. The server listens for HTTP requests and logs all incoming requests to the configured log file.

---

## 2. How to Test with HAProxy

### Prerequisites
- HAProxy installed
- Self-signed TLS certificate generated for `qtglobal.test`

### HAProxy Configuration
1. Place your certificate at:
    ```
    haproxy/certs/qtglobal.test.pem
    ```
2. Configure HAProxy (`haproxy/haproxy.cfg`) to terminate TLS and forward traffic to your server:
    ```bash
    frontend https_front
        bind *:443 ssl crt /home/desire/qtglobal-system-test/haproxy/certs/qtglobal.test.pem
        mode http
        default_backend http_back

    backend http_back
        mode http
        server c_server 127.0.0.1:80 check
    ```

### Test the setup
- **GET request:**
    ```bash
    curl -k https://qtglobal.test/
    ```
    Should return:
    ```html
    <html><body><h1>Hello from C Web Server</h1></body></html>
    ```

- **POST request:**
    ```bash
    curl -k -X POST https://qtglobal.test/test/url
    ```
    Should return:
    ```html
    <html><body><h1>Requested URL: /test/url</h1></body></html>
    ```

---

## 3. Optional Enhancements (Bonus Points)
- **Multithreaded or event-driven server**: The server can be enhanced to handle multiple requests concurrently.
- **Listener/Worker separation**: Separate listening and worker threads/processes for improved scalability.
- **Graceful shutdown**: Handles `SIGINT` and `SIGTERM` signals to close sockets and exit cleanly.
- **Configuration files**: Server port, buffer size, and log file paths are loaded from `config/server.conf` instead of hardcoding.
- **HAProxy security tuning**: Production-ready HAProxy configuration with TLS termination and proper checks.

---

## 4. Backup Script

### Location
`scripts/backup_logs.sh`

### Features
- Compresses the server log folder into `/home/backups/YYYY-MM-DD_backup.tar.gz`.
- Keeps only the last 7 backups.
- Logs success or failure in `/home/backups/backup.log`.
- Scheduled with cron to run daily at 2 AM:
    ```bash
    0 2 * * * /home/desire/qtglobal-system-test/scripts/backup_logs.sh
    ```
- Handles missing directories or permission errors gracefully.

---

## 5. Folder Structure
## Folder Structure

```text
qtglobal-system-test/
├── Y
├── docs
├── haproxy
│   ├── certs
│   │   ├── qtglobal.test.crt
│   │   ├── qtglobal.test.key
│   │   └── qtglobal.test.pem
│   └── haproxy.cfg
├── scripts
│   └── backup_logs.sh
└── server
    ├── Makefile
    ├── config
    │   └── server.conf
    ├── logs
    │   └── server.log
    ├── nano.5529.save
    ├── server
    └── src
        ├── config.c
        ├── config.h
        ├── logger.c
        ├── logger.h
        └── main.c

