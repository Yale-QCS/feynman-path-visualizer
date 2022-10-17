.PHONY: all clean

LOCAL_HDR = channel.hpp faymann.hpp gates.hpp kvstore.hpp name.hpp serialization.hpp spawn.hpp types.hpp
LOCAL_SRC = faymann.cpp gates.cpp local.cpp name.cpp serialization.cpp spawn.cpp tests.cpp

all: local

local: $(LOCAL_HDR) $(LOCAL_SRC)
	g++ -std=c++17 -O2 -o local $(LOCAL_SRC) \
		-I /usr/local/include/uWebSockets \
		/usr/local/lib/uSockets.a \
		-lz -lboost_fiber -lboost_context -lpthread

clean:
	rm -f local
