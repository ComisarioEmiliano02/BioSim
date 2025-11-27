#ifndef ANALISIS_DATOS_H
#define ANALISIS_DATOS_H

#include "estructuras.h"

// ============================================================
// SUBPROBLEMA 1: Análisis de Datos
// Ordenar individuos por Riesgo, Tiempo y Nombre
// Restricción: O(n log n)
// Algoritmos: MergeSort, QuickSort, HeapSort
// ============================================================

typedef enum {
  ORDENAR_POR_RIESGO,
  ORDENAR_POR_TIEMPO,
  ORDENAR_POR_NOMBRE
} CriterioOrdenamiento;

/**
 * Implementa MergeSort para ordenar individuos
 * Complejidad: O(n log n) garantizado
 */
void merge_sort(Individuo *array, int izq, int der, CriterioOrdenamiento criterio);

/**
 * Implementa QuickSort para ordenar individuos
 * Complejidad: O(n log n) promedio, O(n^2) peor caso
 */
void quick_sort(Individuo *array, int izq, int der, CriterioOrdenamiento criterio);

/**
 * Implementa HeapSort para ordenar individuos
 * Complejidad: O(n log n) garantizado
 */
void heap_sort(Individuo *array, int n, CriterioOrdenamiento criterio);

/**
 * Función de comparación para criterios de ordenamiento
 * Retorna: < 0 si a < b, 0 si a == b, > 0 si a > b
 */
int comparar_individuos(const Individuo *a, const Individuo *b, CriterioOrdenamiento criterio);

/**
 * Función de prueba para los tres algoritmos de ordenamiento
 * Complejidad: O(3 * n log n) = O(n log n)
 * Demuestra los tres algoritmos con tres criterios diferentes
 */
void test_analisis_datos(Individuo *poblacion, int num_individuos);

#endif // ANALISIS_DATOS_H
