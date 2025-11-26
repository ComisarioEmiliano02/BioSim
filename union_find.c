#include "union_find.h"
#include <stdlib.h>

// ============================================================
// IMPLEMENTACIÓN UNION-FIND (Disjoint Set Union)
// Con compresión de camino y unión por rango
// ============================================================

UnionFind* union_find_crear(int n) {
  UnionFind *uf = (UnionFind *)malloc(sizeof(UnionFind));
  uf->num_elementos = n;
  uf->padre = (int *)malloc(n * sizeof(int));
  uf->rango = (int *)malloc(n * sizeof(int));
  
  for (int i = 0; i < n; i++) {
    uf->padre[i] = i;
    uf->rango[i] = 0;
  }
  
  return uf;
}

int union_find_buscar(UnionFind *uf, int x) {
  if (!uf || x < 0 || x >= uf->num_elementos) return -1;
  
  // Compresión de camino
  if (uf->padre[x] != x) {
    uf->padre[x] = union_find_buscar(uf, uf->padre[x]);
  }
  
  return uf->padre[x];
}

bool union_find_unir(UnionFind *uf, int x, int y) {
  if (!uf || x < 0 || x >= uf->num_elementos || y < 0 || y >= uf->num_elementos) {
    return false;
  }
  
  int raiz_x = union_find_buscar(uf, x);
  int raiz_y = union_find_buscar(uf, y);
  
  if (raiz_x == raiz_y) {
    return false;  // Ya están en el mismo conjunto
  }
  
  // Unión por rango
  if (uf->rango[raiz_x] < uf->rango[raiz_y]) {
    uf->padre[raiz_x] = raiz_y;
  } else if (uf->rango[raiz_x] > uf->rango[raiz_y]) {
    uf->padre[raiz_y] = raiz_x;
  } else {
    uf->padre[raiz_y] = raiz_x;
    uf->rango[raiz_x]++;
  }
  
  return true;
}

bool union_find_mismo_conjunto(UnionFind *uf, int x, int y) {
  if (!uf) return false;
  
  return union_find_buscar(uf, x) == union_find_buscar(uf, y);
}

void union_find_liberar(UnionFind *uf) {
  if (!uf) return;
  
  free(uf->padre);
  free(uf->rango);
  free(uf);
}
