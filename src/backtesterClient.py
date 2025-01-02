import streamlit as st


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
        
        if form.form_submit_button("Submit"):
            st.write("Stock Symbol is: ", stockSymbol, "Ticker Size is: ", tickerTime, "SMA Short and Long: ", smaShort, ", ", smaLong)


