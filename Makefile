C = gcc 
CFLAGS = -Iinclude
TARGET = httpserver

SRCS = cmd/tcp/main.c \
		internal/request/request.c \
		internal/headers/headers.c


OBJS = $(SRCS:.c =.o)

all:$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -lws2_32


clean:
	rm -f $(OBJS) $(TARGET)

