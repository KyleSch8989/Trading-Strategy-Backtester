#!/bin/bash

cd build
./backtester_server > /dev/null &
echo Server started.

cd ../src
streamlit run backtesterClient.py &
echo GUI started.

echo run the 'stop.sh' script to stop server.