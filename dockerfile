FROM ubuntu:latest

EXPOSE 8501
EXPOSE 8502

RUN apt-get update && apt-get install -y cmake build-essential git

WORKDIR /deps

RUN git clone --recurse-submodules -b v1.66.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc

RUN mkdir -p /deps/grpc/build && cd /deps/grpc/build && \
    cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      .. && \
      make -j8 install


RUN apt install libcurl4-openssl-dev

RUN apt install nlohmann-json3-dev

RUN apt install -y python3
RUN apt install -y python3-pip

RUN pip install --break-system-pack streamlit
RUN pip install --break-system-pack grpcio
RUN pip install --break-system-pack grpcio-tools
RUN pip install --break-system-pack matplotlib

WORKDIR /
RUN mkdir project
WORKDIR /project
RUN git clone https://github.com/KyleSch8989/Trading-Strategy-Backtester.git

WORKDIR /project/Trading-Strategy-Backtester/



