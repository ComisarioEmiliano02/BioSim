#include "contencion_vacunacion.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ============================================================
// SUBPROBLEMA 6: Contencion (Vacunacion)
// Kruskal con Union-Find para MST
// Complejidad: O(m log m) donde m = numero de aristas
// ============================================================

// Estructura auxiliar para comparar aristas
typedef struct {
  int u;
  int v;
  float peso;
} AristaOrdenada;

// Comparador para qsort (ordenar aristas por peso ascendente)
static int comparar_aristas(const void *a, const void *b) {
  const AristaOrdenada *arista_a = (const AristaOrdenada *)a;
  const AristaOrdenada *arista_b = (const AristaOrdenada *)b;
  
  if (arista_a->peso < arista_b->peso) return -1;
  if (arista_a->peso > arista_b->peso) return 1;
  return 0;
}

// ============================================================
// Algoritmo de Kruskal para MST
// Complejidad: O(m log m) por ordenamiento + O(m * alpha(n)) por Union-Find
//            = O(m log m) donde alpha(n) es prácticamente constante
//
// Idea:
//   1. Ordenar todas las aristas por peso (distancia)
//   2. Iterar sobre aristas ordenadas
//   3. Si u y v están en conjuntos diferentes, agregarla al MST
//   4. Unir los conjuntos de u y v
//   5. Repetir hasta n-1 aristas en MST
// ============================================================
ResultadoMST kruskal_mst(Grafo *grafo_territorios, int num_territorios) {
  ResultadoMST resultado;
  resultado.aristas_mst = (AristaTerritorios *)malloc((num_territorios - 1) * sizeof(AristaTerritorios));
  resultado.num_aristas = 0;
  resultado.peso_total = 0.0;
  
  if (!grafo_territorios || num_territorios <= 0) {
    return resultado;
  }
  
  // Paso 1: Extraer todas las aristas del grafo - O(m)
  AristaOrdenada *todas_aristas = (AristaOrdenada *)malloc(grafo_territorios->num_nodos * 100 * sizeof(AristaOrdenada));
  int num_total_aristas = 0;
  
  for (int i = 0; i < num_territorios; i++) {
    NodoAdyacencia *actual = grafo_territorios->listas[i];
    while (actual != NULL) {
      todas_aristas[num_total_aristas].u = i;
      todas_aristas[num_total_aristas].v = actual->destino_id;
      todas_aristas[num_total_aristas].peso = actual->peso;
      num_total_aristas++;
      actual = actual->siguiente;
    }
  }
  
  // Paso 2: Ordenar aristas por peso - O(m log m)
  qsort(todas_aristas, num_total_aristas, sizeof(AristaOrdenada), comparar_aristas);
  
  // Paso 3: Crear Union-Find para detectar ciclos - O(n)
  UnionFind *uf = union_find_crear(num_territorios);
  
  // Paso 4: Algoritmo de Kruskal - O(m * alpha(n))
  for (int i = 0; i < num_total_aristas && resultado.num_aristas < num_territorios - 1; i++) {
    int u = todas_aristas[i].u;
    int v = todas_aristas[i].v;
    float peso = todas_aristas[i].peso;
    
    // Encontrar representantes de u y v
    int raiz_u = union_find_buscar(uf, u);
    int raiz_v = union_find_buscar(uf, v);
    
    // Si están en diferentes componentes, agregar arista
    if (raiz_u != raiz_v) {
      // Agregar al MST
      resultado.aristas_mst[resultado.num_aristas].territorio_1 = u;
      resultado.aristas_mst[resultado.num_aristas].territorio_2 = v;
      resultado.aristas_mst[resultado.num_aristas].peso = peso;
      
      resultado.peso_total += peso;
      resultado.num_aristas++;
      
      // Unir componentes
      union_find_unir(uf, raiz_u, raiz_v);
    }
  }
  
  // Liberar memoria
  free(todas_aristas);
  union_find_liberar(uf);
  
  return resultado;
}

