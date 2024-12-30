#include <vector>
#include <queue>
#include <mutex>
#include "signal.h"
#include <iostream>
#include <chrono>

std::vector<float> SMA (int thread_id, float capital, int movingAvgShort, int movingAvgLong, const std::vector<float>& prices) {
    std::mutex signalMutex;
    std::vector<float> signal = {0.0, 5.0, 100.0}; // {buy/sell (0.0, 1.0), timeOfSingal (tickers since start), quantity of stock}

    float shortAvg = 0.0;
    float longAvg = 0.0;

    bool shortGTlong = true;
    bool firstBuy = false;  //first signal can't be a sell signal


    for (int i = 0; i < (int)prices.size(); i++) {
        shortAvg += prices[i];
        longAvg += prices[i];

        if (i >= movingAvgShort) {
            shortAvg -= prices[i - movingAvgShort];
        }
        if (i >= movingAvgLong) {
            longAvg -= prices[i - movingAvgLong];
        }

        shortAvg /= movingAvgShort;
        longAvg /= movingAvgLong;

        if (firstBuy && shortGTlong && (longAvg > shortAvg)) {  //long avg crosses short avg ## SELL
            shortGTlong = false;
            //send Sell signal
            Signal sellSignal(thread_id, 1, i, capital/prices[i]);
            
            signalMutex.lock();
            signalQueue.push(sellSignal);
            signalMutex.unlock();
        } else if ((!firstBuy) && shortGTlong && (longAvg > shortAvg)) {
            shortGTlong = false;
        } else if (!shortGTlong && (shortAvg > longAvg)) { //Short avg crosses long avg ## BUY
            shortGTlong = true;
            firstBuy = true;
            //send Buy signal
            Signal buySignal(thread_id, 0, i, capital/prices[i]);

            signalMutex.lock();
            signalQueue.push(buySignal);
            signalMutex.unlock();
        }
    }

    //send final sell order
    std::lock_guard<std::mutex> lock(signalMutex);
    Signal lastBuySignal(thread_id, 0, prices.size(), capital/prices[prices.size() - 1]);


    //send signal once finished
    Signal endSignal(thread_id, 2, 0, 0);
    signalQueue.push(endSignal);

    return signal;
}