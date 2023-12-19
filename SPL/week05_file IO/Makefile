#Makefile
TARGET=ex4
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=ex4.o

$(TARGET): $(OBJECTS)
	        $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

ex4.o: ex4.c
	        $(CXX) $(CXXFLAGS) -c $^
		
clean:
	        rm -f $(OBJECTS) $(TARGET)

