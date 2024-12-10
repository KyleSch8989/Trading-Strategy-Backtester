#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
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

                // Check if the current field is the 2nd or 5th field
                if (fieldIndex == 2 || fieldIndex == 5) {
                    try {
                        // Convert the string to a float and add to the vector
                        formattedPrices.push_back(std::stof(token));
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Error: Invalid float value in field " << fieldIndex << ": " << token << '\n';
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
    for (auto price : prices) {
        cout << price << endl;
    }
    
    return 0;
}