# Stock Trading Strategy Backtester

## Introduction
This project is a stock trading strategy backtester designed to evaualte potential strategies by simulating trades based on given parameters. It combines backtesting with visualization to provide insights into the effectiveness of various trading approaches.


### Disclaimers
- Please note that this project was made with the purpose of learning a handful of new technologies which I haven't previously interacted with.
- The API used is quite bad since it was the only free API i would find that had the ability to query any of the top 500 stocks and the ticker time length. Please be mindful that the stock history does not go that far back and therefore using a ticker time length > 30m tends to show some pretty useless information. 

## Features

- **User Input Parameters:** Allow users to input specific stocks and tweak the strategy parameters to their will.
- **Chart Visualization:** Display stock charts to buy/sell markers indicating when the strategy would have executed trades.
- **Profit Calculation:** Compute and display profit metrics based on an initial user-determined capital amount and an assumption of all-in trades.
- **API integration:** Use C++ libraries for backend functionality, including libcurl for API calls and nlohmann.json for JSON data parsing.
- **Inter-Process Communication:** Utilize gRPC to facilitate real-time data transmission between the backend and frontend.

## Technologies Used

- **C++:** For efficient calculation and processing of large datasets and integration of libraries like libcurl.
- **Python and Streamlit:** For the front-end interface, enabling quick and easy app development.
- **Matplotlib:** For creating detailed stock charts.
- **gRPC:** For inter-process communication between C++ backend and Python frontend.

## Usage

Please note that i have very limited experience using docker and therefore this whole process is very messy.
### Clone Repo
`git clone https://github.com/KyleSch8989/Trading-Strategy-Backtester.git`  
`cd Trading-Strategy-Backtester`

### Create docker image:
`docker build -t backtester_project:latest`

### Enter docker container

`docker run -e STOCK_API_KEY='key' --rm -p 8501:8501 -it backtester_project:latest`

- The **key** variable is referring to the API key used to gather historical stock data.
- The API used can be found at the v1/stock/history endpoint under the [YH Finance](https://rapidapi.com/sparior/api/yahoo-finance15) API. The API is free but does require an account to use.

### Build and run

#### Build

`./build_script.sh`

#### Run Server and start streamlit frontend
`./run.sh`

#### Stop Server
`./stop.sh`
