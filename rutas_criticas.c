#include "rutas_criticas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// ============================================================
// SUBPROBLEMA 5: Rutas Críticas
// Dijkstra modificado para máxima probabilidad de infección
// 
// Variantes de Dijkstra:
// - Tradicional: minimiza distancia
// - Modificado (nuestro): MAXIMIZA probabilidad acumulada
// 
// Cambios clave:
// 1. Max-Heap en lugar de Min-Heap
// 2. Maximizar probabilidad * peso en lugar de minimizar suma
// 3. Rastrear rutas además de probabilidades
// ============================================================

// Estructura auxiliar para Max-Heap (procesamiento de nodos)
typedef struct {
  int territorio_id;
  float probabilidad_acumulada;
  int distancia_saltos;  // Para desempates
} NodoHeap;

// Comparador para Max-Heap (máxima probabilidad primero)
static int comparar_max_heap(const void *a, const void *b) {
  const NodoHeap *nodo_a = (const NodoHeap *)a;
  const NodoHeap *nodo_b = (const NodoHeap *)b;
  
  // Max-Heap: mayor probabilidad va primero
  if (nodo_a->probabilidad_acumulada > nodo_b->probabilidad_acumulada) return -1;
  if (nodo_a->probabilidad_acumulada < nodo_b->probabilidad_acumulada) return 1;
  
  // Desempate: menos saltos es mejor
  if (nodo_a->distancia_saltos < nodo_b->distancia_saltos) return -1;
  if (nodo_a->distancia_saltos > nodo_b->distancia_saltos) return 1;
  
  return 0;
}

// ============================================================
// Dijkstra modificado para máxima probabilidad de una ruta
// Complejidad: O((n + m) log n) con Max-Heap
// 
// Idea: Encontrar la ruta donde la probabilidad acumulada
// de infección sea MÁXIMA (no mínima como en Dijkstra clásico)
// ============================================================
RutaCritica dijkstra_maxima_probabilidad(
  Grafo *grafo_territorios,
  int territorio_origen,
  int territorio_destino,
  Cepa *cepas,
  int num_cepas
) {
  RutaCritica ruta;
  ruta.ruta = NULL;
  ruta.longitud_ruta = 0;
  ruta.probabilidad_total = 0.0;
  
  if (!grafo_territorios || territorio_origen < 0 || 
      territorio_destino < 0 || territorio_origen >= grafo_territorios->num_nodos ||
      territorio_destino >= grafo_territorios->num_nodos) {
    return ruta;
  }
  
  int n = grafo_territorios->num_nodos;
  
  // Arrays para Dijkstra
  float *max_probabilidad = (float *)malloc(n * sizeof(float));
  int *padre = (int *)malloc(n * sizeof(int));
  bool *visitado = (bool *)malloc(n * sizeof(bool));
  
  // Inicialización
  for (int i = 0; i < n; i++) {
    max_probabilidad[i] = 0.0f;      // Iniciar con probabilidad 0
    padre[i] = -1;
    visitado[i] = false;
  }
  
  max_probabilidad[territorio_origen] = 1.0f;  // Probabilidad inicial = 1 (100%)
  
  // Cola de prioridades (Max-Heap) - simulado con array
  NodoHeap *heap = (NodoHeap *)malloc(n * sizeof(NodoHeap));
  int heap_size = 1;
  heap[0].territorio_id = territorio_origen;
  heap[0].probabilidad_acumulada = 1.0f;
  heap[0].distancia_saltos = 0;
  
  // Procesar nodos del heap
  while (heap_size > 0) {
    // Extraer máximo (primer elemento del Max-Heap ordenado)
    NodoHeap nodo_actual = heap[0];
    
    // Reorganizar heap (simulación - qsort)
    if (heap_size > 1) {
      heap[0] = heap[heap_size - 1];
    }
    heap_size--;
    
    int territorio_actual = nodo_actual.territorio_id;
    
    if (visitado[territorio_actual]) {
      continue;
    }
    
    visitado[territorio_actual] = true;
    
    // Si llegamos al destino, terminamos
    if (territorio_actual == territorio_destino) {
      break;
    }
    
    // Explorar vecinos
    NodoAdyacencia *vecino = grafo_territorios->listas[territorio_actual];
    while (vecino != NULL) {
      int territorio_vecino = vecino->destino_id;
      
      if (!visitado[territorio_vecino]) {
        // Calcular probabilidad de transmisión (basada en distancia/peso)
        // Fórmula: prob = 1 / (1 + peso/20) - mayor peso = menor probabilidad
        float prob_transmision = 1.0f / (1.0f + vecino->peso / 20.0f);
        
        // Usar beta promedio de todas las cepas para consistencia
        float beta_promedio = 0.5f;
        if (num_cepas > 0) {
          float suma_beta = 0.0f;
          for (int c = 0; c < num_cepas; c++) {
            suma_beta += cepas[c].beta;
          }
          beta_promedio = suma_beta / num_cepas;
        }
        
        prob_transmision *= beta_promedio;
        float nueva_probabilidad = max_probabilidad[territorio_actual] * prob_transmision;
        
        // Si encontramos mejor ruta (mayor probabilidad)
        if (nueva_probabilidad > max_probabilidad[territorio_vecino]) {
          max_probabilidad[territorio_vecino] = nueva_probabilidad;
          padre[territorio_vecino] = territorio_actual;
          
          // Añadir a heap si no está lleno
          if (heap_size < n) {
            heap[heap_size].territorio_id = territorio_vecino;
            heap[heap_size].probabilidad_acumulada = nueva_probabilidad;
            heap[heap_size].distancia_saltos = nodo_actual.distancia_saltos + 1;
            heap_size++;
            
            // Ordenar heap (simple - para Max-Heap)
            qsort(heap, heap_size, sizeof(NodoHeap), comparar_max_heap);
          }
        }
      }
      
      vecino = vecino->siguiente;
    }
  }
  
  // Reconstruir la ruta desde destino hacia origen
  ruta.probabilidad_total = max_probabilidad[territorio_destino];
  
  if (max_probabilidad[territorio_destino] > 0.0f) {
    // Contar nodos en la ruta
    int nodos_ruta = 0;
    int actual = territorio_destino;
    while (actual != -1) {
      nodos_ruta++;
      actual = padre[actual];
    }
    
    ruta.ruta = (int *)malloc(nodos_ruta * sizeof(int));
    ruta.longitud_ruta = nodos_ruta;
    
    // Llenar la ruta (en orden inverso)
    actual = territorio_destino;
    for (int i = nodos_ruta - 1; i >= 0; i--) {
      ruta.ruta[i] = actual;
      actual = padre[actual];
    }
  }
  
  // Liberar memoria
  free(max_probabilidad);
  free(padre);
  free(visitado);
  free(heap);
  
  return ruta;
}

