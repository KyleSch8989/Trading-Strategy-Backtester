#ifndef SIGNAL_H
#define SIGNAL_H

#include <queue>
#include <mutex>

struct Signal {
    int thread_id;
    int type; // (Buy: 0, Sell: 1, finished: 2)
    int days; // Days since start
    float quantity; // Amount of stock
};

extern std::queue<Signal> signalQueue;
extern std::mutex signalMutex;

#endif