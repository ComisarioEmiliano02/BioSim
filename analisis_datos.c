#include "analisis_datos.h"
#include <stdlib.h>
#include <string.h>

// ============================================================
// SUBPROBLEMA 1: Análisis de Datos
// Implementaciones de MergeSort, QuickSort y HeapSort
// ============================================================

int comparar_individuos(const Individuo *a, const Individuo *b, CriterioOrdenamiento criterio) {
  if (!a || !b) return 0;
  
  switch (criterio) {
    case ORDENAR_POR_RIESGO:
      return a->riesgo - b->riesgo;
    case ORDENAR_POR_TIEMPO:
      return a->tiempo_infeccion - b->tiempo_infeccion;
    case ORDENAR_POR_NOMBRE:
      return strcmp(a->nombre, b->nombre);
    default:
      return 0;
  }
}

// MergeSort: O(n log n) garantizado
void merge(Individuo *array, int izq, int mid, int der, CriterioOrdenamiento criterio) {
  int n1 = mid - izq + 1;
  int n2 = der - mid;
  
  Individuo *L = (Individuo *)malloc(n1 * sizeof(Individuo));
  Individuo *R = (Individuo *)malloc(n2 * sizeof(Individuo));
  
  for (int i = 0; i < n1; i++)
    L[i] = array[izq + i];
  for (int i = 0; i < n2; i++)
    R[i] = array[mid + 1 + i];
  
  int i = 0, j = 0, k = izq;
  while (i < n1 && j < n2) {
    if (comparar_individuos(&L[i], &R[j], criterio) <= 0) {
      array[k++] = L[i++];
    } else {
      array[k++] = R[j++];
    }
  }
  
  while (i < n1) array[k++] = L[i++];
  while (j < n2) array[k++] = R[j++];
  
  free(L);
  free(R);
}

void merge_sort_helper(Individuo *array, int izq, int der, CriterioOrdenamiento criterio) {
  if (izq < der) {
    int mid = izq + (der - izq) / 2;
    merge_sort_helper(array, izq, mid, criterio);
    merge_sort_helper(array, mid + 1, der, criterio);
    merge(array, izq, mid, der, criterio);
  }
}

void merge_sort(Individuo *array, int izq, int der, CriterioOrdenamiento criterio) {
  merge_sort_helper(array, izq, der, criterio);
}

// QuickSort: O(n log n) promedio, O(n^2) peor caso
int partition(Individuo *array, int izq, int der, CriterioOrdenamiento criterio) {
  Individuo pivot = array[der];
  int i = izq - 1;
  
  for (int j = izq; j < der; j++) {
    if (comparar_individuos(&array[j], &pivot, criterio) < 0) {
      i++;
      Individuo temp = array[i];
      array[i] = array[j];
      array[j] = temp;
    }
  }
  
  Individuo temp = array[i + 1];
  array[i + 1] = array[der];
  array[der] = temp;
  return i + 1;
}

void quick_sort_helper(Individuo *array, int izq, int der, CriterioOrdenamiento criterio) {
  if (izq < der) {
    int pi = partition(array, izq, der, criterio);
    quick_sort_helper(array, izq, pi - 1, criterio);
    quick_sort_helper(array, pi + 1, der, criterio);
  }
}

void quick_sort(Individuo *array, int izq, int der, CriterioOrdenamiento criterio) {
  quick_sort_helper(array, izq, der, criterio);
}

// HeapSort: O(n log n) garantizado
static int padre_idx(int i) { return (i - 1) / 2; }
static int hijo_izq_idx(int i) { return 2 * i + 1; }
static int hijo_der_idx(int i) { return 2 * i + 2; }

static void heapify_down(Individuo *array, int n, int i, CriterioOrdenamiento criterio) {
  int mayor = i;
  int izq = hijo_izq_idx(i);
  int der = hijo_der_idx(i);
  
  if (izq < n && comparar_individuos(&array[izq], &array[mayor], criterio) > 0)
    mayor = izq;
  if (der < n && comparar_individuos(&array[der], &array[mayor], criterio) > 0)
    mayor = der;
  
  if (mayor != i) {
    Individuo temp = array[i];
    array[i] = array[mayor];
    array[mayor] = temp;
    heapify_down(array, n, mayor, criterio);
  }
}

void heap_sort(Individuo *array, int n, CriterioOrdenamiento criterio) {
  // Construir el heap (Max-Heap)
  for (int i = n / 2 - 1; i >= 0; i--) {
    heapify_down(array, n, i, criterio);
  }
  
  // Extraer elementos uno por uno del heap
  for (int i = n - 1; i > 0; i--) {
    Individuo temp = array[0];
    array[0] = array[i];
    array[i] = temp;
    heapify_down(array, i, 0, criterio);
  }
}
