#ifndef PROPAGACION_TEMPORAL_H
#define PROPAGACION_TEMPORAL_H

#include "estructuras.h"

// ============================================================
// SUBPROBLEMA 3: Propagación Temporal
// Simular contagios día a día usando eventos y Min-Heap
// Restricción: <= O(n log n)
// Estructura: Min-Heap para gestionar eventos cronológicos
// ============================================================

/**
 * Simula la propagación temporal de la infección
 * Usa Min-Heap para procesar eventos en orden cronológico
 * Complejidad: O(n log n) donde n es número total de eventos
 * 
 * Parámetros:
 *   - territorios: array de territorios
 *   - num_territorios: cantidad de territorios
 *   - poblacion: array de individuos
 *   - num_poblacion: cantidad de individuos
 *   - cepas: array de cepas virales
 *   - num_cepas: cantidad de cepas
 *   - dias_simulacion: número de días a simular
 * 
 * Retorna: ResultadoPropagacion con estadísticas de la simulación
 */
ResultadoPropagacion* simular_propagacion_temporal(
  Territorio *territorios,
  int num_territorios,
  Individuo *poblacion,
  int num_poblacion,
  Cepa *cepas,
  int num_cepas,
  int dias_simulacion
);

/**
 * Libera los resultados de la simulación
 * Complejidad: O(1)
 */
void liberar_resultado_propagacion(ResultadoPropagacion *resultado);

/**
 * Función de prueba para Subproblema 3
 * Ejecuta simulación y muestra resultados
 * Complejidad: O(n log n)
 */
void test_propagacion_temporal(Territorio *territorios, int num_territorios,
                               Individuo *poblacion, int num_poblacion,
                               Cepa *cepas, int num_cepas);

#endif // PROPAGACION_TEMPORAL_H
