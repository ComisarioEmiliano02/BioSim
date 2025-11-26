#ifndef MINIMIZACION_RIESGO_H
#define MINIMIZACION_RIESGO_H

#include "estructuras.h"

// ============================================================
// SUBPROBLEMA 4: Minimización de Riesgo (Aislamiento)
// Seleccionar a quién aislar para minimizar propagación
// Restricción: < O(n^2)
// Algoritmo: Enfoque Voraz (Greedy) basado en ordenamiento
// ============================================================

typedef struct {
  int *individuos_a_aislar;
  int cantidad;
  float riesgo_reducido;
} ResultadoAislamiento;

/**
 * Selecciona individuos a aislar usando enfoque Greedy
 * Ordena por riesgo de propagación y aísla los más peligrosos
 * Complejidad: O(n log n) por ordenamiento + O(n) por selección = O(n log n)
 * porcentaje_aislamiento: qué porcentaje de la población aislar (0-100)
 * Retorna: Array con IDs de individuos a aislar
 */
ResultadoAislamiento minimizar_riesgo_greedy(
  Individuo *poblacion,
  int num_individuos,
  float porcentaje_aislamiento
);

/**
 * Calcula el riesgo de propagación de cada individuo
 * Basado en: estado, tiempo de infección, riesgo personal
 * Complejidad: O(n)
 */
float* calcular_riesgo_propagacion(Individuo *poblacion, int num_individuos);

/**
 * Libera los resultados del aislamiento
 * Complejidad: O(1)
 */
void aislamiento_liberar(ResultadoAislamiento *resultado);

#endif // MINIMIZACION_RIESGO_H
