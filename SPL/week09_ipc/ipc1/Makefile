#Makefile
TARGET=ipc1
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=ipc1.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

ipc1.o: ipc1.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJECTS) $(TARGET)
