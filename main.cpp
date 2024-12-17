#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "strategies.h"
using namespace std;

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


int main(int argc, char** argv) {

    cout << "Trading Bot" << endl;
    string priceData = argv[1];

    vector<float> prices = parseData(priceData);
    std::reverse(prices.begin(), prices.end());
    std::vector<float> smaSignal = SMA(10000.0, 25, 200, prices);
    cout << "SMA Signal (Buy/Sell): "<< (smaSignal[0] ? "Buy, " : "Sell, ") << "Days Since Start: " << smaSignal[1] << ", Quantity of Stock: " << smaSignal[2] << endl; 
    return 0;
}