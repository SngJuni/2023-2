TARGET=minishell
CXX=gcc
CXXFLAGS=
OBJECTS=PA1.o

$(TARGET):$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

PA1.o: PA1.c
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm $(OBJECTS) $(TARGET)
