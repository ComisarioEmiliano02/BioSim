#ifndef DETECCION_BROTES_H
#define DETECCION_BROTES_H

#include "estructuras.h"

// ============================================================
// SUBPROBLEMA 2: Detección de Brotes
// Encontrar componentes conectadas de infectados
// Restricción: < O(n^3)
// Algoritmo: BFS o DFS sobre grafo de contactos
// ============================================================

typedef struct {
  int *componentes;  // Array donde índice = id del individuo, valor = id de componente
  int num_componentes;
  int num_infectados_por_componente[1000];  // Max 1000 componentes
} ResultadoDeteccionBrotes;

/**
 * Detecta componentes conectadas de individuos infectados usando BFS
 * Complejidad: O(V + E) donde V es número de infectados, E es número de contactos
 * Retorna: ResultadoDeteccionBrotes con información de componentes
 */
ResultadoDeteccionBrotes detectar_brotes_bfs(Territorio *territorios, int num_territorios, Individuo *poblacion, int num_individuos);

/**
 * Detecta componentes conectadas de individuos infectados usando DFS
 * Complejidad: O(V + E)
 */
ResultadoDeteccionBrotes detectar_brotes_dfs(Territorio *territorios, int num_territorios, Individuo *poblacion, int num_individuos);

/**
 * Libera los resultados de detección de brotes
 * Complejidad: O(1)
 */
void deteccion_brotes_liberar(ResultadoDeteccionBrotes *resultado);

/**
 * Función de prueba para BFS y DFS
 * Complejidad: O(V + E) para cada algoritmo
 */
void test_deteccion_brotes(Territorio *territorios, int num_territorios,
                           Individuo *poblacion, int num_individuos);

#endif // DETECCION_BROTES_H
