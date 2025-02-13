#ifndef SIGNAL_H
#define SIGNAL_H

#include <queue>
#include <mutex>

class Signal {
    private:

        int thread_id;
        int type; // (Buy: 0, Sell: 1, finished: 2)
        int tickers; // Days since start
        float quantity; // Amount of stock
        float price;
    
    public:

        Signal(int id, int signal_type, int tickers_since_start, float stock_quantity, float stock_price) {
            thread_id = id;
            type = signal_type;
            tickers = tickers_since_start;
            quantity = stock_quantity;
            price = stock_price;
        }

        int get_thread_id() {
            return thread_id;
        }

        int get_type() {
            return type;
        }

        int get_tickers() {
            return tickers;
        }

        float get_quantity() {
            return quantity;
        }

        float get_price() {
            return price;
        }
};

//extern std::queue<Signal> signalQueue;
extern std::mutex signalMutex;

#endif