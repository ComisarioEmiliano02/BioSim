#ifndef CLUSTERING_CEPAS_H
#define CLUSTERING_CEPAS_H

#include "estructuras.h"
#include "trie.h"

// ============================================================
// SUBPROBLEMA 7: Clustering de Cepas
// Agrupar variantes por similitud de nombre (cadena ADN)
// Restricción: Búsqueda en O(L) donde L es longitud de ADN
// Estructura: Trie (Árbol de Prefijos)
// ============================================================

typedef struct {
  int *cepas_grupo;
  int cantidad;
  char prefijo_comun[MAX_ADN];
} GrupoVariantes;

/**
 * Construye un Trie con todas las cepas
 * Complejidad: O(k * L) donde k = número de cepas, L = longitud promedio de ADN
 */
Trie* construir_trie_cepas(Cepa *cepas, int num_cepas);

/**
 * Busca cepas con un prefijo común
 * Complejidad: O(L + M) donde L = longitud del prefijo, M = número de resultados
 * Retorna: GrupoVariantes con cepas que comparten el prefijo
 */
GrupoVariantes clustering_por_prefijo(Trie *trie, const char *prefijo, Cepa *cepas);

/**
 * Agrupa todas las cepas por similitud (prefijos comunes)
 * Complejidad: O(k * L) donde k = número de cepas, L = longitud promedio
 * Retorna: Array de GrupoVariantes
 */
GrupoVariantes* clustering_completo(Cepa *cepas, int num_cepas, int *num_grupos);

/**
 * Libera los grupos de variantes
 * Complejidad: O(1)
 */
void clustering_liberar(GrupoVariantes *grupos, int num_grupos);

/**
 * Funcion de prueba para Subproblema 7
 * Demuestra clustering por prefijos y estadisticas
 */
void test_clustering_cepas(Cepa *cepas, int num_cepas);

#endif // CLUSTERING_CEPAS_H
