#!/bin/bash

set -e

# Build the application
echo "Building application..."
mkdir -p build
cd build
cmake ..
make
cd ..

# Install binary
echo "Installing binary to ~/.local/bin..."
mkdir -p ~/.local/bin
cp build/BirthDay ~/.local/bin/

# Cron setup
echo "Setting up cron job..."
CRON_CMD="0 10,18 * * * export DISPLAY=$DISPLAY && export DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS && ~/.local/bin/BirthDay -s"

# Remove old BirthDay jobs and add new one
(crontab -l 2>/dev/null || true) | grep -v "BirthDay" | { cat; echo "$CRON_CMD"; } | crontab -
echo "Cron job updated."

echo "Installation complete."
