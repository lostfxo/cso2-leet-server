FROM alpine:edge

# install required programs
RUN apk add --update-cache boost boost-dev cmake g++ musl-dev ninja

# create dir
WORKDIR /leet

# get source code
COPY external ./external
COPY headers ./headers
COPY sources ./sources
COPY tests ./tests
COPY CMakeLists.txt ./ 

# setup build instructions
RUN mkdir build \
    && cd ./build \
    && cmake -G Ninja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS="-m64" -DCMAKE_BUILD_TYPE=Release -DLEET_BUILD_TESTS=OFF ../

# build the server
RUN cd ./build \
    && ninja

CMD [ "/leet/build/leet-server", "--interface", "eth0" ]
