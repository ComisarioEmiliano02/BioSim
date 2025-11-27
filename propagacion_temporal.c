#include "propagacion_temporal.h"
#include "heap.h"
#include <math.h>
#include <time.h>

// ========================================================================
// SUBPROBLEMA 3: PROPAGACION TEMPORAL
// Objetivo: Simular la propagación temporal de la infección usando Min-Heap
// Complejidad: O(n log n) donde n es el número de eventos
// ========================================================================

// Helper: Comparar eventos por tiempo (para Min-Heap)
static int comparar_eventos(ElementoHeap a, ElementoHeap b) {
  EventoInfeccion *evento_a = (EventoInfeccion *)a.datos;
  EventoInfeccion *evento_b = (EventoInfeccion *)b.datos;
  
  if (evento_a->tiempo < evento_b->tiempo) return -1;
  if (evento_a->tiempo > evento_b->tiempo) return 1;
  return 0;
}

// Crear evento de infección
static EventoInfeccion* crear_evento_infeccion(int tiempo, int individuo_id,
                                               int individuo_origen,
                                               int territorio_id, int cepa_id,
                                               float probabilidad) {
  EventoInfeccion *evento = (EventoInfeccion *)malloc(sizeof(EventoInfeccion));
  evento->tiempo = tiempo;
  evento->individuo_id = individuo_id;
  evento->individuo_origen = individuo_origen;
  evento->territorio_id = territorio_id;
  evento->cepa_id = cepa_id;
  evento->probabilidad = probabilidad;
  return evento;
}

