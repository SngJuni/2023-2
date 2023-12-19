#Makefile
TARGET=ipc2
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=ipc2.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

ipc2.o: ipc2.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJECTS) $(TARGET)
