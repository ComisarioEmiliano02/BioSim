#ifndef TRIE_H
#define TRIE_H

#include "estructuras.h"

// ============================================================
// TRIE (Árbol de Prefijos) - Subproblema 7: Clustering de Cepas
// Complejidad: O(L) donde L es la longitud de la cadena ADN
// ============================================================

/**
 * Crea un Trie vacío
 * Complejidad: O(1)
 */
Trie* trie_crear();

/**
 * Inserta una cadena ADN de una cepa en el Trie
 * Complejidad: O(L) donde L es la longitud de la cadena
 */
void trie_insertar(Trie *trie, const char *adn, int cepa_id);

/**
 * Busca una cadena ADN exacta en el Trie
 * Complejidad: O(L)
 * Retorna: ID de la cepa o -1 si no existe
 */
int trie_buscar(Trie *trie, const char *adn);

/**
 * Busca todas las cepas que comienzan con un prefijo
 * Complejidad: O(L + M) donde L es la longitud del prefijo y M es el número de resultados
 * Retorna: Array de IDs de cepas (debe liberarse con free)
 */
int* trie_buscar_por_prefijo(Trie *trie, const char *prefijo, int *cantidad);

/**
 * Libera toda la memoria del Trie
 * Complejidad: O(nodos en el trie)
 */
void trie_liberar(Trie *trie);

#endif // TRIE_H