// ============================================================
// Algoritmo de Prim para MST (versión simple sin Heap)
// Complejidad: O(n^2) versión simple, O((n+m) log n) con Heap
// ============================================================
ResultadoMST prim_mst(Grafo *grafo_territorios, int num_territorios, int territorio_inicio) {
  ResultadoMST resultado;
  resultado.aristas_mst = (AristaTerritorios *)malloc((num_territorios - 1) * sizeof(AristaTerritorios));
  resultado.num_aristas = 0;
  resultado.peso_total = 0.0;
  
  if (!grafo_territorios || num_territorios <= 0 || 
      territorio_inicio < 0 || territorio_inicio >= num_territorios) {
    return resultado;
  }
  
  // Arrays para rastrear nodos visitados y distancias minimas
  bool *visitado = (bool *)malloc(num_territorios * sizeof(bool));
  float *distancia_minima = (float *)malloc(num_territorios * sizeof(float));
  int *padre = (int *)malloc(num_territorios * sizeof(int));
  
  // Inicializar
  for (int i = 0; i < num_territorios; i++) {
    visitado[i] = false;
    distancia_minima[i] = 1e9;  // Infinito
    padre[i] = -1;
  }
  
  distancia_minima[territorio_inicio] = 0.0;
  
  // Construir MST - O(n^2)
  for (int i = 0; i < num_territorios - 1; i++) {
    // Encontrar nodo no visitado con distancia minima
    int u = -1;
    float min_dist = 1e9;
    
    for (int j = 0; j < num_territorios; j++) {
      if (!visitado[j] && distancia_minima[j] < min_dist) {
        u = j;
        min_dist = distancia_minima[j];
      }
    }
    
    if (u == -1) break;  // No hay mas nodos alcanzables
    
    visitado[u] = true;
    
    // Si tiene padre, agregar arista al MST
    if (padre[u] != -1) {
      resultado.aristas_mst[resultado.num_aristas].territorio_1 = padre[u];
      resultado.aristas_mst[resultado.num_aristas].territorio_2 = u;
      resultado.aristas_mst[resultado.num_aristas].peso = distancia_minima[u];
      
      resultado.peso_total += distancia_minima[u];
      resultado.num_aristas++;
    }
    
    // Actualizar distancias a los vecinos de u
    NodoAdyacencia *vecino = grafo_territorios->listas[u];
    while (vecino != NULL) {
      int v = vecino->destino_id;
      float peso = vecino->peso;
      
      if (!visitado[v] && peso < distancia_minima[v]) {
        distancia_minima[v] = peso;
        padre[v] = u;
      }
      
      vecino = vecino->siguiente;
    }
  }
  
  // Liberar memoria
  free(visitado);
  free(distancia_minima);
  free(padre);
  
  return resultado;
}

void mst_liberar(ResultadoMST *resultado) {
  if (resultado && resultado->aristas_mst) {
    free(resultado->aristas_mst);
    resultado->aristas_mst = NULL;
    resultado->num_aristas = 0;
  }
}

// ============================================================
// FUNCION DE PRUEBA: Test del algoritmo Kruskal y Prim
// ============================================================

