#include "contencion_vacunacion.h"
#include <stdlib.h>

// ============================================================
// SUBPROBLEMA 6: Contención (Vacunación)
// Kruskal y Prim para MST
// ============================================================

ResultadoMST kruskal_mst(Grafo *grafo_territorios, int num_territorios) {
  ResultadoMST resultado;
  resultado.aristas_mst = (AristaTerritorios *)malloc(num_territorios * sizeof(AristaTerritorios));
  resultado.num_aristas = 0;
  resultado.peso_total = 0.0;
  
  // TODO: Implementar Kruskal
  return resultado;
}

ResultadoMST prim_mst(Grafo *grafo_territorios, int num_territorios, int territorio_inicio) {
  ResultadoMST resultado;
  resultado.aristas_mst = (AristaTerritorios *)malloc(num_territorios * sizeof(AristaTerritorios));
  resultado.num_aristas = 0;
  resultado.peso_total = 0.0;
  
  // TODO: Implementar Prim
  return resultado;
}

void mst_liberar(ResultadoMST *resultado) {
  if (resultado && resultado->aristas_mst) {
    free(resultado->aristas_mst);
  }
}
