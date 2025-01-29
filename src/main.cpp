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

std::vector<float> userChooseData();
//std::queue<Signal> signalQueue;
std::mutex signalMutex;


size_t cb(void* contents, size_t size, size_t nmemb, std::string responseData) {
    size_t totalSize = size * nmemb;
    responseData.append((char*)contents, totalSize);
    return totalSize;
}

std::vector<float> callStockAPI(std::string stockSymbol, std::string tickerLength) {
    std::cout << "calling stock api on; stock: " << stockSymbol << ", ticker length: " << tickerLength << endl;
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

    if (prices.size() == 0) {
        cout << "API does not have any data for this stock symbol and ticker size" << endl;
        return std::vector<float>(0);
    }

    return prices;
}


void printSignal (Signal& s) {
    cout << "--Strategy Signal-- [Thread: "<< s.get_thread_id() << "] - [Type: " << (s.get_type() == 0 ? "Buy" : "Sell") << "] - [Stock Amount: " << s.get_quantity() << "] - [Tickers: " << s.get_tickers() << "]" << endl;
}

std::pair<float, float> determineProfit(const std::queue<Signal>& strategySignals, int capital) {
    cout << "Determining Profit" << endl;
    float dollar_profit;
    float percent_profit;
    std::queue<Signal> strategySignals_cpy = strategySignals;
    
    for (int i = 0; i < strategySignals_cpy.size() - 2; i++) {
        Signal s = strategySignals_cpy.front();
        cout << "type: " << s.get_type() << ", quantity: " << s.get_quantity() << ", dollar amount: " << s.get_quantity() * s.get_price() << endl;
        strategySignals_cpy.pop();
        
    }
    
    float final_capital = strategySignals_cpy.front().get_quantity() * strategySignals_cpy.front().get_price();
    dollar_profit = (capital - final_capital) * -1;
    percent_profit = ((final_capital - capital)/capital) * 100;
    cout << "quantity: " << strategySignals_cpy.front().get_quantity() << ", dollar amount: " << final_capital << endl;
    return std::pair<float, float>(dollar_profit, percent_profit);
}



class BacktesterGuiImpl final : public BacktesterGui::Service {
    public:
        ::grpc::Status SMA_Call(grpc::ServerContext* context, const SMARequest* request, 
                              SMAReply* response) {
            cout << "handling request and response" << endl;
            auto stockSymbol = request->stocksymbol();
            auto tickerLength = request->tickersize();
            auto SMA_short = request->smashort();
            auto SMA_long = request->smalong();
            auto capital = request->capital();
            cout << "symbol: " << stockSymbol << " tickersize: " << tickerLength << " smashort: " << SMA_short << " smalong: " << SMA_long << " capital: " << capital << endl;
            
            const std::vector<float> stock_prices = callStockAPI(stockSymbol, tickerLength);
            std::queue<Signal> strategySignals = SMA(0, static_cast<float>(capital), SMA_short, SMA_long, stock_prices);
            const std::pair<float, float> profit = determineProfit(strategySignals, capital);

            response->set_dollar_profit(profit.first); 
            response->set_percent_profit(profit.second); 

            auto* prices_field = response->mutable_prices();
            prices_field->Add(stock_prices.begin(), stock_prices.end()); 
            
            for (; !strategySignals.empty(); strategySignals.pop()) {
                Signal s = strategySignals.front();
                auto signal_field = response->add_signals();
                signal_field->set_type(s.get_type());
                signal_field->set_timesincestart(s.get_tickers());
                signal_field->set_quantity(s.get_quantity());
            }

            cout << "Request handled and response sent!" << endl << endl;
            return grpc::Status::OK;
        }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    BacktesterGuiImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server Listening On: " << server_address << endl;
    server->Wait();
}





int main(int argc, char** argv) {

    cout << "Trading Strategy Analysis Project" << endl << endl;
    
    RunServer();
    
    return 0;
}


/*

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
*/






/*
std::vector<float> userChooseData() {   //for terminal based application(no longer available)

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

*/