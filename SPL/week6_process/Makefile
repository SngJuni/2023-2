#Makefile
TARGET=ex3
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=ex3.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

ex3.o: ex3.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJECTS) $(TARGET)
