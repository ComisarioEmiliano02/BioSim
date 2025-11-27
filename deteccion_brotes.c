#include "deteccion_brotes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// SUBPROBLEMA 2: Detección de Brotes
// BFS y DFS para encontrar componentes conectadas de infectados
// Complejidad: O(V + E) donde V = infectados, E = contactos
// ============================================================

// ===== ESTRUCTURAS AUXILIARES PARA COLA FIFO (BFS) =====

Cola* cola_crear() {
  Cola *cola = (Cola *)malloc(sizeof(Cola));
  cola->frente = NULL;
  cola->cola = NULL;
  cola->tamano = 0;
  return cola;
}

void cola_encolar(Cola *cola, int individuo_id) {
  if (!cola) return;
  
  NodoFila *nuevo = (NodoFila *)malloc(sizeof(NodoFila));
  nuevo->individuo_id = individuo_id;
  nuevo->siguiente = NULL;
  
  if (cola->tamano == 0) {
    cola->frente = nuevo;
  } else {
    cola->cola->siguiente = nuevo;
  }
  cola->cola = nuevo;
  cola->tamano++;
}

int cola_desencolar(Cola *cola) {
  if (!cola || cola->tamano == 0) return -1;
  
  NodoFila *temp = cola->frente;
  int id = temp->individuo_id;
  cola->frente = temp->siguiente;
  cola->tamano--;
  
  if (cola->tamano == 0) {
    cola->cola = NULL;
  }
  
  free(temp);
  return id;
}

bool cola_vacia(Cola *cola) {
  return !cola || cola->tamano == 0;
}

void cola_liberar(Cola *cola) {
  if (!cola) return;
  
  while (!cola_vacia(cola)) {
    cola_desencolar(cola);
  }
  free(cola);
}

// ===== CONSTRUCCION DE GRAFO DE CONTACTOS =====
// Complejidad: O(T) donde T es número total de individuos en territorios

GrafoContactos* construir_grafo_contactos(Territorio *territorios, int num_territorios, int num_individuos) {
  GrafoContactos *grafo = (GrafoContactos *)malloc(sizeof(GrafoContactos));
  grafo->num_individuos = num_individuos;
  grafo->listas = (NodoContacto **)calloc(num_individuos, sizeof(NodoContacto *));
  
  // Crear contactos entre individuos del mismo territorio
  // Complejidad: O(T) donde T es número total de individuos
  for (int t = 0; t < num_territorios; t++) {
    // Para cada territorio, conectar individuos (simulando contacto territorial)
    NodoIndividuo *nodo1 = territorios[t].individuos;
    
    while (nodo1 != NULL) {
      int id1 = nodo1->data->id;
      
      // Conectar con otros individuos del mismo territorio (crear contactos)
      NodoIndividuo *nodo2 = nodo1->siguiente;
      while (nodo2 != NULL) {
        int id2 = nodo2->data->id;
        
        // Agregar arista de id1 a id2
        NodoContacto *nuevo = (NodoContacto *)malloc(sizeof(NodoContacto));
        nuevo->individuo_id = id2;
        nuevo->probabilidad_contacto = 0.5f + (rand() % 50) / 100.0f; // 0.5 a 0.99
        nuevo->siguiente = grafo->listas[id1];
        grafo->listas[id1] = nuevo;
        
        // Agregar arista de id2 a id1 (grafo no dirigido)
        nuevo = (NodoContacto *)malloc(sizeof(NodoContacto));
        nuevo->individuo_id = id1;
        nuevo->probabilidad_contacto = 0.5f + (rand() % 50) / 100.0f;
        nuevo->siguiente = grafo->listas[id2];
        grafo->listas[id2] = nuevo;
        
        nodo2 = nodo2->siguiente;
      }
      
      nodo1 = nodo1->siguiente;
    }
  }
  
  return grafo;
}

void grafo_contactos_liberar(GrafoContactos *grafo) {
  if (!grafo) return;
  
  for (int i = 0; i < grafo->num_individuos; i++) {
    NodoContacto *actual = grafo->listas[i];
    while (actual != NULL) {
      NodoContacto *temp = actual;
      actual = actual->siguiente;
      free(temp);
    }
  }
  
  free(grafo->listas);
  free(grafo);
}

