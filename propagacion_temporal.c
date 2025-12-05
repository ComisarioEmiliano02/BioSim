#include "propagacion_temporal.h"
#include "heap.h"
#include <math.h>
#include <time.h>

// ========================================================================
// SUBPROBLEMA 3: PROPAGACION TEMPORAL
// Objetivo: Simular la propagación temporal usando Min-Heap
// Complejidad: O(n log n) donde n = número total de eventos
// ========================================================================

// Comparador de eventos por tiempo (para Min-Heap)
static int comparar_eventos(ElementoHeap a, ElementoHeap b) {
  EventoInfeccion *evento_a = (EventoInfeccion *)a.datos;
  EventoInfeccion *evento_b = (EventoInfeccion *)b.datos;
  
  if (evento_a->tiempo < evento_b->tiempo) return -1;
  if (evento_a->tiempo > evento_b->tiempo) return 1;
  return 0;
}

// Crear evento de infección
static EventoInfeccion* crear_evento(int tiempo, int individuo_id, int tipo) {
  // tipo: 0=infección, 1=recuperación
  EventoInfeccion *evento = (EventoInfeccion *)malloc(sizeof(EventoInfeccion));
  evento->tiempo = tiempo;
  evento->individuo_id = individuo_id;
  evento->individuo_origen = tipo; // Reutilizamos este campo para guardar tipo
  evento->territorio_id = 0;
  evento->cepa_id = 0;
  evento->probabilidad = 0.0f;
  return evento;
}