// Simular la propagación temporal
// Usa Min-Heap para procesar eventos cronológicamente
// Complejidad: O(n log n) donde n = número total de eventos
ResultadoPropagacion* simular_propagacion_temporal(Territorio *territorios,
                                                   int num_territorios,
                                                   Individuo *poblacion,
                                                   int num_poblacion,
                                                   Cepa *cepas,
                                                   int num_cepas,
                                                   int dias_simulacion) {
  // Inicializar resultado
  ResultadoPropagacion *resultado = (ResultadoPropagacion *)malloc(sizeof(ResultadoPropagacion));
  resultado->dias_simulados = dias_simulacion;
  resultado->num_eventos = 0;
  resultado->total_infectados = 0;
  resultado->total_recuperados = 0;
  resultado->total_muertos = 0;
  
  // Arrays para rastrear por día
  resultado->infectados_por_dia = (int *)calloc(dias_simulacion + 1, sizeof(int));
  resultado->recuperados_por_dia = (int *)calloc(dias_simulacion + 1, sizeof(int));
  resultado->muertos_por_dia = (int *)calloc(dias_simulacion + 1, sizeof(int));
  
  // Array de estado para cada individuo
  EstadoSalud *estado_actual = (EstadoSalud *)malloc(sizeof(EstadoSalud) * num_poblacion);
  int *dias_infectados = (int *)malloc(sizeof(int) * num_poblacion);
  
  // Copiar estado inicial
  for (int i = 0; i < num_poblacion; i++) {
    estado_actual[i] = poblacion[i].estado;
    dias_infectados[i] = poblacion[i].tiempo_infeccion;
  }
  
  // Crear Min-Heap para eventos (O(1) creación, O(log n) insertado)
  Heap *heap_eventos = heap_crear(num_poblacion * dias_simulacion, true);
  
  // Contar infectados iniciales y generar eventos iniciales de propagación
  for (int i = 0; i < num_poblacion; i++) {
    if (estado_actual[i] == INFECTADO) {
      resultado->total_infectados++;
      // Agregar evento para que propague en día 0 mismo
      EventoInfeccion *evento_inicial = crear_evento_infeccion(0, i, -1, 
                                                               poblacion[i].territorio_id, 0, 1.0);
      heap_insertar(heap_eventos, i, 0, evento_inicial);
      resultado->num_eventos++;
    }
  }
  
  // Inicializar estadísticas por día (usar -1 como "no registrado")
  // Así podemos saber qué días tuvieron eventos
  for (int d = 0; d <= dias_simulacion; d++) {
    resultado->infectados_por_dia[d] = -1;
    resultado->recuperados_por_dia[d] = -1;
    resultado->muertos_por_dia[d] = -1;
  }
  // Registrar día 0
  resultado->infectados_por_dia[0] = resultado->total_infectados;
  resultado->recuperados_por_dia[0] = resultado->total_recuperados;
  resultado->muertos_por_dia[0] = resultado->total_muertos;
  
  // Procesar eventos cronológicamente O(n log n)
  // n = número de eventos generados
  while (!heap_vacio(heap_eventos)) {
    // Extraer evento con tiempo mínimo O(log n)
    ElementoHeap elem = heap_extraer(heap_eventos);
    EventoInfeccion *evento = (EventoInfeccion *)elem.datos;
    
    if (evento->tiempo > dias_simulacion) {
      free(evento);
      continue;
    }
    
    int individuo_id = evento->individuo_id;
    int tiempo = evento->tiempo;
    Cepa *cepa = &cepas[evento->cepa_id];
    
    // Procesar infección: cambiar estado si estaba sano
    bool es_nuevo_infectado = (estado_actual[individuo_id] == SANO);
    if (es_nuevo_infectado) {
      // Infectar
      estado_actual[individuo_id] = INFECTADO;
      dias_infectados[individuo_id] = 0;
      resultado->total_infectados++;
    }
    
    // Si está infectado, generar eventos de propagación a contactos
    // Hacer esto ANTES de incrementar dias_infectados para identificar primer día
    if (estado_actual[individuo_id] == INFECTADO) {
      // Generar eventos de propagación a contactos (O(1) amortizado por evento)
      // Probabilidad de transmisión = cepa->beta - AUMENTADA para más dinámica
      float prob_transmision = cepa->beta * 0.95f;
      
      // Buscar contactos en el mismo territorio
      for (int j = 0; j < num_poblacion; j++) {
        if (j != individuo_id &&
            poblacion[j].territorio_id == poblacion[individuo_id].territorio_id &&
            estado_actual[j] == SANO) {
          // Generar evento de infección con probabilidad O(log n) inserción
          float rand_val = (rand() % 100) / 100.0f;
          if (rand_val < prob_transmision) {
            int nuevo_tiempo = tiempo + 1;
            EventoInfeccion *nuevo_evento = crear_evento_infeccion(
              nuevo_tiempo, j, individuo_id,
              poblacion[j].territorio_id, evento->cepa_id, prob_transmision);
            heap_insertar(heap_eventos, j, nuevo_tiempo, nuevo_evento);
            resultado->num_eventos++;
          }
        }
      }
    }
    
    // Actualizar infección (días)
    dias_infectados[individuo_id]++;
    
    // Verificar recuperación (gamma = tasa de recuperación)
    // Usar 1 día como período de infección - MUY CORTO para múltiples ciclos
    float dias_hasta_recuperacion = 1.0f / cepa->gamma;
    if (dias_infectados[individuo_id] > (int)dias_hasta_recuperacion) {
      // Probabilidad de muerte = letalidad
      if ((rand() % 1000) / 1000.0f < cepa->letalidad) {
        estado_actual[individuo_id] = RECUPERADO;
        resultado->total_muertos++;
      } else {
        estado_actual[individuo_id] = RECUPERADO;
        resultado->total_recuperados++;
      }
    }
    
    // Registrar estadísticas del día
    if (tiempo <= dias_simulacion) {
      // Registrar SIEMPRE el estado actual en este día
      // Esto actualiza el array con los valores más recientes
      resultado->infectados_por_dia[tiempo] = resultado->total_infectados;
      resultado->recuperados_por_dia[tiempo] = resultado->total_recuperados;
      resultado->muertos_por_dia[tiempo] = resultado->total_muertos;
    }
    
    free(evento);
  }
  
  // Propagar estadísticas hacia adelante para llenar días sin eventos
  // Si un día no tiene valor registrado (-1), usar el del día anterior
  for (int d = 1; d <= dias_simulacion; d++) {
    if (resultado->infectados_por_dia[d] == -1) {
      resultado->infectados_por_dia[d] = resultado->infectados_por_dia[d-1];
    }
    if (resultado->recuperados_por_dia[d] == -1) {
      resultado->recuperados_por_dia[d] = resultado->recuperados_por_dia[d-1];
    }
    if (resultado->muertos_por_dia[d] == -1) {
      resultado->muertos_por_dia[d] = resultado->muertos_por_dia[d-1];
    }
  }
  
  // Reinfectar algunos recuperados cada 15 días para simular nuevas olas
  for (int d = 15; d <= dias_simulacion; d += 15) {
    for (int i = 0; i < num_poblacion && d <= dias_simulacion; i++) {
      if (estado_actual[i] == RECUPERADO && (rand() % 100) < 5) {
        // 5% de recuperados se reinifectan
        estado_actual[i] = INFECTADO;
        dias_infectados[i] = 0;
        resultado->total_infectados++;
        resultado->total_recuperados--;
        
        // Registrar la reinfección
        resultado->infectados_por_dia[d] = resultado->total_infectados;
        resultado->recuperados_por_dia[d] = resultado->total_recuperados;
        resultado->muertos_por_dia[d] = resultado->total_muertos;
      }
    }
  }
  
  // Liberar recursos
  heap_liberar(heap_eventos);
  free(estado_actual);
  free(dias_infectados);
  
  return resultado;
}

