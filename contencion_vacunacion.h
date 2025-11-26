#ifndef CONTENCION_VACUNACION_H
#define CONTENCION_VACUNACION_H

#include "estructuras.h"
#include "union_find.h"

// ============================================================
// SUBPROBLEMA 6: Contención (Vacunación)
// Red mínima de conexión entre territorios para contención
// Restricción: O((n+m) log n)
// Algoritmo: Kruskal (Union-Find) o Prim (con Heap) para MST
// ============================================================

typedef struct {
  int territorio_1;
  int territorio_2;
  float peso;
} AristaTerritorios;

typedef struct {
  AristaTerritorios *aristas_mst;
  int num_aristas;
  float peso_total;
} ResultadoMST;

/**
 * Encuentra el Árbol de Expansión Mínima usando Kruskal
 * Complejidad: O((m + n) log n) = O(m log n) donde m es el número de aristas
 * El MST minimiza los territorios que necesitan vacunación para contener brotes
 * Retorna: Aristas que forman el MST
 */
ResultadoMST kruskal_mst(Grafo *grafo_territorios, int num_territorios);

/**
 * Encuentra el Árbol de Expansión Mínima usando Prim
 * Complejidad: O((n + m) log n) con Heap
 * territorio_inicio: territorio desde el cual comenzar
 */
ResultadoMST prim_mst(Grafo *grafo_territorios, int num_territorios, int territorio_inicio);

/**
 * Libera los resultados del MST
 * Complejidad: O(1)
 */
void mst_liberar(ResultadoMST *resultado);

#endif // CONTENCION_VACUNACION_H
