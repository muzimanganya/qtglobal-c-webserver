# QTGlobal C Web Server

Minimal end-to-end HTTP server implemented in C, designed to demonstrate low-level networking, HTTP request handling, logging, and integration with HAProxy for TLS termination. It includes a backup automation script for server logs.

## How to Build and Run the Server

1. Navigate to the server folder:

```bash
cd server
Compile the server:

bash
Copier le code
make
Run the server (requires root for port 80):

bash
Copier le code
sudo ./server
The server will:

Listen on the configured port (default 80)

Accept GET and POST requests

Respond with minimal HTML pages:

GET: displays a greeting message

POST: displays the requested URL

How to Test with HAProxy
Generate a self-signed TLS certificate for qtglobal.test (certificate files in haproxy/certs/).

Ensure HAProxy is installed and configured (haproxy/haproxy.cfg) to:

Terminate TLS on port 443

Forward decrypted HTTP traffic to the C web server

Start HAProxy:

bash
Copier le code
sudo haproxy -f haproxy/haproxy.cfg
Test the server via HAProxy:

bash
Copier le code
curl -k https://qtglobal.test/
curl -k -X POST https://qtglobal.test/test/url
GET request: displays greeting page

POST request: displays requested URL

Backup Script (Task B)
Script location: scripts/backup_logs.sh

Stores compressed backups in /home/backups

Names backups: YYYY-MM-DD_backup.tar.gz

Keeps only the last 7 backups automatically

Logs operations in /home/backups/backup.log

Scheduled in cron to run daily at 2 AM:

Example cron entry:

cron
Copier le code
0 2 * * * /home/desire/qtglobal-system-test/scripts/backup_logs.sh
Optional Enhancements (Bonus Points)
Multithreaded or event-driven server

Listener/worker separation and connection pooling

Graceful shutdown / signal handling

Config files instead of hardcoded values

Production-level HAProxy tuning
