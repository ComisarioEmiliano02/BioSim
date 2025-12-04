#include "minimizacion_riesgo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================
// SUBPROBLEMA 4: Minimización de Riesgo (Aislamiento)
// Enfoque Greedy basado en ordenamiento
// Complejidad: O(n log n) por ordenamiento + O(n) por selección
// ============================================================

// Estructura auxiliar para rastrear riesgo con índice de individuo
typedef struct {
  int individuo_id;
  float riesgo_valor;
} IndividuoConRiesgo;

// Función comparadora para ordenar por riesgo descendente
static int comparar_riesgo_descendente(const void *a, const void *b) {
  const IndividuoConRiesgo *ind_a = (const IndividuoConRiesgo *)a;
  const IndividuoConRiesgo *ind_b = (const IndividuoConRiesgo *)b;
  
  // Orden descendente: si a.riesgo > b.riesgo, a va primero
  if (ind_a->riesgo_valor > ind_b->riesgo_valor) return -1;
  if (ind_a->riesgo_valor < ind_b->riesgo_valor) return 1;
  return 0;
}

// ============================================================
// Calcula el riesgo de propagación para cada individuo
// Complejidad: O(n)
// 
// Fórmula de riesgo:
//   - Si SANO: riesgo = 0 (no puede propagar)
//   - Si INFECTADO: riesgo = riesgo_personal * (1 + tiempo_infeccion / 100)
//     * Cuanto más infectado, mayor riesgo
//     * Cuanto más tiempo, más contactos posibles
//   - Si RECUPERADO: riesgo = 0 (ya no propaga)
// ============================================================
float* calcular_riesgo_propagacion(Individuo *poblacion, int num_individuos) {
  float *riesgo = (float *)malloc(num_individuos * sizeof(float));
  
  if (!riesgo || !poblacion) {
    return riesgo;
  }
  
  for (int i = 0; i < num_individuos; i++) {
    // Solo individuos infectados pueden propagar
    if (poblacion[i].estado == INFECTADO) {
      // Riesgo base multiplicado por factor de tiempo
      // Cuanto más días infectado, mayor riesgo de haber contagiado
      float factor_tiempo = 1.0f + (poblacion[i].tiempo_infeccion / 100.0f);
      riesgo[i] = poblacion[i].riesgo * factor_tiempo;
    } else {
      // Sanos o recuperados no propagan
      riesgo[i] = 0.0f;
    }
  }
  
  return riesgo;
}

// ============================================================
// Algoritmo Greedy para minimizar riesgo
// Complejidad: O(n log n)
//
// Estrategia:
//   1. Calcular riesgo para cada individuo O(n)
//   2. Crear array de (id, riesgo) O(n)
//   3. Ordenar por riesgo descendente O(n log n)
//   4. Seleccionar top porcentaje O(n)
//   5. Retornar IDs de individuos a aislar
// ============================================================
ResultadoAislamiento minimizar_riesgo_greedy(
  Individuo *poblacion,
  int num_individuos,
  float porcentaje_aislamiento
) {
  ResultadoAislamiento resultado;
  resultado.cantidad = 0;
  resultado.individuos_a_aislar = NULL;
  resultado.riesgo_reducido = 0.0;
  
  if (!poblacion || num_individuos <= 0 || porcentaje_aislamiento < 0 || porcentaje_aislamiento > 100) {
    return resultado;
  }
  
  // Paso 1: Calcular riesgos - O(n)
  float *riesgos = calcular_riesgo_propagacion(poblacion, num_individuos);
  if (!riesgos) {
    return resultado;
  }
  
  // Paso 2: Crear array con (id, riesgo) - O(n)
  IndividuoConRiesgo *individuos_riesgo = 
    (IndividuoConRiesgo *)malloc(num_individuos * sizeof(IndividuoConRiesgo));
  
  if (!individuos_riesgo) {
    free(riesgos);
    return resultado;
  }
  
  for (int i = 0; i < num_individuos; i++) {
    individuos_riesgo[i].individuo_id = poblacion[i].id;
    individuos_riesgo[i].riesgo_valor = riesgos[i];
  }
  
  // Paso 3: Ordenar por riesgo descendente - O(n log n)
  qsort(individuos_riesgo, num_individuos, sizeof(IndividuoConRiesgo), 
        comparar_riesgo_descendente);
  
  // Paso 4: Calcular cuántos individuos aislar
  resultado.cantidad = (int)(num_individuos * porcentaje_aislamiento / 100.0f);
  if (resultado.cantidad == 0 && porcentaje_aislamiento > 0) {
    resultado.cantidad = 1; // Al menos aislar 1 si hay porcentaje > 0
  }
  
  // Paso 5: Asignar memoria y copiar IDs - O(k) donde k = cantidad
  if (resultado.cantidad > 0) {
    resultado.individuos_a_aislar = 
      (int *)malloc(resultado.cantidad * sizeof(int));
    
    if (!resultado.individuos_a_aislar) {
      resultado.cantidad = 0;
      free(riesgos);
      free(individuos_riesgo);
      return resultado;
    }
    
    // Copiar los IDs de los individuos con mayor riesgo
    float riesgo_total_aislado = 0.0f;
    for (int i = 0; i < resultado.cantidad; i++) {
      resultado.individuos_a_aislar[i] = individuos_riesgo[i].individuo_id;
      riesgo_total_aislado += individuos_riesgo[i].riesgo_valor;
    }
    
    // Calcular riesgo reducido
    float riesgo_total = 0.0f;
    for (int i = 0; i < num_individuos; i++) {
      riesgo_total += riesgos[i];
    }
    
    // El riesgo reducido es el riesgo que evitamos al aislar
    resultado.riesgo_reducido = riesgo_total_aislado;
  }
  
  // Liberar memoria temporal
  free(riesgos);
  free(individuos_riesgo);
  
  return resultado;
}

