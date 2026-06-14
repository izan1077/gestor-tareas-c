# Variables para facilitar cambios en el futuro
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = gestor

# La regla principal (lo que se ejecuta al escribir 'make')
all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) -lsqlite3

# Regla para limpiar los archivos compilados y empezar desde cero
clean:
	rm -f $(TARGET) tareas.db