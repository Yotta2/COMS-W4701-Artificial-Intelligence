CC=g++
CFLAGS=-c -std=c++0x #-Wall 
LDFLAGS=-std=c++0x  
SOURCES=$(wildcard src/*.cpp)
INCLUDES = $(wildcard include/*.h)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Sokoban

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(INCLUDES) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
		rm -rf $(OBJECTS) $(EXECUTABLE)
