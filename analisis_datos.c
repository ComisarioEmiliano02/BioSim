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
// COMENTARIO Big-O: Construcción del heap O(n) + extracción de n elementos O(n log n) = O(n log n) total
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
  // O(n) operación de heapificación
  for (int i = n / 2 - 1; i >= 0; i--) {
    heapify_down(array, n, i, criterio);
  }
  
  // Extraer elementos uno por uno del heap
  // O(n log n): n iteraciones, cada una O(log n)
  for (int i = n - 1; i > 0; i--) {
    Individuo temp = array[0];
    array[0] = array[i];
    array[i] = temp;
    heapify_down(array, i, 0, criterio);
  }
}

// ============================================================
// FUNCION DE PRUEBA: Test de los tres algoritmos
// ============================================================

void test_analisis_datos(Individuo *poblacion, int num_individuos) {
  if (!poblacion || num_individuos <= 0) {
    printf("ERROR: Poblacion invalida para test\n");
    return;
  }
  
  printf("\n========== SUBPROBLEMA 1: ANALISIS DE DATOS ==========\n");
  printf("Población: %d individuos\n\n", num_individuos);
  
  // Crear copias de la poblacion para cada algoritmo
  Individuo *copia_merge = (Individuo *)malloc(num_individuos * sizeof(Individuo));
  Individuo *copia_quick = (Individuo *)malloc(num_individuos * sizeof(Individuo));
  Individuo *copia_heap = (Individuo *)malloc(num_individuos * sizeof(Individuo));
  
  if (!copia_merge || !copia_quick || !copia_heap) {
    printf("ERROR: No se pudo asignar memoria para copias\n");
    free(copia_merge);
    free(copia_quick);
    free(copia_heap);
    return;
  }
  
  // ===== PRUEBA 1: ORDENAR POR RIESGO =====
  printf("--- PRUEBA 1: Ordenar por RIESGO ---\n");
  
  // Copiar datos
  memcpy(copia_merge, poblacion, num_individuos * sizeof(Individuo));
  memcpy(copia_quick, poblacion, num_individuos * sizeof(Individuo));
  memcpy(copia_heap, poblacion, num_individuos * sizeof(Individuo));
  
  // MergeSort
  merge_sort(copia_merge, 0, num_individuos - 1, ORDENAR_POR_RIESGO);
  
  // QuickSort
  quick_sort(copia_quick, 0, num_individuos - 1, ORDENAR_POR_RIESGO);
  
  // HeapSort
  heap_sort(copia_heap, num_individuos, ORDENAR_POR_RIESGO);
  
  printf("MergeSort - Últimos 5 individuos por riesgo (MAYOR RIESGO):\n");
  for (int i = num_individuos - 5; i < num_individuos && i >= 0; i++) {
    printf("  %s: riesgo=%d\n", copia_merge[i].nombre, copia_merge[i].riesgo);
  }
  
  printf("\nQuickSort - Últimos 5 individuos por riesgo (MAYOR RIESGO):\n");
  for (int i = num_individuos - 5; i < num_individuos && i >= 0; i++) {
    printf("  %s: riesgo=%d\n", copia_quick[i].nombre, copia_quick[i].riesgo);
  }
  
  printf("\nHeapSort - Últimos 5 individuos por riesgo (MAYOR RIESGO):\n");
  for (int i = num_individuos - 5; i < num_individuos && i >= 0; i++) {
    printf("  %s: riesgo=%d\n", copia_heap[i].nombre, copia_heap[i].riesgo);
  }
  
  // ===== PRUEBA 2: ORDENAR POR TIEMPO DE INFECCION =====
  printf("\n--- PRUEBA 2: Ordenar por TIEMPO DE INFECCION ---\n");
  
  // Copiar datos
  memcpy(copia_merge, poblacion, num_individuos * sizeof(Individuo));
  memcpy(copia_quick, poblacion, num_individuos * sizeof(Individuo));
  memcpy(copia_heap, poblacion, num_individuos * sizeof(Individuo));
  
  // MergeSort
  merge_sort(copia_merge, 0, num_individuos - 1, ORDENAR_POR_TIEMPO);
  
  // QuickSort
  quick_sort(copia_quick, 0, num_individuos - 1, ORDENAR_POR_TIEMPO);
  
  // HeapSort
  heap_sort(copia_heap, num_individuos, ORDENAR_POR_TIEMPO);
  
  printf("MergeSort - Últimos 5 infectados (mayor tiempo):\n");
  for (int i = num_individuos - 5; i < num_individuos && i >= 0; i++) {
    printf("  %s: tiempo_infeccion=%d\n", copia_merge[i].nombre, copia_merge[i].tiempo_infeccion);
  }
  
  printf("\nQuickSort - Últimos 5 infectados (mayor tiempo):\n");
  for (int i = num_individuos - 5; i < num_individuos && i >= 0; i++) {
    printf("  %s: tiempo_infeccion=%d\n", copia_quick[i].nombre, copia_quick[i].tiempo_infeccion);
  }
  
  printf("\nHeapSort - Últimos 5 infectados (mayor tiempo):\n");
  for (int i = num_individuos - 5; i < num_individuos && i >= 0; i++) {
    printf("  %s: tiempo_infeccion=%d\n", copia_heap[i].nombre, copia_heap[i].tiempo_infeccion);
  }
  
  // ===== PRUEBA 3: ORDENAR POR NOMBRE =====
  printf("\n--- PRUEBA 3: Ordenar por NOMBRE (alfabetico) ---\n");
  
  // Copiar datos
  memcpy(copia_merge, poblacion, num_individuos * sizeof(Individuo));
  memcpy(copia_quick, poblacion, num_individuos * sizeof(Individuo));
  memcpy(copia_heap, poblacion, num_individuos * sizeof(Individuo));
  
  // MergeSort
  merge_sort(copia_merge, 0, num_individuos - 1, ORDENAR_POR_NOMBRE);
  
  // QuickSort
  quick_sort(copia_quick, 0, num_individuos - 1, ORDENAR_POR_NOMBRE);
  
  // HeapSort
  heap_sort(copia_heap, num_individuos, ORDENAR_POR_NOMBRE);
  
  printf("MergeSort - Primeros 5 por nombre:\n");
  for (int i = 0; i < 5 && i < num_individuos; i++) {
    printf("  %s\n", copia_merge[i].nombre);
  }
  
  printf("\nQuickSort - Primeros 5 por nombre:\n");
  for (int i = 0; i < 5 && i < num_individuos; i++) {
    printf("  %s\n", copia_quick[i].nombre);
  }
  
  printf("\nHeapSort - Primeros 5 por nombre:\n");
  for (int i = 0; i < 5 && i < num_individuos; i++) {
    printf("  %s\n", copia_heap[i].nombre);
  }
  
  printf("\n===== FIN PRUEBAS SUBPROBLEMA 1 =====\n\n");
  
  // Liberar memoria
  free(copia_merge);
  free(copia_quick);
  free(copia_heap);
}
