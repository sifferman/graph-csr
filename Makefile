
# Makefile #

SOURCES=main.cpp
headers=graph.hpp
EXE=main

CXX=g++
CXXFLAGS=--std=c++11 -Wall

all: clean ${EXE}

${EXE}: ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} -o $@ ${SOURCES}

clean:
	rm -f *.o ${EXE} ${REMOVE}


# Individual Files

main.o: main.cpp
	${CXX} ${CXXFLAGS} -c main.cpp
