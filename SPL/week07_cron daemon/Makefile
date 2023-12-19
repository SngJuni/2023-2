#Makefile
TARGET=cron
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=cron.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

cron.o: cron.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJECTS) $(TARGET)