// ===== IMPLEMENTACION BFS =====
// Complejidad: O(V + E) donde V = infectados, E = contactos entre infectados
// BFS es iterativo usando Cola FIFO

ResultadoDeteccionBrotes detectar_brotes_bfs(Territorio *territorios, int num_territorios, Individuo *poblacion, int num_individuos) {
  ResultadoDeteccionBrotes resultado;
  resultado.componentes = (int *)malloc(num_individuos * sizeof(int));
  resultado.num_componentes = 0;
  memset(resultado.componentes, -1, num_individuos * sizeof(int));
  memset(resultado.num_infectados_por_componente, 0, sizeof(resultado.num_infectados_por_componente));
  
  // Construir grafo de contactos
  GrafoContactos *grafo = construir_grafo_contactos(territorios, num_territorios, num_individuos);
  
  // Crear cola para BFS
  Cola *cola = cola_crear();
  
  // Iterar sobre todos los individuos
  for (int i = 0; i < num_individuos; i++) {
    // Solo procesar infectados no visitados
    if (poblacion[i].estado == INFECTADO && resultado.componentes[i] == -1) {
      // Nuevo componente
      int id_componente = resultado.num_componentes;
      
      // Encolar el individuo inicial
      cola_encolar(cola, i);
      resultado.componentes[i] = id_componente;
      resultado.num_infectados_por_componente[id_componente]++;
      
      // BFS: procesar cola
      while (!cola_vacia(cola)) {
        int id_actual = cola_desencolar(cola);
        
        // Explorar vecinos (contactos)
        NodoContacto *contacto = grafo->listas[id_actual];
        while (contacto != NULL) {
          int id_vecino = contacto->individuo_id;
          
          // Si el vecino está infectado y no ha sido visitado
          if (poblacion[id_vecino].estado == INFECTADO && resultado.componentes[id_vecino] == -1) {
            resultado.componentes[id_vecino] = id_componente;
            resultado.num_infectados_por_componente[id_componente]++;
            cola_encolar(cola, id_vecino);
          }
          
          contacto = contacto->siguiente;
        }
      }
      
      resultado.num_componentes++;
    }
  }
  
  // Liberar recursos
  cola_liberar(cola);
  grafo_contactos_liberar(grafo);
  
  return resultado;
}

// ===== IMPLEMENTACION DFS =====
// Complejidad: O(V + E)
// DFS es recursivo (stack implícito)

void dfs_recurivo(int id_individuo, int id_componente, Individuo *poblacion,
                  GrafoContactos *grafo, ResultadoDeteccionBrotes *resultado) {
  // Marcar como visitado
  resultado->componentes[id_individuo] = id_componente;
  resultado->num_infectados_por_componente[id_componente]++;
  
  // Visitar vecinos infectados no visitados
  NodoContacto *contacto = grafo->listas[id_individuo];
  while (contacto != NULL) {
    int id_vecino = contacto->individuo_id;
    
    if (poblacion[id_vecino].estado == INFECTADO && resultado->componentes[id_vecino] == -1) {
      dfs_recurivo(id_vecino, id_componente, poblacion, grafo, resultado);
    }
    
    contacto = contacto->siguiente;
  }
}

ResultadoDeteccionBrotes detectar_brotes_dfs(Territorio *territorios, int num_territorios, Individuo *poblacion, int num_individuos) {
  ResultadoDeteccionBrotes resultado;
  resultado.componentes = (int *)malloc(num_individuos * sizeof(int));
  resultado.num_componentes = 0;
  memset(resultado.componentes, -1, num_individuos * sizeof(int));
  memset(resultado.num_infectados_por_componente, 0, sizeof(resultado.num_infectados_por_componente));
  
  // Construir grafo de contactos
  GrafoContactos *grafo = construir_grafo_contactos(territorios, num_territorios, num_individuos);
  
  // Iterar sobre todos los individuos
  for (int i = 0; i < num_individuos; i++) {
    // Solo procesar infectados no visitados
    if (poblacion[i].estado == INFECTADO && resultado.componentes[i] == -1) {
      // Nuevo componente
      int id_componente = resultado.num_componentes;
      
      // Iniciar DFS recursivo
      dfs_recurivo(i, id_componente, poblacion, grafo, &resultado);
      
      resultado.num_componentes++;
    }
  }
  
  // Liberar recursos
  grafo_contactos_liberar(grafo);
  
  return resultado;
}

