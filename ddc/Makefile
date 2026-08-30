CC = gcc
CFLAGS = -O2 -Wall -Wextra -fopenmp -pthread
LDFLAGS = -fopenmp -pthread -lm
OBJS = main.o compute.o parallel.o
TARGET = mandelbrot

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	
%.o: %.c mandelbrot.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.pgm times.txt