#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes de Generación ---
#define NUM_CEPAS 50
#define NUM_TERRITORIOS 20
#define MIN_CONEXIONES_TERRITORIOS 30
#define NUM_INDIVIDUOS_TOTAL 1000 // Ejemplo inicial
#define NUM_PACIENTES_CERO 10

// --- Variables Globales (Simulando base de datos en memoria) ---
Cepa cepas[NUM_CEPAS];
Territorio territorios[NUM_TERRITORIOS];
Individuo *poblacion; // Array dinámico
Grafo grafo_territorios;

// --- Funciones Auxiliares ---
float random_float(float min, float max) {
  return min + ((float)rand() / RAND_MAX) * (max - min);
}

void generar_cadena_adn(char *buffer, int length) {
  const char bases[] = "ACGT";
  for (int i = 0; i < length - 1; i++) {
    buffer[i] = bases[rand() % 4];
  }
  buffer[length - 1] = '\0';
}

// --- Inicialización de Estructuras ---

void inicializar_cepas() {
  for (int i = 0; i < NUM_CEPAS; i++) {
    cepas[i].id = i;
    generar_cadena_adn(cepas[i].nombre_adn, 20); // ADN de longitud 20
    cepas[i].beta = random_float(0.1, 0.9);
    cepas[i].letalidad = random_float(0.01, 0.3);
    cepas[i].gamma = random_float(0.05, 0.5);
  }
  printf("Generadas %d cepas.\n", NUM_CEPAS);
}

void inicializar_territorios() {
  for (int i = 0; i < NUM_TERRITORIOS; i++) {
    territorios[i].id = i;
    sprintf(territorios[i].nombre, "Territorio_%d", i);
    territorios[i].individuos = NULL;
    territorios[i].num_individuos = 0;
  }
  printf("Generados %d territorios.\n", NUM_TERRITORIOS);
}

void inicializar_grafo_territorios() {
  grafo_territorios.num_nodos = NUM_TERRITORIOS;
  grafo_territorios.listas =
      (NodoAdyacencia **)malloc(NUM_TERRITORIOS * sizeof(NodoAdyacencia *));

  for (int i = 0; i < NUM_TERRITORIOS; i++) {
    grafo_territorios.listas[i] = NULL;
  }

  int conexiones_creadas = 0;
  // Asegurar mínimo 30 conexiones
  while (conexiones_creadas < MIN_CONEXIONES_TERRITORIOS) {
    int origen = rand() % NUM_TERRITORIOS;
    int destino = rand() % NUM_TERRITORIOS;

    if (origen != destino) {
      // Verificar si ya existe conexión (simple check)
      bool existe = false;
      NodoAdyacencia *actual = grafo_territorios.listas[origen];
      while (actual != NULL) {
        if (actual->destino_id == destino) {
          existe = true;
          break;
        }
        actual = actual->siguiente;
      }

      if (!existe) {
        NodoAdyacencia *nuevo =
            (NodoAdyacencia *)malloc(sizeof(NodoAdyacencia));
        nuevo->destino_id = destino;
        nuevo->peso = random_float(1.0, 10.0); // Distancia o costo
        nuevo->siguiente = grafo_territorios.listas[origen];
        grafo_territorios.listas[origen] = nuevo;
        conexiones_creadas++;
      }
    }
  }
  printf("Generadas %d conexiones entre territorios.\n", conexiones_creadas);
}

void agregar_individuo_a_territorio(Territorio *t, Individuo *ind) {
  NodoIndividuo *nuevo = (NodoIndividuo *)malloc(sizeof(NodoIndividuo));
  nuevo->data = ind;
  nuevo->siguiente = t->individuos;
  t->individuos = nuevo;
  t->num_individuos++;
}

void inicializar_poblacion() {
  poblacion = (Individuo *)malloc(NUM_INDIVIDUOS_TOTAL * sizeof(Individuo));

  for (int i = 0; i < NUM_INDIVIDUOS_TOTAL; i++) {
    poblacion[i].id = i;
    sprintf(poblacion[i].nombre, "Individuo_%d", i);
    poblacion[i].territorio_id = rand() % NUM_TERRITORIOS;
    poblacion[i].riesgo = rand() % 100; // 0-99
    poblacion[i].estado = SANO;
    poblacion[i].tiempo_infeccion = 0;

    // Agregar a la lista del territorio correspondiente
    agregar_individuo_a_territorio(&territorios[poblacion[i].territorio_id],
                                   &poblacion[i]);
  }

  // Infectar Pacientes Cero
  for (int i = 0; i < NUM_PACIENTES_CERO; i++) {
    int idx = rand() % NUM_INDIVIDUOS_TOTAL;
    // Asegurar que no repetimos (simple check omitido por brevedad,
    // probabilidad baja)
    poblacion[idx].estado = INFECTADO;
    poblacion[idx].tiempo_infeccion = 1;
  }
  printf("Generada poblacion de %d individuos con %d infectados iniciales.\n",
         NUM_INDIVIDUOS_TOTAL, NUM_PACIENTES_CERO);
}

void liberar_memoria() {
  // Liberar grafo
  for (int i = 0; i < NUM_TERRITORIOS; i++) {
    NodoAdyacencia *actual = grafo_territorios.listas[i];
    while (actual != NULL) {
      NodoAdyacencia *temp = actual;
      actual = actual->siguiente;
      free(temp);
    }
  }
  free(grafo_territorios.listas);

  // Liberar listas de territorios
  for (int i = 0; i < NUM_TERRITORIOS; i++) {
    NodoIndividuo *actual = territorios[i].individuos;
    while (actual != NULL) {
      NodoIndividuo *temp = actual;
      actual = actual->siguiente;
      free(temp);
    }
  }

  free(poblacion);
  printf("Memoria liberada correctamente.\n");
}

// --- Main para pruebas ---
int main() {
  srand(time(NULL));

  printf("=== Inicializando BioSim ===\n");
  inicializar_cepas();
  inicializar_territorios();
  inicializar_grafo_territorios();
  inicializar_poblacion();

  printf("=== Inicializacion Completa ===\n");

  // Limpieza
  liberar_memoria();
  return 0;
}
