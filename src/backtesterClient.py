import streamlit as st
from collections import defaultdict
import grpc
import service_pb2
import service_pb2_grpc

st.set_page_config(layout="wide")



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

        userCapital = form.number_input("Initial Investment", min_value=1)
        
        if form.form_submit_button("Submit"):
            st.write("Stock Symbol is: ", stockSymbol, "Ticker Size is: ", tickerTime, "SMA Short and Long: ", smaShort, ", ", smaLong, "user capital: ", userCapital)
            channel = grpc.insecure_channel('localhost:50051')
            stub = service_pb2_grpc.BacktesterGuiStub(channel)
            feature = None
            try:
                feature = stub.SMA_Call(service_pb2.SMARequest(StockSymbol=stockSymbol, tickerSize=tickerTime, SMAShort=smaShort, SMALong=smaLong, capital=userCapital))
            except:
                st.write("Could Not Connect to Server")
            if feature:
                st.write("request has been recived with a profit of: $", feature.dollar_profit)
