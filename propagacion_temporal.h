#ifndef PROPAGACION_TEMPORAL_H
#define PROPAGACION_TEMPORAL_H

#include "estructuras.h"
#include "heap.h"

// ============================================================
// SUBPROBLEMA 3: Propagación Temporal
// Simular contagios día a día
// Restricción: <= O(n log n)
// Estructura: Min-Heap para gestionar eventos cronológicos
// ============================================================

typedef struct {
  int dia;
  int susceptibles;
  int infectados;
  int recuperados;
  int fallecidos;
} EstadisticaDia;

typedef struct {
  EstadisticaDia *estadisticas;
  int dias_simulados;
  int capacidad;
} ResultadoSimulacion;

/**
 * Simula la propagación de la epidemia día a día
 * Usa Min-Heap para procesar eventos en orden cronológico
 * Complejidad: O(n log n) donde n es número total de eventos
 * días: número de días a simular
 * Retorna: Estadísticas diarias de la simulación
 */
ResultadoSimulacion simular_propagacion(
  Individuo *poblacion, 
  int num_individuos,
  Cepa *cepas,
  int num_cepas,
  Territorio *territorios,
  int num_territorios,
  Grafo *grafo_territorios,
  int dias
);

/**
 * Libera los resultados de la simulación
 * Complejidad: O(1)
 */
void simulacion_liberar(ResultadoSimulacion *resultado);

#endif // PROPAGACION_TEMPORAL_H