// ============================================================
// Dijkstra múltiple: desde un origen a todos los destinos
// Complejidad: O((n + m) log n)
// ============================================================
RutaCritica* dijkstra_multiple(
  Grafo *grafo_territorios,
  int territorio_origen,
  Cepa *cepas,
  int num_cepas,
  int num_territorios
) {
  RutaCritica *rutas = (RutaCritica *)malloc(num_territorios * sizeof(RutaCritica));
  
  if (!rutas || !grafo_territorios) {
    return rutas;
  }
  
  // Ejecutar Dijkstra desde origen hacia cada destino
  for (int destino = 0; destino < num_territorios; destino++) {
    if (destino != territorio_origen) {
      rutas[destino] = dijkstra_maxima_probabilidad(
        grafo_territorios,
        territorio_origen,
        destino,
        cepas,
        num_cepas
      );
    } else {
      // El origen a sí mismo tiene probabilidad 1 y ruta vacía
      rutas[destino].ruta = NULL;
      rutas[destino].longitud_ruta = 0;
      rutas[destino].probabilidad_total = 1.0f;
    }
  }
  
  return rutas;
}

void ruta_critica_liberar(RutaCritica *ruta) {
  if (ruta && ruta->ruta) {
    free(ruta->ruta);
    ruta->ruta = NULL;
    ruta->longitud_ruta = 0;
  }
}

// ============================================================
// FUNCION DE PRUEBA: Test del algoritmo Dijkstra
// ============================================================

