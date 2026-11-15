
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
THREAD_FLAGS = -pthread
LDLIBS = -pthread -lrt

TARGET = campuscompute

SOURCES = src/main.c src/scheduler.c src/logger.c src/ipc.c \
          src/banker.c src/worker.c src/resource.c

OBJECTS = $(SOURCES:.c=.o)

TESTS = test_scheduler test_logger test_ipc test_banker \
        test_worker test_resource

all: $(TARGET) $(TESTS)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@

test_scheduler: src/test_scheduler.c src/scheduler.c
	$(CC) $(CFLAGS) $^ -o $@

test_logger: src/test_logger.c src/logger.c
	$(CC) $(CFLAGS) $(THREAD_FLAGS) $^ -o $@

test_ipc: src/test_ipc.c src/ipc.c
	$(CC) $(CFLAGS) $(THREAD_FLAGS) $^ -o $@ $(LDLIBS)

test_banker: src/test_banker.c src/banker.c
	$(CC) $(CFLAGS) $^ -o $@

test_worker: src/test_worker.c src/worker.c
	$(CC) $(CFLAGS) $^ -o $@

test_resource: src/test_resource.c src/resource.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TESTS) *.o

.PHONY: all clean
