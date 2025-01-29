import streamlit as st
from collections import defaultdict
import grpc
import service_pb2
import service_pb2_grpc
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import markers

st.set_page_config(layout="wide")



col1, col2, col3 = st.columns([0.2, 0.2, 0.6], border=True)

with col1:
    st.header("Trading Strategy")
    if st.button("SMA", use_container_width=True):
        st.session_state["strategy"] = "SMA"

with col2:
    if "strategy" in st.session_state and st.session_state["strategy"] == "SMA":
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
                st.session_state["SMA_Response"] = feature

with col3:
    if "SMA_Response" in st.session_state:
        st.header("SMA strategy results")
        
        chart_data = pd.DataFrame(np.array(st.session_state["SMA_Response"].prices), columns=["Stock Price"])
        x = np.array(range(len(st.session_state["SMA_Response"].prices)))
        y = np.array(st.session_state["SMA_Response"].prices)

        fig, ax = plt.subplots(figsize=(8,4))

        #colors
        ax.plot(x, y, color='#ff4846')
        fig.patch.set_facecolor(color='#0e1118')
        ax.set_facecolor(color='#0e1118')
        for spine in ax.spines.values():
            spine.set_edgecolor('white')
        ax.xaxis.label.set_color('white')
        ax.yaxis.label.set_color('white')
        ax.tick_params(axis='x', colors='white')
        ax.tick_params(axis='y', colors='white')

        ax.set_xlabel("Tickers Since Start")
        ax.set_ylabel("Stock Price (USD)")

        marker_x_buy = [sig.timeSinceStart for sig in st.session_state["SMA_Response"].signals if sig.type == 0]
        marker_y_buy = [y[mx - 1] for mx in marker_x_buy]
        plt.plot(marker_x_buy, marker_y_buy, marker='v', color="green", linestyle='')

        marker_x_sell = [sig.timeSinceStart for sig in st.session_state["SMA_Response"].signals if sig.type == 1]
        marker_y_sell = [y[mx - 1] for mx in marker_x_sell]    
        plt.plot(marker_x_sell, marker_y_sell, marker='v', color="red", linestyle='')


        st.pyplot(fig)

        st.metric(label="Dollar Profit (USD)", value=f"{round(st.session_state["SMA_Response"].dollar_profit, 2)}$")
        st.metric(label="Percentage Profit", value=f"{round(st.session_state["SMA_Response"].percent_profit, 2)}%")
