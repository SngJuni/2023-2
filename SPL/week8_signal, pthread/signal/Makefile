#Makefile
TARGET=signal
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=signal.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

signal.o: signal.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJECTS) $(TARGET)
