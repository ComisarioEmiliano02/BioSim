#include "minimizacion_riesgo.h"
#include <stdlib.h>
#include <string.h>

// ============================================================
// SUBPROBLEMA 4: Minimización de Riesgo (Aislamiento)
// Enfoque Greedy
// ============================================================

float* calcular_riesgo_propagacion(Individuo *poblacion, int num_individuos) {
  float *riesgo = (float *)malloc(num_individuos * sizeof(float));
  
  // TODO: Calcular riesgo para cada individuo
  for (int i = 0; i < num_individuos; i++) {
    riesgo[i] = 0.0;
  }
  
  return riesgo;
}

ResultadoAislamiento minimizar_riesgo_greedy(
  Individuo *poblacion,
  int num_individuos,
  float porcentaje_aislamiento
) {
  ResultadoAislamiento resultado;
  resultado.cantidad = (int)(num_individuos * porcentaje_aislamiento / 100.0);
  resultado.individuos_a_aislar = (int *)malloc(resultado.cantidad * sizeof(int));
  resultado.riesgo_reducido = 0.0;
  
  // TODO: Implementar Greedy
  return resultado;
}

void aislamiento_liberar(ResultadoAislamiento *resultado) {
  if (resultado && resultado->individuos_a_aislar) {
    free(resultado->individuos_a_aislar);
  }
}
