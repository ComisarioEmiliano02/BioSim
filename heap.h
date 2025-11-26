#ifndef HEAP_H
#define HEAP_H

#include "estructuras.h"

// ============================================================
// HEAP (Min-Heap y Max-Heap)
// Subproblema 3: Propagación Temporal (Min-Heap para eventos cronológicos)
// Subproblema 5: Rutas Críticas (Max-Heap para máxima probabilidad)
// Complejidad: O(log n) para inserción y extracción
// ============================================================

/**
 * Crea un Heap vacío
 * es_min_heap: true para Min-Heap, false para Max-Heap
 * Complejidad: O(1)
 */
Heap* heap_crear(int capacidad_inicial, bool es_min_heap);

/**
 * Inserta un elemento en el Heap
 * Complejidad: O(log n)
 */
void heap_insertar(Heap *heap, int individuo_id, int prioridad, void *datos);

/**
 * Extrae el elemento con mayor (Max-Heap) o menor (Min-Heap) prioridad
 * Complejidad: O(log n)
 * Retorna: ElementoHeap con el extremo del heap, o un elemento con id=-1 si está vacío
 */
ElementoHeap heap_extraer(Heap *heap);

/**
 * Consulta el elemento superior sin extraerlo
 * Complejidad: O(1)
 */
ElementoHeap heap_peek(Heap *heap);

/**
 * Verifica si el heap está vacío
 * Complejidad: O(1)
 */
bool heap_vacio(Heap *heap);

/**
 * Obtiene el número de elementos en el heap
 * Complejidad: O(1)
 */
int heap_tamano(Heap *heap);

/**
 * Libera toda la memoria del Heap
 * Complejidad: O(1)
 */
void heap_liberar(Heap *heap);

#endif // HEAP_H
