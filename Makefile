CC = gcc 
CFLAGS = -Iinclude -Wall -O2 -std=c99


SOURCES = internal/headers/headers.c  internal/request/request.c
HEADERS = include/request.h include/headers.h include/server.h

OBJECTS = $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

HTTP = build/main.o build/server.o build/response.o 
REQ_TEST = build/request_test.o
HEADERS_TEST = build/headers_test.o

vpath %.c internal/headers internal/request internal/server internal/response httpserver

http: $(OBJECTS) $(HTTP)
	$(CC) $(CFLAGS) -o $@ $^ -lws2_32

req_test: $(OBJECTS) $(REQ_TEST)
	$(CC) $(CFLAGS) -o $@ $^ -lws2_32 -lmunit

headers_test: $(OBJECTS) $(HEADERS_TEST)
	$(CC) $(CFLAGS) -o $@ $^ -lws2_32 -lmunit


build/%.o: %.c $(HEADERS) | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

.PHONY: clean
clean:
	rm -f http http.exe req_test req_test.exe headers_test headers_test.exe $(OBJECTS) $(HTTP) $(REQ_TEST) $(HEADERS_TEST)


