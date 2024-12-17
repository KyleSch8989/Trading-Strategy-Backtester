#include <vector>

std::vector<float> SMA (float capital, int movingAvgShort, int movingAvgLong, std::vector<float> prices) {
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

        if (shortGTlong && (longAvg > shortAvg)) {  //long avg crosses short avg ## SELL
            shortGTlong = false;
            //send Sell signal
        } else if (!shortGTlong && (shortAvg > longAvg)) { //Short avg crosses long avg ## BUY
            shortGTlong = true;
            //send Buy signal
        }
    }

    return signal;
}