void aislamiento_liberar(ResultadoAislamiento *resultado) {
  if (resultado && resultado->individuos_a_aislar) {
    free(resultado->individuos_a_aislar);
    resultado->individuos_a_aislar = NULL;
    resultado->cantidad = 0;
  }
}

// ============================================================
// FUNCION DE PRUEBA: Test del algoritmo Greedy
// ============================================================

void test_minimizacion_riesgo(Individuo *poblacion, int num_individuos) {
  if (!poblacion || num_individuos <= 0) {
    printf("ERROR: Poblacion invalida para test\n");
    return;
  }
  
  printf("\n========== SUBPROBLEMA 4: MINIMIZACION DE RIESGO ==========\n");
  printf("Poblacion: %d individuos\n\n", num_individuos);
  
  // Calcular riesgos y estadísticas
  float *riesgos = calcular_riesgo_propagacion(poblacion, num_individuos);
  int num_infectados = 0;
  float riesgo_total_poblacion = 0.0f;
  
  for (int i = 0; i < num_individuos; i++) {
    if (poblacion[i].estado == INFECTADO) {
      num_infectados++;
    }
    riesgo_total_poblacion += riesgos[i];
  }
  
  printf("Infectados: %d\n", num_infectados);
  printf("Riesgo total de poblacion: %.2f\n\n", riesgo_total_poblacion);
  
  // Prueba 1: Aislar el 5% de la población
  printf("--- PRUEBA 1: Aislar 5%% de la poblacion ---\n");
  ResultadoAislamiento resultado_5 = minimizar_riesgo_greedy(poblacion, num_individuos, 5.0f);
  
  printf("Individuos a aislar: %d\n", resultado_5.cantidad);
  printf("Riesgo reducido: %.2f\n", resultado_5.riesgo_reducido);
  
  if (resultado_5.cantidad > 0) {
    printf("IDs a aislar (primeros 10): ");
    for (int i = 0; i < 10 && i < resultado_5.cantidad; i++) {
      printf("%d ", resultado_5.individuos_a_aislar[i]);
    }
    if (resultado_5.cantidad > 10) printf("...");
    printf("\n");
  }
  
  // Prueba 2: Aislar el 10% de la población
  printf("--- PRUEBA 2: Aislar 10%% de la poblacion ---\n");
  ResultadoAislamiento resultado_10 = minimizar_riesgo_greedy(poblacion, num_individuos, 10.0f);
  
  printf("Individuos a aislar: %d\n", resultado_10.cantidad);
  printf("Riesgo reducido: %.2f\n", resultado_10.riesgo_reducido);
  
  if (resultado_10.cantidad > 0) {
    printf("IDs a aislar (primeros 10): ");
    for (int i = 0; i < 10 && i < resultado_10.cantidad; i++) {
      printf("%d ", resultado_10.individuos_a_aislar[i]);
    }
    if (resultado_10.cantidad > 10) printf("...");
    printf("\n");
  }
  
  // Prueba 3: Aislar el 20% de la población
  printf("--- PRUEBA 3: Aislar 20%% de la poblacion ---\n");
  ResultadoAislamiento resultado_20 = minimizar_riesgo_greedy(poblacion, num_individuos, 20.0f);
  
  printf("Individuos a aislar: %d\n", resultado_20.cantidad);
  printf("Riesgo reducido: %.2f\n", resultado_20.riesgo_reducido);
  
  if (resultado_20.cantidad > 0) {
    printf("IDs a aislar (primeros 10): ");
    for (int i = 0; i < 10 && i < resultado_20.cantidad; i++) {
      printf("%d ", resultado_20.individuos_a_aislar[i]);
    }
    if (resultado_20.cantidad > 10) printf("...");
    printf("\n");
  }
  
  // Estadísticas
  printf("\n--- COMPARACION DE ESTRATEGIAS ---\n");
  printf("Aislamiento | Cantidad | Riesgo Reducido | Porcentaje de Riesgo\n");
  printf("%-11s | %8d | %15.2f | %18.2f%%\n", 
         "5%", resultado_5.cantidad, resultado_5.riesgo_reducido, 
         (riesgo_total_poblacion > 0) ? (resultado_5.riesgo_reducido / riesgo_total_poblacion * 100) : 0);
  printf("%-11s | %8d | %15.2f | %18.2f%%\n", 
         "10%", resultado_10.cantidad, resultado_10.riesgo_reducido,
         (riesgo_total_poblacion > 0) ? (resultado_10.riesgo_reducido / riesgo_total_poblacion * 100) : 0);
  printf("%-11s | %8d | %15.2f | %18.2f%%\n", 
         "20%", resultado_20.cantidad, resultado_20.riesgo_reducido,
         (riesgo_total_poblacion > 0) ? (resultado_20.riesgo_reducido / riesgo_total_poblacion * 100) : 0);
  
  printf("\nComplejidad: O(n log n) por ordenamiento\n");
  printf("Estrategia: Greedy - Selecciona individuos con maximo riesgo\n");
  
  printf("\n===== FIN PRUEBAS SUBPROBLEMA 4 =====\n\n");
  
  // Liberar resultados
  aislamiento_liberar(&resultado_5);
  aislamiento_liberar(&resultado_10);
  aislamiento_liberar(&resultado_20);
  free(riesgos);
}