void test_contencion_vacunacion(Grafo *grafo_territorios, int num_territorios) {
  if (!grafo_territorios || num_territorios <= 0) {
    printf("ERROR: Grafo o territorios invalidos\n");
    return;
  }
  
  printf("\n========== SUBPROBLEMA 6: CONTENCION (VACUNACION) ==========\n");
  printf("Territorios: %d\n", num_territorios);
  printf("Algoritmo: MST (Minimum Spanning Tree) con Kruskal y Prim\n\n");
  
  // Prueba 1: Kruskal MST
  printf("--- PRUEBA 1: MST usando Kruskal ---\n");
  ResultadoMST mst_kruskal = kruskal_mst(grafo_territorios, num_territorios);
  
  printf("Aristas en MST: %d\n", mst_kruskal.num_aristas);
  printf("Peso total (distancia): %.2f\n", mst_kruskal.peso_total);
  printf("Territorios conectados en cobertura minima: %d de %d\n\n",
         mst_kruskal.num_aristas + 1, num_territorios);
  
  printf("Conexiones de la red minima (primeras 10):\n");
  for (int i = 0; i < 10 && i < mst_kruskal.num_aristas; i++) {
    printf("  T%d -- T%d (distancia: %.2f)\n",
           mst_kruskal.aristas_mst[i].territorio_1,
           mst_kruskal.aristas_mst[i].territorio_2,
           mst_kruskal.aristas_mst[i].peso);
  }
  if (mst_kruskal.num_aristas > 10) {
    printf("  ... y %d conexiones mas\n", mst_kruskal.num_aristas - 10);
  }
  
  // Prueba 2: Prim MST desde territorio 0
  printf("\n--- PRUEBA 2: MST usando Prim (inicio en Territorio 0) ---\n");
  ResultadoMST mst_prim = prim_mst(grafo_territorios, num_territorios, 0);
  
  printf("Aristas en MST: %d\n", mst_prim.num_aristas);
  printf("Peso total (distancia): %.2f\n", mst_prim.peso_total);
  printf("Territorios conectados: %d de %d\n\n",
         mst_prim.num_aristas + 1, num_territorios);
  
  printf("Conexiones de la red minima (primeras 10):\n");
  for (int i = 0; i < 10 && i < mst_prim.num_aristas; i++) {
    printf("  T%d -- T%d (distancia: %.2f)\n",
           mst_prim.aristas_mst[i].territorio_1,
           mst_prim.aristas_mst[i].territorio_2,
           mst_prim.aristas_mst[i].peso);
  }
  if (mst_prim.num_aristas > 10) {
    printf("  ... y %d conexiones mas\n", mst_prim.num_aristas - 10);
  }
  
  // Comparacion
  printf("\n--- COMPARACION KRUSKAL vs PRIM ---\n");
  printf("Algoritmo | Aristas | Peso Total | Diferencia\n");
  printf("-----------+---------+------------+-----------\n");
  printf("Kruskal   |   %3d   |   %.2f    |    0.00\n",
         mst_kruskal.num_aristas, mst_kruskal.peso_total);
  printf("Prim      |   %3d   |   %.2f    |   %.2f\n",
         mst_prim.num_aristas, mst_prim.peso_total,
         mst_prim.peso_total - mst_kruskal.peso_total);
  
  // Estadisticas
  printf("\n--- ESTADISTICAS DE COBERTURA ---\n");
  float costo_por_territorio = mst_kruskal.peso_total / num_territorios;
  float eficiencia = (float)mst_kruskal.num_aristas / num_territorios;
  
  printf("Costo promedio por territorio: %.2f\n", costo_por_territorio);
  printf("Eficiencia de cobertura: %.2f%% (aristas/territorios)\n",
         eficiencia * 100);
  printf("Territorios con vacunacion: %d (%.1f%%)\n",
         mst_kruskal.num_aristas + 1,
         (float)(mst_kruskal.num_aristas + 1) / num_territorios * 100);
  
  printf("\nComplejidad Kruskal: O(m log m) donde m=%d aristas\n", 
         mst_kruskal.num_aristas * 3);  // Aproximacion
  printf("Complejidad Prim: O(n^2) = O(%d)\n", num_territorios * num_territorios);
  printf("Estructura auxiliar: Union-Find con O(alpha(n)) amortizado\n");
  
  printf("\n===== FIN PRUEBAS SUBPROBLEMA 6 =====\n\n");
  
  // Liberar
  mst_liberar(&mst_kruskal);
  mst_liberar(&mst_prim);
}
