# CC = gcc 
# CFLAGS = -Iinclude
# TARGET = httpserver
#
# SRCS = cmd/tcp/main.c \
# 		internal/request/request.c \
# 		internal/headers/headers.c
#
# vpath %.c cmd/tcp/ internal/request internal/headers
#
# BUILD_DIR = build
#
# OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))
#
# all:$(TARGET)
#
# $(BUILD_DIR)/%.o: %.c
# 	@mkdir -p $(BUILD_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@
#
# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -lws2_32
#
#
# TEST_SRC = internal/headers/headers.c \
# 		   internal/request/request.c \
# 		   internal/request/request_test.c
#
# TEST_OBJS = $(TEST_SRC:.c=.o)
# TEST_TARGET = request_test
#
# test:$(TEST_OBJS)
# 	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_TARGET) -lws2_32
# 	./$(TEST_TARGET)
# debug:
# 	@echo "files to be deleted: $(OBJS) $(TARGET) $(TEST_OBJS)"
#
# clean:
# 	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_OBJS) $(TEST_TARGET)
#
