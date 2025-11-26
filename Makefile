# ============================================================
# MAKEFILE - BioSim: Simulador Algorítmico de Epidemias
# ============================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = -lm

# Directorio de objetos
OBJ_DIR = obj

# Archivos fuente
SOURCES = generador.c \
          hash_table.c \
          trie.c \
          heap.c \
          union_find.c \
          analisis_datos.c \
          deteccion_brotes.c \
          propagacion_temporal.c \
          minimizacion_riesgo.c \
          rutas_criticas.c \
          contencion_vacunacion.c \
          clustering_cepas.c \
          consultas_rapidas.c

HEADERS = estructuras.h \
          hash_table.h \
          trie.h \
          heap.h \
          union_find.h \
          analisis_datos.h \
          deteccion_brotes.h \
          propagacion_temporal.h \
          minimizacion_riesgo.h \
          rutas_criticas.h \
          contencion_vacunacion.h \
          clustering_cepas.h \
          consultas_rapidas.h

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))

TARGET = generador.exe

# ============================================================
# REGLAS
# ============================================================

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo Compilacion exitosa: $(TARGET)

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(TARGET) del $(TARGET)

rebuild: clean all

run: $(TARGET)
	@$(TARGET)

.PHONY: all clean rebuild run
