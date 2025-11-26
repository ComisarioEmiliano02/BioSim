#include "heap.h"
#include <stdlib.h>
#include <string.h>

// ============================================================
// IMPLEMENTACION HEAP (Min-Heap y Max-Heap)
// ============================================================

static int padre(int i) { return (i - 1) / 2; }
static int hijo_izq(int i) { return 2 * i + 1; }
static int hijo_der(int i) { return 2 * i + 2; }

static void intercambiar(ElementoHeap *a, ElementoHeap *b) {
  ElementoHeap temp = *a;
  *a = *b;
  *b = temp;
}

static bool comparar(ElementoHeap a, ElementoHeap b, bool es_min) {
  return es_min ? (a.prioridad < b.prioridad) : (a.prioridad > b.prioridad);
}

Heap* heap_crear(int capacidad_inicial, bool es_min_heap) {
  Heap *heap = (Heap *)malloc(sizeof(Heap));
  heap->capacidad = capacidad_inicial;
  heap->tamano = 0;
  heap->es_min_heap = es_min_heap;
  heap->elementos = (ElementoHeap *)malloc(capacidad_inicial * sizeof(ElementoHeap));
  return heap;
}

void heap_insertar(Heap *heap, int individuo_id, int prioridad, void *datos) {
  if (!heap) return;
  
  // Expandir si es necesario
  if (heap->tamano >= heap->capacidad) {
    heap->capacidad *= 2;
    heap->elementos = (ElementoHeap *)realloc(heap->elementos, heap->capacidad * sizeof(ElementoHeap));
  }
  
  // Insertar al final
  heap->elementos[heap->tamano].individuo_id = individuo_id;
  heap->elementos[heap->tamano].prioridad = prioridad;
  heap->elementos[heap->tamano].datos = datos;
  
  // Bubble up
  int actual = heap->tamano;
  while (actual > 0 && comparar(heap->elementos[actual], heap->elementos[padre(actual)], heap->es_min_heap)) {
    intercambiar(&heap->elementos[actual], &heap->elementos[padre(actual)]);
    actual = padre(actual);
  }
  
  heap->tamano++;
}

ElementoHeap heap_extraer(Heap *heap) {
  ElementoHeap vacio = {-1, 0, NULL};
  
  if (!heap || heap->tamano == 0) {
    return vacio;
  }
  
  ElementoHeap raiz = heap->elementos[0];
  heap->elementos[0] = heap->elementos[heap->tamano - 1];
  heap->tamano--;
  
  // Bubble down
  int actual = 0;
  while (hijo_izq(actual) < heap->tamano) {
    int menor_mayor = hijo_izq(actual);
    
    if (hijo_der(actual) < heap->tamano &&
        comparar(heap->elementos[hijo_der(actual)], heap->elementos[hijo_izq(actual)], heap->es_min_heap)) {
      menor_mayor = hijo_der(actual);
    }
    
    if (comparar(heap->elementos[menor_mayor], heap->elementos[actual], heap->es_min_heap)) {
      intercambiar(&heap->elementos[actual], &heap->elementos[menor_mayor]);
      actual = menor_mayor;
    } else {
      break;
    }
  }
  
  return raiz;
}

ElementoHeap heap_peek(Heap *heap) {
  ElementoHeap vacio = {-1, 0, NULL};
  
  if (!heap || heap->tamano == 0) {
    return vacio;
  }
  
  return heap->elementos[0];
}

bool heap_vacio(Heap *heap) {
  return !heap || heap->tamano == 0;
}

int heap_tamano(Heap *heap) {
  return heap ? heap->tamano : 0;
}

void heap_liberar(Heap *heap) {
  if (!heap) return;
  
  free(heap->elementos);
  free(heap);
}
