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

// --- Forward Declarations ---
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

#endif // ESTRUCTURAS_H
