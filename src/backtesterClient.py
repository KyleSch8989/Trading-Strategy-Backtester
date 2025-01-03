import streamlit as st
from collections import defaultdict
import grpc
import service_pb2
import service_pb2_grpc

st.set_page_config(layout="wide")
tickerMap = {"5m" : 0, "15m" : 1, "30m" : 2, "1h" : 3, "1d" : 4, "1wk" : 5, "1mo" : 6, "3mo" : 7}


col1, col2, col3 = st.columns([0.2, 0.2, 0.6], border=True)

with col1:
    st.header("Trading Strategy")
    if st.button("SMA", use_container_width=True):
        st.session_state["strategy"] = "SMA"

with col2:
    if st.session_state["strategy"] == "SMA":
        st.header(st.session_state["strategy"])
        form = st.form("SMA_form", border=False)
        stockSymbol = form.text_input("Stock Symbol")
        tickerTime = form.selectbox("Ticker Size", ("5m","15m","30m","1h","1d","1wk","1mo","3mo"))

        smaShort = form.slider("SMA short average", value=10)
        smaLong = form.slider("SMA short average", value=30)

        capital = form.number_input("Initial Investment", min_value=1)
        
        if form.form_submit_button("Submit"):
            st.write("Stock Symbol is: ", stockSymbol, "Ticker Size is: ", tickerTime, "SMA Short and Long: ", smaShort, ", ", smaLong)
            channel = grpc.insecure_channel('localhost:50051')
            stub = service_pb2_grpc.BacktesterGuiStub(channel)
            feature = None
            try:
                feature = stub.SMA_Call(service_pb2.SMARequest(StockSymbol=stockSymbol, tickerSize=tickerMap[tickerTime], SMAShort=smaShort, SMALong=smaLong))
            except:
                st.write("Could Not Connect to Server")
            if feature:
                st.write(feature.dollar_profit)
