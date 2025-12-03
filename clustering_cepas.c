#include "clustering_cepas.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================
// SUBPROBLEMA 7: Clustering de Cepas
// Agrupacion por similitud con Trie - O(k*L)
// donde k = num cepas, L = longitud promedio de ADN
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
  strncpy(grupo.prefijo_comun, prefijo, MAX_ADN - 1);
  grupo.prefijo_comun[MAX_ADN - 1] = '\0';
  
  return grupo;
}

// ============================================================
// Algoritmo de Clustering Completo
// Estrategia: Agrupar cepas por prefijos comunes de 1-4 caracteres
// Complejidad: O(k * L) donde k = num cepas, L = longitud promedio
// ============================================================
GrupoVariantes* clustering_completo(Cepa *cepas, int num_cepas, int *num_grupos) {
  if (!cepas || !num_grupos || num_cepas <= 0) {
    *num_grupos = 0;
    return NULL;
  }
  
  GrupoVariantes *grupos = (GrupoVariantes *)malloc(num_cepas * sizeof(GrupoVariantes));
  int grupos_idx = 0;
  
  // Array auxiliar para rastrear cepas ya agrupadas
  bool *procesada = (bool *)malloc(num_cepas * sizeof(bool));
  memset(procesada, false, num_cepas * sizeof(bool));
  
  // Construir trie de todas las cepas - O(k * L)
  Trie *trie = construir_trie_cepas(cepas, num_cepas);
  
  // Iterar sobre cada cepa para encontrar grupos
  for (int i = 0; i < num_cepas; i++) {
    if (procesada[i]) continue;
    
    // Crear un grupo con la cepa actual
    GrupoVariantes grupo;
    grupo.cepas_grupo = (int *)malloc(num_cepas * sizeof(int));
    grupo.cantidad = 0;
    
    // Buscar todas las cepas con el mismo prefijo de longitud 2
    // (esto agrupa cepas con similaridad base)
    char prefijo[5];
    strncpy(prefijo, cepas[i].nombre_adn, 2);
    prefijo[2] = '\0';
    
    int cantidad_prefijo = 0;
    int *cepas_prefijo = trie_buscar_por_prefijo(trie, prefijo, &cantidad_prefijo);
    
    // Agregar las cepas encontradas al grupo
    if (cepas_prefijo) {
      for (int j = 0; j < cantidad_prefijo && grupo.cantidad < num_cepas; j++) {
        int cepa_id = cepas_prefijo[j];
        if (!procesada[cepa_id]) {
          grupo.cepas_grupo[grupo.cantidad++] = cepa_id;
          procesada[cepa_id] = true;
        }
      }
      free(cepas_prefijo);
    }
    
    // Si este grupo no tiene cepas (caso borde), agregar la cepa actual
    if (grupo.cantidad == 0) {
      grupo.cepas_grupo[grupo.cantidad++] = cepas[i].id;
      procesada[i] = true;
    }
    
    strncpy(grupo.prefijo_comun, prefijo, MAX_ADN - 1);
    grupo.prefijo_comun[MAX_ADN - 1] = '\0';
    
    grupos[grupos_idx++] = grupo;
  }
  
  // Liberar trie y array auxiliar
  trie_liberar(trie);
  free(procesada);
  
  *num_grupos = grupos_idx;
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

// ============================================================
// FUNCION DE PRUEBA: Test del algoritmo de Clustering
// ============================================================

void test_clustering_cepas(Cepa *cepas, int num_cepas) {
  if (!cepas || num_cepas <= 0) {
    printf("ERROR: Cepas invalidas\n");
    return;
  }
  
  printf("\n========== SUBPROBLEMA 7: CLUSTERING DE CEPAS ==========\n");
  printf("Cepas disponibles: %d\n", num_cepas);
  printf("Algoritmo: Trie para busqueda de prefijos comunes\n\n");
  
  // Prueba 1: Clustering por prefijo especifico
  printf("--- PRUEBA 1: Clustering por prefijos especificos ---\n");
  
  Trie *trie = construir_trie_cepas(cepas, num_cepas);
  
  const char *prefijos_prueba[] = {"A", "C", "G", "T", "AC", "AT", "CG"};
  int num_prefijos = 7;
  
  for (int i = 0; i < num_prefijos; i++) {
    GrupoVariantes grupo = clustering_por_prefijo(trie, prefijos_prueba[i], cepas);
    
    printf("Prefijo '%s': %d cepas encontradas\n", prefijos_prueba[i], grupo.cantidad);
    if (grupo.cantidad > 0 && grupo.cantidad <= 5) {
      printf("  Cepas: ");
      for (int j = 0; j < grupo.cantidad; j++) {
        printf("%s ", cepas[grupo.cepas_grupo[j]].nombre_adn);
      }
      printf("\n");
    } else if (grupo.cantidad > 5) {
      printf("  (primeras 5) ");
      for (int j = 0; j < 5; j++) {
        printf("%s ", cepas[grupo.cepas_grupo[j]].nombre_adn);
      }
      printf("...\n");
    }
    
    if (grupo.cepas_grupo) free(grupo.cepas_grupo);
  }
  
  // Prueba 2: Clustering completo
  printf("\n--- PRUEBA 2: Clustering completo por prefijos de 2 caracteres ---\n");
  
  int num_grupos = 0;
  GrupoVariantes *grupos = clustering_completo(cepas, num_cepas, &num_grupos);
  
  printf("Grupos identificados: %d\n\n", num_grupos);
  
  // Mostrar estadisticas de grupos
  printf("Grupo | Prefijo | Cantidad | Cepas en grupo\n");
  printf("------+---------+----------+----------------\n");
  
  for (int i = 0; i < num_grupos && i < 10; i++) {
    printf("%3d   | %6s  | %8d | ", i, grupos[i].prefijo_comun, grupos[i].cantidad);
    
    if (grupos[i].cantidad <= 3) {
      for (int j = 0; j < grupos[i].cantidad; j++) {
        printf("%s ", cepas[grupos[i].cepas_grupo[j]].nombre_adn);
      }
    } else {
      for (int j = 0; j < 2; j++) {
        printf("%s ", cepas[grupos[i].cepas_grupo[j]].nombre_adn);
      }
      printf("...");
    }
    printf("\n");
  }
  
  if (num_grupos > 10) {
    printf("... y %d grupos mas\n", num_grupos - 10);
  }
  
  // Estadisticas generales
  printf("\n--- ESTADISTICAS DE CLUSTERING ---\n");
  
  int cepas_agrupadas = 0;
  int grupo_mas_grande = 0;
  int grupo_mas_pequeno = num_cepas + 1;
  float promedio_grupo = 0;
  
  for (int i = 0; i < num_grupos; i++) {
    cepas_agrupadas += grupos[i].cantidad;
    if (grupos[i].cantidad > grupo_mas_grande) {
      grupo_mas_grande = grupos[i].cantidad;
    }
    if (grupos[i].cantidad < grupo_mas_pequeno) {
      grupo_mas_pequeno = grupos[i].cantidad;
    }
  }
  
  promedio_grupo = (float)cepas_agrupadas / num_grupos;
  
  printf("Total cepas procesadas: %d\n", cepas_agrupadas);
  printf("Grupo mas grande: %d cepas\n", grupo_mas_grande);
  printf("Grupo mas pequeno: %d cepas\n", grupo_mas_pequeno);
  printf("Promedio por grupo: %.2f cepas\n", promedio_grupo);
  printf("Fragmentacion: %.1f%% (1 - grupos/cepas)\n", 
         (1.0 - (float)num_grupos / num_cepas) * 100);
  
  printf("\nComplejidad Trie-insertar: O(k * L) = O(%d * 20)\n", num_cepas);
  printf("Complejidad busqueda-prefijo: O(L + M) = O(2 + M)\n");
  printf("Complejidad clustering-completo: O(k * L * log k)\n\n");
  
  printf("===== FIN PRUEBAS SUBPROBLEMA 7 =====\n\n");
  
  // Liberar memoria
  trie_liberar(trie);
  clustering_liberar(grupos, num_grupos);
}