void test_rutas_criticas(
  Grafo *grafo_territorios,
  int num_territorios,
  Cepa *cepas,
  int num_cepas
) {
  if (!grafo_territorios || num_territorios <= 0) {
    printf("ERROR: Grafo o territorios inválidos\n");
    return;
  }
  
  printf("\n========== SUBPROBLEMA 5: RUTAS CRITICAS ==========\n");
  printf("Territorios: %d\n", num_territorios);
  printf("Cepas disponibles: %d\n", num_cepas);
  printf("Algoritmo: Dijkstra modificado para maxima probabilidad\n\n");
  
  // Prueba 1: Ruta de territorio 0 a territorio 5
  if (num_territorios > 5) {
    printf("--- PRUEBA 1: Ruta de Territorio 0 -> Territorio 5 ---\n");
    RutaCritica ruta_1 = dijkstra_maxima_probabilidad(
      grafo_territorios, 0, 5, cepas, num_cepas
    );
    
    printf("Probabilidad de infeccion: %.4f (%.2f%%)\n", 
           ruta_1.probabilidad_total, ruta_1.probabilidad_total * 100);
    printf("Saltos en la ruta: %d\n", ruta_1.longitud_ruta);
    
    if (ruta_1.ruta && ruta_1.longitud_ruta > 0) {
      printf("Ruta: ");
      for (int i = 0; i < ruta_1.longitud_ruta; i++) {
        printf("T%d", ruta_1.ruta[i]);
        if (i < ruta_1.longitud_ruta - 1) printf(" -> ");
      }
      printf("\n");
    }
    
    ruta_critica_liberar(&ruta_1);
  }
  
  // Prueba 2: Ruta de territorio 3 a territorio 15
  if (num_territorios > 15) {
    printf("\n--- PRUEBA 2: Ruta de Territorio 3 -> Territorio 15 ---\n");
    RutaCritica ruta_2 = dijkstra_maxima_probabilidad(
      grafo_territorios, 3, 15, cepas, num_cepas
    );
    
    printf("Probabilidad de infeccion: %.4f (%.2f%%)\n", 
           ruta_2.probabilidad_total, ruta_2.probabilidad_total * 100);
    printf("Saltos en la ruta: %d\n", ruta_2.longitud_ruta);
    
    if (ruta_2.ruta && ruta_2.longitud_ruta > 0) {
      printf("Ruta: ");
      for (int i = 0; i < ruta_2.longitud_ruta; i++) {
        printf("T%d", ruta_2.ruta[i]);
        if (i < ruta_2.longitud_ruta - 1) printf(" -> ");
      }
      printf("\n");
    }
    
    ruta_critica_liberar(&ruta_2);
  }
  
  // Prueba 3: Multiples rutas desde territorio 0
  printf("\n--- PRUEBA 3: Rutas desde Territorio 0 hacia todos ---\n");
  printf("(Mostrando top 5 destinos con maxima probabilidad)\n\n");
  
  RutaCritica *rutas_multiples = dijkstra_multiple(
    grafo_territorios, 0, cepas, num_cepas, num_territorios
  );
  
  // Buscar los 5 destinos con máxima probabilidad
  typedef struct {
    int territorio;
    float probabilidad;
  } ResultadoOrdenado;
  
  ResultadoOrdenado *resultados = (ResultadoOrdenado *)malloc(num_territorios * sizeof(ResultadoOrdenado));
  
  for (int i = 0; i < num_territorios; i++) {
    resultados[i].territorio = i;
    resultados[i].probabilidad = rutas_multiples[i].probabilidad_total;
  }
  
  // Ordenar por probabilidad descendente (simple)
  for (int i = 0; i < num_territorios - 1; i++) {
    for (int j = i + 1; j < num_territorios; j++) {
      if (resultados[j].probabilidad > resultados[i].probabilidad) {
        ResultadoOrdenado temp = resultados[i];
        resultados[i] = resultados[j];
        resultados[j] = temp;
      }
    }
  }
  
  // Mostrar top 5
  printf("Territorio | Probabilidad | Saltos\n");
  printf("-----------+--------------+-------\n");
  
  int mostrar = (num_territorios < 6) ? num_territorios : 6;
  for (int i = 0; i < mostrar; i++) {
    if (resultados[i].territorio != 0) {  // Saltar origen
      printf("T%-9d | %12.4f | %6d\n",
             resultados[i].territorio,
             resultados[i].probabilidad,
             rutas_multiples[resultados[i].territorio].longitud_ruta);
    }
  }
  
  // Estadísticas generales
  printf("\n--- ESTADISTICAS GENERALES ---\n");
  float probabilidad_promedio = 0.0f;
  int total_rutas_validas = 0;
  
  for (int i = 0; i < num_territorios; i++) {
    if (i != 0 && rutas_multiples[i].probabilidad_total > 0.0f) {
      probabilidad_promedio += rutas_multiples[i].probabilidad_total;
      total_rutas_validas++;
    }
  }
  
  if (total_rutas_validas > 0) {
    probabilidad_promedio /= total_rutas_validas;
  }
  
  printf("Rutas alcanzables desde T0: %d/%d (%.1f%%)\n",
         total_rutas_validas, num_territorios - 1,
         (float)total_rutas_validas / (num_territorios - 1) * 100);
  printf("Probabilidad promedio: %.4f (%.2f%%)\n",
         probabilidad_promedio, probabilidad_promedio * 100);
  
  printf("\nComplejidad: O((n + m) log n) donde n=%d territorios\n", num_territorios);
  printf("Algoritmo: Dijkstra modificado con Max-Heap\n");
  
  printf("\n===== FIN PRUEBAS SUBPROBLEMA 5 =====\n\n");
  
  // Liberar memoria
  for (int i = 0; i < num_territorios; i++) {
    ruta_critica_liberar(&rutas_multiples[i]);
  }
  free(rutas_multiples);
  free(resultados);
}