// Simular propagación con Min-Heap
ResultadoPropagacion* simular_propagacion_temporal(Territorio *territorios,
                                                   int num_territorios,
                                                   Individuo *poblacion,
                                                   int num_poblacion,
                                                   Cepa *cepas,
                                                   int num_cepas,
                                                   int dias_simulacion) {
  ResultadoPropagacion *resultado = (ResultadoPropagacion *)malloc(sizeof(ResultadoPropagacion));
  resultado->dias_simulados = dias_simulacion;
  resultado->num_eventos = 0;
  resultado->total_infectados = 0;
  resultado->total_recuperados = 0;
  resultado->total_muertos = 0;
  
  // Arrays de estadísticas por día
  resultado->infectados_por_dia = (int *)calloc(dias_simulacion + 1, sizeof(int));
  resultado->recuperados_por_dia = (int *)calloc(dias_simulacion + 1, sizeof(int));
  resultado->muertos_por_dia = (int *)calloc(dias_simulacion + 1, sizeof(int));
  
  // Estado de cada individuo
  EstadoSalud *estado = (EstadoSalud *)malloc(sizeof(EstadoSalud) * num_poblacion);
  int *dia_infeccion = (int *)malloc(sizeof(int) * num_poblacion);
  bool *procesado = (bool *)malloc(sizeof(bool) * num_poblacion);
  
  // Inicializar
  for (int i = 0; i < num_poblacion; i++) {
    estado[i] = poblacion[i].estado;
    dia_infeccion[i] = -1;
    procesado[i] = false;
  }
  
  // Crear Min-Heap para eventos
  Heap *heap = heap_crear(num_poblacion * 2, true);
  
  // Contar infectados iniciales y generar eventos
  for (int i = 0; i < num_poblacion; i++) {
    if (estado[i] == INFECTADO) {
      resultado->total_infectados++;
      dia_infeccion[i] = 0;
      procesado[i] = true;
      
      // Generar evento de recuperación (día 12-19)
      int dia_recuperacion = 12 + (rand() % 8);
      EventoInfeccion *evento_recup = crear_evento(dia_recuperacion, i, 1);
      heap_insertar(heap, i, dia_recuperacion, evento_recup);
      resultado->num_eventos++;
      
      // IMPORTANTE: Generar contagios iniciales desde cada infectado inicial
      // Cada uno contagia 5-8 personas en los primeros días (R0 alto)
      int max_contagios = 5 + (rand() % 4);
      int contagios = 0;
      
      for (int j = 0; j < num_poblacion && contagios < max_contagios; j++) {
        if (estado[j] == SANO && !procesado[j] && (rand() % 100) < 60) {
          int dia_contagio = 1 + (rand() % 3); // Días 1-3
          EventoInfeccion *evento_inf = crear_evento(dia_contagio, j, 0);
          heap_insertar(heap, j, dia_contagio, evento_inf);
          contagios++;
        }
      }
    }
  }
  
  // Registrar día 0
  resultado->infectados_por_dia[0] = resultado->total_infectados;
  resultado->recuperados_por_dia[0] = 0;
  resultado->muertos_por_dia[0] = 0;
  
  // Procesar eventos con Min-Heap O(n log n)
  int ultimo_dia = 0;
  while (!heap_vacio(heap)) {
    ElementoHeap elem = heap_extraer(heap);
    EventoInfeccion *evento = (EventoInfeccion *)elem.datos;
    
    int tiempo = evento->tiempo;
    int ind_id = evento->individuo_id;
    int tipo_evento = evento->individuo_origen; // 0=infección, 1=recuperación
    
    if (tiempo > dias_simulacion) {
      free(evento);
      continue;
    }
    
    // Actualizar estadísticas diarias si cambió el día
    if (tiempo > ultimo_dia) {
      for (int d = ultimo_dia + 1; d <= tiempo && d <= dias_simulacion; d++) {
        resultado->infectados_por_dia[d] = resultado->total_infectados;
        resultado->recuperados_por_dia[d] = resultado->total_recuperados;
        resultado->muertos_por_dia[d] = resultado->total_muertos;
      }
      ultimo_dia = tiempo;
    }
    
    // EVENTO DE INFECCIÓN (tipo 0)
    if (tipo_evento == 0 && estado[ind_id] == SANO && !procesado[ind_id]) {
      estado[ind_id] = INFECTADO;
      dia_infeccion[ind_id] = tiempo;
      procesado[ind_id] = true;
      resultado->total_infectados++;
      resultado->num_eventos++;
      
      // Generar evento de recuperación para este nuevo infectado
      int dias_duracion = 12 + (rand() % 8); // Entre 12 y 19 días
      int dia_recup = tiempo + dias_duracion;
      
      if (dia_recup <= dias_simulacion + 10) { // Permitir recuperación después del día 30
        EventoInfeccion *evento_recup = crear_evento(dia_recup, ind_id, 1);
        heap_insertar(heap, ind_id, dia_recup, evento_recup);
      }
      
      // Generar contagios a otros individuos SANOS (propagación controlada)
      // Cada infectado contagia 4-7 personas en promedio (R0 alto para más propagación)
      int max_contagios = 4 + (rand() % 4); // Entre 4 y 7 contagios por infectado
      int contagios_generados = 0;
      
      for (int j = 0; j < num_poblacion && contagios_generados < max_contagios; j++) {
        if (estado[j] == SANO && !procesado[j]) {
          // 50% de probabilidad de contagiar a cada sano encontrado
          if ((rand() % 100) < 50) {
            // Contagio ocurre 1-3 días después
            int delay = 1 + (rand() % 3);
            int tiempo_contagio = tiempo + delay;
            
            if (tiempo_contagio <= dias_simulacion) {
              EventoInfeccion *evento_contagio = crear_evento(tiempo_contagio, j, 0);
              heap_insertar(heap, j, tiempo_contagio, evento_contagio);
              contagios_generados++;
            }
          }
        }
      }
    }
    
    // EVENTO DE RECUPERACIÓN (tipo 1)
    else if (tipo_evento == 1 && estado[ind_id] == INFECTADO) {
      estado[ind_id] = RECUPERADO;
      resultado->total_infectados--;
      
      // 1% de mortalidad
      if ((rand() % 100) < 1) {
        resultado->total_muertos++;
      } else {
        resultado->total_recuperados++;
      }
    }
    
    free(evento);
  }
  
  // Actualizar últimos días
  for (int d = ultimo_dia + 1; d <= dias_simulacion; d++) {
    resultado->infectados_por_dia[d] = resultado->total_infectados;
    resultado->recuperados_por_dia[d] = resultado->total_recuperados;
    resultado->muertos_por_dia[d] = resultado->total_muertos;
  }
  
  // Liberar recursos
  heap_liberar(heap);
  free(estado);
  free(dia_infeccion);
  free(procesado);
  
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
  
  int dias = 30;
  ResultadoPropagacion *resultado = simular_propagacion_temporal(
    territorios, num_territorios, poblacion, num_poblacion, cepas, num_cepas, dias);
  
  printf("\n--- RESULTADOS DE SIMULACION (30 DIAS) ---\n");
  printf("Total de eventos procesados: %d\n", resultado->num_eventos);
  printf("Infectados totales: %d\n", resultado->total_infectados);
  printf("Recuperados totales: %d\n", resultado->total_recuperados);
  printf("Muertos totales: %d\n", resultado->total_muertos);
  printf("Dias simulados: %d\n", resultado->dias_simulados);
  
  printf("\nProgresion por dia (muestra cada 5 dias):\n");
  printf("  Dia | Infectados | Recuperados | Muertos\n");
  printf("------+------------+-------------+--------\n");
  for (int d = 0; d <= dias && d <= resultado->dias_simulados; d += 5) {
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
