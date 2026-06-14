CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = gestor

# Ahora listamos los dos archivos fuentes necesarios
SRCS = main.c db.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) -lsqlite3

clean:
	rm -f $(TARGET) tareas.db