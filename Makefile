CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = biosim_generator

all: $(TARGET)

$(TARGET): generador.c estructuras.h
	$(CC) $(CFLAGS) -o $(TARGET) generador.c

clean:
	del $(TARGET).exe
