#include <vector>
#include <queue>
#include <mutex>
#include "signal.h"
#include "strategies.h"
#include <iostream>
#include <chrono>

std::queue<Signal> SMA (int thread_id, float capital, int movingAvgShort, int movingAvgLong, const std::vector<float>& prices) {
    std::cout << "Running SMA strategy" << std::endl;
    std::cout << "prices size: " << prices.size() << std::endl;
    std::queue<Signal> signalQueue;
    float current_capital{capital};

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
            float last_buy_quantity{signalQueue.back().get_quantity()};
            Signal sellSignal(thread_id, 1, i, last_buy_quantity, prices[i]);
            current_capital = last_buy_quantity * prices[i];
            //signalMutex.lock();
            signalQueue.push(sellSignal);
            //signalMutex.unlock();
        } else if ((!firstBuy) && shortGTlong && (longAvg > shortAvg)) {
            shortGTlong = false;
        } else if (!shortGTlong && (shortAvg > longAvg)) { //Short avg crosses long avg ## BUY
            shortGTlong = true;
            firstBuy = true;
            //send Buy signal
            Signal buySignal(thread_id, 0, i, current_capital/prices[i], prices[i]);

           // signalMutex.lock();
            signalQueue.push(buySignal);
            //signalMutex.unlock();
        }
    }

    //send final sell order if last order was a buy
    if (signalQueue.back().get_type() == 0) {
        Signal lastBuySignal(thread_id, 1, prices.size(), capital/(prices[static_cast<int>(prices.size()) - 1]), prices[static_cast<int>(prices.size()) - 1]);
        signalQueue.push(lastBuySignal);
    }
    

    //send signal once finished
    Signal endSignal(thread_id, 2, 0, 0, 0);
    signalQueue.push(endSignal);
    return signalQueue;
}