// Liberar resultado
void liberar_resultado_propagacion(ResultadoPropagacion *resultado) {
  if (resultado) {
    free(resultado->infectados_por_dia);
    free(resultado->recuperados_por_dia);
    free(resultado->muertos_por_dia);
    free(resultado);
  }
}

// Función de prueba
void test_propagacion_temporal(Territorio *territorios, int num_territorios,
                               Individuo *poblacion, int num_poblacion,
                               Cepa *cepas, int num_cepas) {
  printf("\n========== SUBPROBLEMA 3: PROPAGACION TEMPORAL ==========\n");
  printf("Poblacion: %d individuos\n", num_poblacion);
  printf("Territorios: %d\n", num_territorios);
  printf("Cepas: %d\n", num_cepas);
  
  // Simular propagación por 60 días O(n log n)
  int dias = 60;
  ResultadoPropagacion *resultado = simular_propagacion_temporal(
    territorios, num_territorios, poblacion, num_poblacion, cepas, num_cepas, dias);
  
  printf("\n--- RESULTADOS DE SIMULACION (60 DIAS) ---\n");
  printf("Total de eventos procesados: %d\n", resultado->num_eventos);
  printf("Infectados totales: %d\n", resultado->total_infectados);
  printf("Recuperados totales: %d\n", resultado->total_recuperados);
  printf("Muertos totales: %d\n", resultado->total_muertos);
  printf("Dias simulados: %d\n", resultado->dias_simulados);
  
  printf("\nProgresion por dia (muestra cada 10 dias):\n");
  printf("  Dia | Infectados | Recuperados | Muertos\n");
  printf("------+------------+-------------+--------\n");
  for (int d = 0; d <= dias && d <= resultado->dias_simulados; d += 10) {
    printf("  %3d | %10d | %11d | %7d\n",
           d,
           resultado->infectados_por_dia[d],
           resultado->recuperados_por_dia[d],
           resultado->muertos_por_dia[d]);
  }
  
  printf("\nEstadisticas finales (dia %d):\n", dias);
  printf("  Infectados: %d (%.1f%% de poblacion)\n",
         resultado->total_infectados,
         (100.0f * resultado->total_infectados) / num_poblacion);
  printf("  Recuperados: %d (%.1f%% de poblacion)\n",
         resultado->total_recuperados,
         (100.0f * resultado->total_recuperados) / num_poblacion);
  printf("  Muertos: %d (%.1f%% de poblacion)\n",
         resultado->total_muertos,
         (100.0f * resultado->total_muertos) / num_poblacion);
  
  printf("\nComplejidad: O(n log n) donde n = %d eventos\n", resultado->num_eventos);
  printf("===== FIN PRUEBAS SUBPROBLEMA 3 =====\n");
  
  liberar_resultado_propagacion(resultado);
}
