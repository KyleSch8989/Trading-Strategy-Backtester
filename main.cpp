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
#include <thread>
#include <curl/curl.h>
#include <nlohmann/json.hpp>


#include "strategies.h"
using namespace std;
using json = nlohmann::json;

std::vector<float> parseData(string priceData) {
    vector<float> formattedPrices;
    ifstream data(priceData);
    string line;
    if (data.is_open()) {
        while (getline(data, line)) {
            if (line[0] == 'D') {
                continue;
            }
            stringstream ss(line);
            string token;
            int fieldIndex = 0;

            while (std::getline(ss, token, ':')) {
                ++fieldIndex;

                
                if (fieldIndex == 2 || fieldIndex == 5) {
                    try {
                        
                        formattedPrices.push_back(std::stof(token));
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Error: Invalid float value in field " << fieldIndex << ": " << token << endl;
                    }
                }
            }
        }
        data.close();
    } else {
        cout << "unable to open file" << endl;
    }

    return formattedPrices;
}

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
    }

    std::vector<float> prices;

    if (res != CURLE_OK) {
        cout << "Failed to recive a response from the API" << endl;
        return;
    } else {
        json response_json = json::parse(response); 
        auto json_body = response_json["body"];

        for (const auto &ticker : json_body) {
            prices.emplace_back((float)ticker["open"]);
        }
        
    }

    return prices;
}


void printSignal (const Signal& s) {
    cout << "--Strategy Signal-- [Thread: "<< s.thread_id << "] - [Type: " << (s.type == 0 ? "Buy" : "Sell") << "] - [Stock Amount: " << s.quantity << "] - [Day: " << s.days << "]" << endl;
}



std::queue<Signal> signalQueue;
std::mutex signalMutex;

int main(int argc, char** argv) {

    cout << "Trading Bot" << endl;
    string priceData = argv[1];

    vector<float> prices = parseData(priceData);
    std::reverse(prices.begin(), prices.end());


    std::string stockSymbol = "AAPL";
    std::string tickerLength = "5m";

    callStockAPI(stockSymbol, tickerLength);

    /*
    bool eventLoop = true;

    
    std::thread smaThread1(SMA, 0, 10000.0, 25, 150, prices);
    std::thread smaThread2(SMA, 1, 10000.0, 25, 200, prices);

    while (eventLoop) {
        

        while (!signalQueue.empty()) {
            signalMutex.lock();
            Signal nextSignal = signalQueue.front();
            signalQueue.pop();

            if (nextSignal.type == 2) {
                eventLoop = false;
                cout << "--[End]--" << endl;
            } else if (nextSignal.type == 0) {
                printSignal(nextSignal);
            } else if (nextSignal.type == 1) {
                printSignal(nextSignal);
            }
            signalMutex.unlock();

        }
    }
    

    smaThread1.join();
    smaThread2.join();
    */
    
    return 0;
}