#include "deteccion_brotes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// SUBPROBLEMA 2: Detección de Brotes
// BFS y DFS para encontrar componentes conectadas de infectados
// ============================================================

ResultadoDeteccionBrotes detectar_brotes_bfs(Territorio *territorios, int num_territorios, Individuo *poblacion, int num_individuos) {
  ResultadoDeteccionBrotes resultado;
  resultado.componentes = (int *)malloc(num_individuos * sizeof(int));
  resultado.num_componentes = 0;
  memset(resultado.componentes, -1, num_individuos * sizeof(int));
  memset(resultado.num_infectados_por_componente, 0, sizeof(resultado.num_infectados_por_componente));
  
  // TODO: Implementar BFS
  return resultado;
}

ResultadoDeteccionBrotes detectar_brotes_dfs(Territorio *territorios, int num_territorios, Individuo *poblacion, int num_individuos) {
  ResultadoDeteccionBrotes resultado;
  resultado.componentes = (int *)malloc(num_individuos * sizeof(int));
  resultado.num_componentes = 0;
  memset(resultado.componentes, -1, num_individuos * sizeof(int));
  memset(resultado.num_infectados_por_componente, 0, sizeof(resultado.num_infectados_por_componente));
  
  // TODO: Implementar DFS
  return resultado;
}

void deteccion_brotes_liberar(ResultadoDeteccionBrotes *resultado) {
  if (resultado && resultado->componentes) {
    free(resultado->componentes);
  }
}
