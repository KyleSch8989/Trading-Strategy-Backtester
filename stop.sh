#!/bin/bash


PID=$(pgrep -f "backtester_server")

if [ -n "$PID" ]; then
    echo "Killing process $PID"
    kill "$PID"
else
    echo "No server process found."
fi