void deteccion_brotes_liberar(ResultadoDeteccionBrotes *resultado) {
  if (resultado && resultado->componentes) {
    free(resultado->componentes);
  }
}

// ===== FUNCION DE PRUEBA =====

void test_deteccion_brotes(Territorio *territorios, int num_territorios,
                           Individuo *poblacion, int num_individuos) {
  printf("\n========== SUBPROBLEMA 2: DETECCION DE BROTES ==========\n");
  printf("Poblacion: %d individuos\n", num_individuos);
  
  // Contar infectados
  int num_infectados = 0;
  for (int i = 0; i < num_individuos; i++) {
    if (poblacion[i].estado == INFECTADO) {
      num_infectados++;
    }
  }
  printf("Infectados: %d\n", num_infectados);
  printf("Territorios: %d\n\n", num_territorios);
  
  if (num_infectados == 0) {
    printf("No hay infectados para detectar brotes\n");
    printf("===== FIN PRUEBAS SUBPROBLEMA 2 =====\n\n");
    return;
  }
  
  // Prueba 1: BFS
  printf("--- PRUEBA 1: Deteccion de Brotes con BFS ---\n");
  ResultadoDeteccionBrotes resultado_bfs = detectar_brotes_bfs(territorios, num_territorios, poblacion, num_individuos);
  
  printf("Componentes encontradas: %d\n", resultado_bfs.num_componentes);
  for (int i = 0; i < resultado_bfs.num_componentes; i++) {
    printf("  Componente %d: %d infectados\n", i, resultado_bfs.num_infectados_por_componente[i]);
  }
  
  printf("\nMuestra de infectados por componente:\n");
  for (int comp = 0; comp < resultado_bfs.num_componentes && comp < 3; comp++) {
    printf("  Componente %d:\n", comp);
    int contador = 0;
    for (int i = 0; i < num_individuos && contador < 3; i++) {
      if (resultado_bfs.componentes[i] == comp) {
        printf("    %s (territorio %d)\n", poblacion[i].nombre, poblacion[i].territorio_id);
        contador++;
      }
    }
  }
  
  // Prueba 2: DFS
  printf("\n--- PRUEBA 2: Deteccion de Brotes con DFS ---\n");
  ResultadoDeteccionBrotes resultado_dfs = detectar_brotes_dfs(territorios, num_territorios, poblacion, num_individuos);
  
  printf("Componentes encontradas: %d\n", resultado_dfs.num_componentes);
  for (int i = 0; i < resultado_dfs.num_componentes; i++) {
    printf("  Componente %d: %d infectados\n", i, resultado_dfs.num_infectados_por_componente[i]);
  }
  
  printf("\nMuestra de infectados por componente:\n");
  for (int comp = 0; comp < resultado_dfs.num_componentes && comp < 3; comp++) {
    printf("  Componente %d:\n", comp);
    int contador = 0;
    for (int i = 0; i < num_individuos && contador < 3; i++) {
      if (resultado_dfs.componentes[i] == comp) {
        printf("    %s (territorio %d)\n", poblacion[i].nombre, poblacion[i].territorio_id);
        contador++;
      }
    }
  }
  
  // Comparacion
  printf("\n--- COMPARACION BFS vs DFS ---\n");
  printf("BFS componentes: %d\n", resultado_bfs.num_componentes);
  printf("DFS componentes: %d\n", resultado_dfs.num_componentes);
  
  if (resultado_bfs.num_componentes == resultado_dfs.num_componentes) {
    printf("✓ Ambos algoritmos encontraron el mismo numero de componentes\n");
  } else {
    printf("✗ Inconsistencia entre BFS y DFS\n");
  }
  
  printf("\nComplejidad BFS: O(V + E) = O(%d + E)\n", num_infectados);
  printf("Complejidad DFS: O(V + E) = O(%d + E)\n", num_infectados);
  
  printf("\n===== FIN PRUEBAS SUBPROBLEMA 2 =====\n\n");
  
  // Liberar
  deteccion_brotes_liberar(&resultado_bfs);
  deteccion_brotes_liberar(&resultado_dfs);
}
