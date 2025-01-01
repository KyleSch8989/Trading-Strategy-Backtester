#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
#include <mutex>
#include "signal.h"
#include "strategies.h"
#include <thread>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <grpcpp/grpcpp.h>
#include "protos/service.grpc.pb.h"
#include "protos/service.pb.h"


#include "strategies.h"
using namespace std;
using json = nlohmann::json;


size_t cb(void* contents, size_t size, size_t nmemb, std::string responseData) {
    size_t totalSize = size * nmemb;
    responseData.append((char*)contents, totalSize);
    return totalSize;
}

std::vector<float> callStockAPI(std::string stockSymbol, std::string tickerLength) {
    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string response;
    std::string APIKEY_STRING;

    const char* APIKEY = std::getenv("STOCK_API_KEY");
    if (APIKEY) {
        APIKEY_STRING = APIKEY;
    } else {
        cout << "Error connecting to API" << endl;
        return std::vector<float>(0);
    }

    std::string request_URL = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/history?symbol=" + stockSymbol + "&interval=" + tickerLength + "&diffandsplits=false";
    std::string request_api_key = "X-Rapidapi-Key: " + APIKEY_STRING;

    if (curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, request_URL.c_str());
        
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, request_api_key.c_str());
        headers = curl_slist_append(headers, "X-Rapidapi-Host: yahoo-finance15.p.rapidapi.com");
        headers = curl_slist_append(headers, "Host: yahoo-finance15.p.rapidapi.com");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    } else {
        cout << "Failed to Initialize Curl_Init" << endl;
        return std::vector<float>(0);
    }

    std::vector<float> prices;

    if (res != CURLE_OK) {
        cout << "Failed to recive a response from the API" << endl;
        return std::vector<float>(0);
    } else {
        json response_json = json::parse(response); 
        auto json_body = response_json["body"];

        for (const auto &ticker : json_body) {
            prices.emplace_back((float)ticker["open"]);
        }
        
    }

    return prices;
}


void printSignal (Signal& s) {
    cout << "--Strategy Signal-- [Thread: "<< s.get_thread_id() << "] - [Type: " << (s.get_type() == 0 ? "Buy" : "Sell") << "] - [Stock Amount: " << s.get_quanity() << "] - [Tickers: " << s.get_tickers() << "]" << endl;
}

std::vector<float> userChooseData() {

    cout << "Please choose a stock symbol to view" << endl;
    std::string stockSymbol;
    cin >> stockSymbol;


    std::string tickerChoice;
    std::string tickerLength;
    std::vector<std::string> tickerSizes = {"5m","15m","30m","1h","1d","1wk","1mo","3mo"};
    
    while(true) {
        cout << "Please choose a number corresponding to the respective ticker length:" << endl;
        cout << "0 -> 5 minutes" << endl << "1 -> 15 minutes" << endl << "2 -> 30 minutes" << endl 
                << "3 -> 1 hour" << endl << "4 -> 1 day" << endl << "5 -> 1 week" << endl 
                << "6 -> 1 month" << endl << "7 -> 3 months" << endl;

        cin >> tickerChoice;
        int tickerNumberChoice = std::stoi(tickerChoice);
        if (tickerNumberChoice >= 8 ||  tickerNumberChoice < 0) {
            cout << "Please choose a number between 0 - 7" << endl;
        } else {
            tickerLength = tickerSizes[tickerNumberChoice];
            break;
        }
    }

    std::vector<float> prices = callStockAPI(stockSymbol, tickerLength);

    if (prices.size() < 1) {
        cout << "Stock Symbol not supported" << '\n' << "Please choose another stock symbol" << endl;
        while (true) {
            stockSymbol = "";
            cin >> stockSymbol;
            prices = callStockAPI(stockSymbol, tickerLength);

            if (prices.size() > 1) {
                break;
            }
        }
    }

    return prices;
}



std::queue<Signal> signalQueue;
std::mutex signalMutex;

int main(int argc, char** argv) {

    cout << "Trading Strategy Analysis Project" << endl << endl;
    

    std::vector<float> prices = userChooseData();

    
    bool eventLoop = true;

    
    std::thread smaThread1(SMA, 0, 10000.0, 25, 50, prices);

    while (eventLoop) {
        

        while (!signalQueue.empty()) {
            signalMutex.lock();
            Signal nextSignal = signalQueue.front();
            signalQueue.pop();

            if (nextSignal.get_type() == 2) {
                eventLoop = false;
            } else if (nextSignal.get_type() == 0) {
                printSignal(nextSignal);
            } else if (nextSignal.get_type() == 1) {
                printSignal(nextSignal);
            }
            signalMutex.unlock();

        }
    }
    

    smaThread1.join();
    
    return 0;
}