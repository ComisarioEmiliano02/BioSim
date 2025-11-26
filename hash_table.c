#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

// ============================================================
// IMPLEMENTACIÓN TABLA HASH
// Función hash simple: DHM (Daniel J. Bernstein)
// ============================================================

static unsigned long hash_djb2(int key) {
  return ((unsigned long)key) % HASH_TABLE_SIZE;
}

TablaHash* hash_table_crear() {
  TablaHash *tabla = (TablaHash *)malloc(sizeof(TablaHash));
  tabla->tabla = (NodoHash **)calloc(HASH_TABLE_SIZE, sizeof(NodoHash *));
  tabla->size = HASH_TABLE_SIZE;
  tabla->elementos = 0;
  return tabla;
}

void hash_table_insertar(TablaHash *tabla, int individuo_id, Individuo *individuo) {
  if (!tabla) return;
  
  unsigned long indice = hash_djb2(individuo_id);
  
  // Verificar si ya existe
  NodoHash *actual = tabla->tabla[indice];
  while (actual != NULL) {
    if (actual->individuo_id == individuo_id) {
      actual->data = individuo;
      return;
    }
    actual = actual->siguiente;
  }
  
  // Insertar al inicio
  NodoHash *nuevo = (NodoHash *)malloc(sizeof(NodoHash));
  nuevo->individuo_id = individuo_id;
  nuevo->data = individuo;
  nuevo->siguiente = tabla->tabla[indice];
  tabla->tabla[indice] = nuevo;
  tabla->elementos++;
}

Individuo* hash_table_buscar(TablaHash *tabla, int individuo_id) {
  if (!tabla) return NULL;
  
  unsigned long indice = hash_djb2(individuo_id);
  NodoHash *actual = tabla->tabla[indice];
  
  while (actual != NULL) {
    if (actual->individuo_id == individuo_id) {
      return actual->data;
    }
    actual = actual->siguiente;
  }
  
  return NULL;
}

void hash_table_eliminar(TablaHash *tabla, int individuo_id) {
  if (!tabla) return;
  
  unsigned long indice = hash_djb2(individuo_id);
  NodoHash *actual = tabla->tabla[indice];
  NodoHash *anterior = NULL;
  
  while (actual != NULL) {
    if (actual->individuo_id == individuo_id) {
      if (anterior == NULL) {
        tabla->tabla[indice] = actual->siguiente;
      } else {
        anterior->siguiente = actual->siguiente;
      }
      free(actual);
      tabla->elementos--;
      return;
    }
    anterior = actual;
    actual = actual->siguiente;
  }
}

void hash_table_liberar(TablaHash *tabla) {
  if (!tabla) return;
  
  for (int i = 0; i < tabla->size; i++) {
    NodoHash *actual = tabla->tabla[i];
    while (actual != NULL) {
      NodoHash *temp = actual;
      actual = actual->siguiente;
      free(temp);
    }
  }
  
  free(tabla->tabla);
  free(tabla);
}
