import http.client
import json
import sys

#conn = http.client.HTTPSConnection("yahoo-finance15.p.rapidapi.com")

#headers = {
#    'x-rapidapi-key': "9d535ff119msh679d5e23e6ac912p1f6c1djsnc751a2917ebe",
#    'x-rapidapi-host': "yahoo-finance15.p.rapidapi.com"
#}


stockSymbol = sys.argv[1] if sys.argv[1] else "AAPL" 
tickerLength = sys.argv[2] if sys.argv[2] else "5m"



#conn.request("GET", "/api/v1/markets/stock/history?symbol=AAPL&interval=5m&diffandsplits=false", headers=headers)

#res = conn.getresponse()
#data = res.read()

#parsed_data = json.loads(data.decode("utf-8"))

##with open("output.json", "w") as file:
##    json.dump(parsed_data, file, indent=4)


with open("output.json", "r") as file:
    data = json.load(file)


body = data.get("body", {})

for timestamp, entry in body.items():
    if 'open' in entry:  # Ensure 'date' key exists
        print(entry['open'])
