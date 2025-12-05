#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes ---
#define MAX_NOMBRE 100
#define MAX_ADN 50

// --- Enums ---
typedef enum { SANO, INFECTADO, RECUPERADO } EstadoSalud;

// --- Declaración directa ---
struct NodoIndividuo; // Para la lista de individuos en Territorio

// --- Estructuras Principales ---

// 1. Individuo
typedef struct {
  int id;
  char nombre[MAX_NOMBRE];
  int territorio_id;
  int riesgo; // Valor numérico de riesgo
  EstadoSalud estado;
  int tiempo_infeccion; // Días desde la infección (0 si no está infectado)
} Individuo;

// Nodo para lista enlazada de individuos (usado en Territorio)
typedef struct NodoIndividuo {
  Individuo *data;
  struct NodoIndividuo *siguiente;
} NodoIndividuo;

// 2. Territorio
typedef struct {
  int id;
  char nombre[MAX_NOMBRE];
  NodoIndividuo *individuos; // Lista enlazada de individuos en este territorio
  int num_individuos;
} Territorio;

// 3. Cepa / Variante
typedef struct {
  int id;
  char nombre_adn[MAX_ADN]; // Cadena representativa del ADN
  float beta;               // Tasa de propagación
  float letalidad;          // Tasa de letalidad
  float gamma;              // Tasa de recuperación
} Cepa;

// 4. Conexiones (Grafo)
// Representación: Lista de Adyacencia
// Podemos tener conexiones entre Territorios (para movimiento) y entre
// Individuos (contactos) Por simplicidad inicial, definiremos un grafo genérico
// o específico según se requiera. Para el requisito "Conexiones entre
// territorios", usaremos un grafo de territorios.

typedef struct NodoAdyacencia {
  int destino_id; // ID del Territorio o Individuo conectado
  float peso;     // Puede representar distancia, probabilidad de flujo, etc.
  struct NodoAdyacencia *siguiente;
} NodoAdyacencia;

typedef struct {
  int num_nodos;
  NodoAdyacencia **listas; // Array de punteros a listas de adyacencia
} Grafo;

// ============================================================
// ESTRUCTURAS AUXILIARES PARA LOS 8 SUBPROBLEMAS
// ============================================================

// 5. Hash Table (Subproblema 8: Consultas Rápidas O(1))
#define HASH_TABLE_SIZE 1009  // Número primo para mejor distribución

typedef struct NodoHash {
  int individuo_id;      // Clave
  Individuo *data;       // Valor
  struct NodoHash *siguiente;  // Para manejo de colisiones (encadenamiento)
} NodoHash;

typedef struct {
  NodoHash **tabla;
  int size;
  int elementos;
} TablaHash;

// 6. Trie (Subproblema 7: Clustering de Cepas O(L))
#define ALPHABET_SIZE 4  // A, C, G, T

typedef struct NodoTrie {
  struct NodoTrie *hijos[ALPHABET_SIZE];
  bool es_final;
  int cepa_id;
} NodoTrie;

typedef struct {
  NodoTrie *raiz;
} Trie;

// 7. Heap (Min-Heap y Max-Heap) (Subproblemas 3 y 5)
typedef struct {
  int individuo_id;
  int prioridad;  // Tiempo, riesgo, o probabilidad según el caso
  void *datos;
} ElementoHeap;

typedef struct {
  ElementoHeap *elementos;
  int tamano;
  int capacidad;
  bool es_min_heap;  // true para Min-Heap, false para Max-Heap
} Heap;

// 8. Union-Find (Subproblema 6: Contención/Kruskal O(α(n)))
typedef struct {
  int *padre;
  int *rango;
  int num_elementos;
} UnionFind;

// 9. Cola FIFO para BFS (Subproblema 2: Deteccion de Brotes)
typedef struct NodoFila {
  int individuo_id;
  struct NodoFila *siguiente;
} NodoFila;

typedef struct {
  NodoFila *frente;
  NodoFila *cola;
  int tamano;
} Cola;

// 10. Grafo de Contactos (Subproblema 2: Detección de Brotes)
// Lista de adyacencia para contactos entre individuos
typedef struct NodoContacto {
  int individuo_id;          // ID del individuo en contacto
  float probabilidad_contacto; // Probabilidad de transmisión
  struct NodoContacto *siguiente;
} NodoContacto;

typedef struct {
  int num_individuos;
  NodoContacto **listas;  // Array de listas de adyacencia
} GrafoContactos;

// 11. Evento de Infeccion (Subproblema 3: Propagación Temporal)
typedef struct {
  int tiempo;           // Día del evento
  int individuo_id;     // Individuo afectado
  int individuo_origen; // Quién lo infectó (-1 si es inicial)
  int territorio_id;    // Territorio donde ocurre
  int cepa_id;          // Cepa viral
  float probabilidad;   // Probabilidad de transmisión
} EventoInfeccion;

// Resultado de la simulación de propagación temporal
typedef struct {
  int total_infectados;
  int total_recuperados;
  int total_muertos;
  int dias_simulados;
  int num_eventos;
  int *infectados_por_dia;  // Array con infectados acumulados por día
  int *recuperados_por_dia; // Array con recuperados acumulados por día
  int *muertos_por_dia;     // Array con muertos acumulados por día
} ResultadoPropagacion;

#endif // ESTRUCTURAS_H
