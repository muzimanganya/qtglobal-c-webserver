#!/bin/bash

# Paths
LOG_DIR="$HOME/qtglobal-system-test/server/logs"
BACKUP_DIR="$HOME/backups"
LOG_FILE="$BACKUP_DIR/backup.log"

# Chwck if backup directory exists
mkdir -p "$BACKUP_DIR"

# Get Current date
DATE=$(date +%F)
BACKUP_FILE="$BACKUP_DIR/${DATE}_backup.tar.gz"

# Create backup
if [ -d "$LOG_DIR" ]; then
    tar -czf "$BACKUP_FILE" -C "$LOG_DIR" .
    if [ $? -eq 0 ]; then
        echo "$(date '+%Y-%m-%d %H:%M:%S') | Backup successful: $BACKUP_FILE" >> "$LOG_FILE"
    else
        echo "$(date '+%Y-%m-%d %H:%M:%S') | Backup failed: tar error" >> "$LOG_FILE"
    fi
else
    echo "$(date '+%Y-%m-%d %H:%M:%S') | Backup failed: source drectory missing" >> "$LOG_FILE"
fi

# Keep only last 7 backups
ls -1tr "$BACKUP_DIR"/*_backup.tar.gz 2>/dev/null | head -n -7 | xargs -d '\n' rm -f 2>/dev/null


