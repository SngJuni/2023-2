#Makefile
TARGET=pthread
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=pthread.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) -lpthread

pthread.o: pthread.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJECTS) $(TARGET)
