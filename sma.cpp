#include <vector>
#include <queue>
#include <mutex>
#include "signal.h"
#include <iostream>
#include <chrono>

std::vector<float> SMA (int thread_id, float capital, int movingAvgShort, int movingAvgLong, const std::vector<float>& prices) {
    std::mutex signalMutex;
    std::vector<float> signal = {0.0, 5.0, 100.0}; // {buy/sell (0.0, 1.0), dateOfSingal (days since start), quantity of stock}

    float shortAvg = 0.0;
    float longAvg = 0.0;

    bool shortGTlong = true;


    for (int i = 0; i < (int)prices.size(); i++) {
        shortAvg += prices[i];
        longAvg += prices[i];

        if (i >= movingAvgShort) {
            shortAvg -= prices[i - movingAvgShort];
        }
        if (i >= movingAvgLong) {
            shortAvg -= prices[i - movingAvgLong];
        }

        shortAvg /= movingAvgShort;
        longAvg /= movingAvgLong;

        if (shortGTlong && (longAvg > shortAvg)) {  //long avg crosses short avg ## SELL
            shortGTlong = false;
            //send Sell signal
            Signal buySignal;
            buySignal.thread_id = thread_id;
            buySignal.type = 1;
            buySignal.days = i;
            buySignal.quantity = (capital/prices[i]);
            
            signalMutex.lock();
            signalQueue.push(buySignal);
            signalMutex.unlock();
        } else if (!shortGTlong && (shortAvg > longAvg)) { //Short avg crosses long avg ## BUY
            shortGTlong = true;
            //send Buy signal
            Signal sellSignal;
            sellSignal.thread_id = thread_id;
            sellSignal.type = 0;
            sellSignal.days = i;
            sellSignal.quantity = (capital/prices[i]);
            
            signalMutex.lock();
            signalQueue.push(sellSignal);
            signalMutex.unlock();
        }
    }

    //send signal once finished
    Signal endSignal;
    endSignal.thread_id = thread_id;
    endSignal.type = 2;
    endSignal.days = 0;
    endSignal.quantity = 0;
    std::lock_guard<std::mutex> lock(signalMutex);
    signalQueue.push(endSignal);

    return signal;
}