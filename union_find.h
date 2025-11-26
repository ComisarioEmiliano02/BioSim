#ifndef UNION_FIND_H
#define UNION_FIND_H

#include "estructuras.h"

// ============================================================
// UNION-FIND (Disjoint Set Union)
// Subproblema 6: Contención (Vacunación) - MST con Kruskal
// Complejidad: O(α(n)) ≈ O(1) amortizado (α = inversa de Ackermann)
// ============================================================

/**
 * Crea una estructura Union-Find con n elementos
 * Inicialmente, cada elemento está en su propio conjunto
 * Complejidad: O(n)
 */
UnionFind* union_find_crear(int n);

/**
 * Encuentra el representante (raíz) del conjunto al que pertenece x
 * Con compresión de camino: O(α(n)) amortizado
 * Complejidad: O(α(n)) ≈ O(1)
 */
int union_find_buscar(UnionFind *uf, int x);

/**
 * Une dos elementos en el mismo conjunto
 * Con unión por rango: O(α(n)) amortizado
 * Complejidad: O(α(n)) ≈ O(1)
 * Retorna: true si se unieron, false si ya estaban en el mismo conjunto
 */
bool union_find_unir(UnionFind *uf, int x, int y);

/**
 * Verifica si dos elementos están en el mismo conjunto
 * Complejidad: O(α(n)) ≈ O(1)
 */
bool union_find_mismo_conjunto(UnionFind *uf, int x, int y);

/**
 * Libera toda la memoria de la estructura Union-Find
 * Complejidad: O(1)
 */
void union_find_liberar(UnionFind *uf);

#endif // UNION_FIND_H
