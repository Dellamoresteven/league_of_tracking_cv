CXX?=c++
SDL2FLAGS=$(shell pkg-config sdl2 --cflags --libs)
OPENCV2FLAGS=$(shell pkg-config opencv --cflags --libs)
CXXFLAGS?=-std=c++14 -Wall -pedantic -Wshadow -Wstrict-aliasing -Wstrict-overflow

.PHONY: all msg clean fullclean

all: msg main

msg:
	@echo '--- C++14 ---'

main: loft.cpp frame.cpp global.h helper.cpp
	${CXX} ${CXXFLAGS} -O2 -I /usr/local/include/eigen3 -o $@ $< ${SDL2FLAGS} ${OPENCV2FLAGS}
