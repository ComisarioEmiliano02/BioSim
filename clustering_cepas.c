#include "clustering_cepas.h"
#include <stdlib.h>
#include <string.h>

// ============================================================
// SUBPROBLEMA 7: Clustering de Cepas
// Agrupación por similitud con Trie
// ============================================================

Trie* construir_trie_cepas(Cepa *cepas, int num_cepas) {
  Trie *trie = trie_crear();
  
  for (int i = 0; i < num_cepas; i++) {
    trie_insertar(trie, cepas[i].nombre_adn, cepas[i].id);
  }
  
  return trie;
}

GrupoVariantes clustering_por_prefijo(Trie *trie, const char *prefijo, Cepa *cepas) {
  GrupoVariantes grupo;
  int cantidad = 0;
  grupo.cepas_grupo = trie_buscar_por_prefijo(trie, prefijo, &cantidad);
  grupo.cantidad = cantidad;
  strcpy(grupo.prefijo_comun, prefijo);
  
  return grupo;
}

GrupoVariantes* clustering_completo(Cepa *cepas, int num_cepas, int *num_grupos) {
  GrupoVariantes *grupos = (GrupoVariantes *)malloc(num_cepas * sizeof(GrupoVariantes));
  *num_grupos = 0;
  
  // TODO: Implementar clustering completo
  return grupos;
}

void clustering_liberar(GrupoVariantes *grupos, int num_grupos) {
  if (!grupos) return;
  
  for (int i = 0; i < num_grupos; i++) {
    if (grupos[i].cepas_grupo) {
      free(grupos[i].cepas_grupo);
    }
  }
  free(grupos);
}
