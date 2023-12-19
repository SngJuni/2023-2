CC = gcc
CFLAGS = -Wall
TARGETS = server client

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

client: client.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

clean:
	rm -f $(TARGETS)
