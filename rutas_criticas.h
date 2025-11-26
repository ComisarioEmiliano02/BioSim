#ifndef RUTAS_CRITICAS_H
#define RUTAS_CRITICAS_H

#include "estructuras.h"

// ============================================================
// SUBPROBLEMA 5: Rutas Críticas
// Camino de mayor probabilidad de infección entre territorios
// Restricción: O((n+m) log n)
// Algoritmo: Dijkstra modificado (Max-Heap, multiplicar probabilidades)
// ============================================================

typedef struct {
  int *ruta;
  int longitud_ruta;
  float probabilidad_total;
} RutaCritica;

/**
 * Encuentra la ruta de máxima probabilidad de infección entre dos territorios
 * Utiliza Dijkstra modificado con Max-Heap
 * Complejidad: O((n + m) log n) donde n = territorios, m = conexiones
 * territorio_origen: ID del territorio de inicio
 * territorio_destino: ID del territorio destino
 * Retorna: RutaCritica con la ruta y probabilidad acumulada
 */
RutaCritica dijkstra_maxima_probabilidad(
  Grafo *grafo_territorios,
  int territorio_origen,
  int territorio_destino,
  Cepa *cepas,
  int num_cepas
);

/**
 * Encuentra todas las rutas críticas desde un territorio hacia todos los demás
 * Complejidad: O((n + m) log n)
 */
RutaCritica* dijkstra_multiple(
  Grafo *grafo_territorios,
  int territorio_origen,
  Cepa *cepas,
  int num_cepas,
  int num_territorios
);

/**
 * Libera la memoria de una ruta crítica
 * Complejidad: O(1)
 */
void ruta_critica_liberar(RutaCritica *ruta);

#endif // RUTAS_CRITICAS_H
