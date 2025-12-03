#include "trie.h"
#include <stdlib.h>
#include <string.h>

// ============================================================
// IMPLEMENTACIÓN TRIE
// Mapeo: A=0, C=1, G=2, T=3
// ============================================================

static int adn_a_indice(char base) {
  switch (base) {
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'T': return 3;
    default: return -1;
  }
}

static char indice_a_adn(int indice) {
  const char bases[] = {'A', 'C', 'G', 'T'};
  return (indice >= 0 && indice < 4) ? bases[indice] : '?';
}

Trie* trie_crear() {
  Trie *trie = (Trie *)malloc(sizeof(Trie));
  trie->raiz = (NodoTrie *)calloc(1, sizeof(NodoTrie));
  trie->raiz->es_final = false;
  trie->raiz->cepa_id = -1;
  return trie;
}

void trie_insertar(Trie *trie, const char *adn, int cepa_id) {
  if (!trie || !adn) return;
  
  NodoTrie *actual = trie->raiz;
  
  for (int i = 0; adn[i] != '\0'; i++) {
    int indice = adn_a_indice(adn[i]);
    if (indice < 0) continue;  // Ignorar bases inválidas
    
    if (actual->hijos[indice] == NULL) {
      actual->hijos[indice] = (NodoTrie *)calloc(1, sizeof(NodoTrie));
      actual->hijos[indice]->es_final = false;
      actual->hijos[indice]->cepa_id = -1;
    }
    
    actual = actual->hijos[indice];
  }
  
  actual->es_final = true;
  actual->cepa_id = cepa_id;
}

int trie_buscar(Trie *trie, const char *adn) {
  if (!trie || !adn) return -1;
  
  NodoTrie *actual = trie->raiz;
  
  for (int i = 0; adn[i] != '\0'; i++) {
    int indice = adn_a_indice(adn[i]);
    if (indice < 0 || actual->hijos[indice] == NULL) {
      return -1;
    }
    actual = actual->hijos[indice];
  }
  
  return actual->es_final ? actual->cepa_id : -1;
}

int* trie_buscar_por_prefijo(Trie *trie, const char *prefijo, int *cantidad) {
  if (!trie || !prefijo || !cantidad) {
    if (cantidad) *cantidad = 0;
    return NULL;
  }
  
  // Navegar hasta el nodo del prefijo
  NodoTrie *actual = trie->raiz;
  for (int i = 0; prefijo[i] != '\0'; i++) {
    int indice = adn_a_indice(prefijo[i]);
    if (indice < 0 || actual->hijos[indice] == NULL) {
      *cantidad = 0;
      return NULL;
    }
    actual = actual->hijos[indice];
  }
  
  // Contar cepas que comienzan con este prefijo (DFS)
  int *resultados = (int *)malloc(sizeof(int) * 50);  // Max 50 cepas
  int idx = 0;
  
  // DFS recursivo para recolectar todas las cepas bajo este nodo
  void _dfs_recolectar(NodoTrie *nodo) {
    if (nodo == NULL) return;
    if (nodo->es_final && idx < 50) {
      resultados[idx++] = nodo->cepa_id;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      if (nodo->hijos[i] != NULL) {
        _dfs_recolectar(nodo->hijos[i]);
      }
    }
  }
  
  _dfs_recolectar(actual);
  *cantidad = idx;
  return resultados;
}

static void trie_liberar_recurivo(NodoTrie *nodo) {
  if (!nodo) return;
  
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (nodo->hijos[i] != NULL) {
      trie_liberar_recurivo(nodo->hijos[i]);
    }
  }
  
  free(nodo);
}

void trie_liberar(Trie *trie) {
  if (!trie) return;
  
  trie_liberar_recurivo(trie->raiz);
  free(trie);
}
