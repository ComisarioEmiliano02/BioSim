#include "propagacion_temporal.h"
#include <stdio.h>
#include <stdlib.h>

// ============================================================
// SUBPROBLEMA 3: Propagación Temporal
// Simulación con eventos en Min-Heap
// ============================================================

ResultadoSimulacion simular_propagacion(
  Individuo *poblacion, 
  int num_individuos,
  Cepa *cepas,
  int num_cepas,
  Territorio *territorios,
  int num_territorios,
  Grafo *grafo_territorios,
  int dias
) {
  ResultadoSimulacion resultado;
  resultado.estadisticas = (EstadisticaDia *)malloc(dias * sizeof(EstadisticaDia));
  resultado.dias_simulados = 0;
  resultado.capacidad = dias;
  
  // TODO: Implementar simulacion con Min-Heap
  return resultado;
}

void simulacion_liberar(ResultadoSimulacion *resultado) {
  if (resultado && resultado->estadisticas) {
    free(resultado->estadisticas);
  }
}
