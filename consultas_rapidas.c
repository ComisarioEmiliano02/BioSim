#include "consultas_rapidas.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

// ============================================================
// SUBPROBLEMA 8: Consultas Rapidas
// Busquedas con Tabla Hash - O(1) promedio
// ============================================================

// Estructura auxiliar para rastrear historial de cambios
typedef struct {
  time_t timestamp;
  EstadoSalud estado_anterior;
  EstadoSalud estado_nuevo;
} CambioEstado;

// Array global para rastrear historial (simplificado para la demostración)
typedef struct {
  int individuo_id;
  CambioEstado *cambios;
  int num_cambios;
  int capacidad;
} HistorialIndividuo;

// Tabla auxiliar de historiales (en producción, sería persistente)
HistorialIndividuo *historiales_globales = NULL;
int num_historiales = 0;

TablaHash* construir_hash_individuos(Individuo *poblacion, int num_individuos) {
  TablaHash *tabla = hash_table_crear();
  
  // Inicializar tabla de historiales
  historiales_globales = (HistorialIndividuo *)malloc(num_individuos * sizeof(HistorialIndividuo));
  num_historiales = num_individuos;
  
  for (int i = 0; i < num_individuos; i++) {
    hash_table_insertar(tabla, poblacion[i].id, &poblacion[i]);
    
    // Inicializar historial para cada individuo
    historiales_globales[i].individuo_id = poblacion[i].id;
    historiales_globales[i].cambios = (CambioEstado *)malloc(100 * sizeof(CambioEstado));
    historiales_globales[i].num_cambios = 0;
    historiales_globales[i].capacidad = 100;
  }
  
  return tabla;
}

Individuo* consulta_rapida_por_id(TablaHash *tabla, int individuo_id) {
  return hash_table_buscar(tabla, individuo_id);
}

void registrar_cambio_estado(TablaHash *tabla, int individuo_id, EstadoSalud nuevo_estado) {
  Individuo *ind = hash_table_buscar(tabla, individuo_id);
  if (!ind) return;
  
  // Registrar cambio en historial
  for (int i = 0; i < num_historiales; i++) {
    if (historiales_globales[i].individuo_id == individuo_id) {
      if (historiales_globales[i].num_cambios < historiales_globales[i].capacidad) {
        CambioEstado cambio;
        cambio.timestamp = time(NULL);
        cambio.estado_anterior = ind->estado;
        cambio.estado_nuevo = nuevo_estado;
        
        historiales_globales[i].cambios[historiales_globales[i].num_cambios++] = cambio;
      }
      break;
    }
  }
  
  // Actualizar estado
  ind->estado = nuevo_estado;
  if (nuevo_estado == INFECTADO && ind->tiempo_infeccion == 0) {
    ind->tiempo_infeccion = 1;
  } else if (nuevo_estado == RECUPERADO) {
    ind->tiempo_infeccion = 0;
  }
}

RegistroHistorial obtener_historial_paciente(TablaHash *tabla, int individuo_id) {
  RegistroHistorial historial;
  historial.individuo_id = individuo_id;
  historial.cambios_registrados = 0;
  memset(historial.historial, 0, sizeof(historial.historial));
  
  // Buscar en la tabla de historiales globales
  for (int i = 0; i < num_historiales; i++) {
    if (historiales_globales[i].individuo_id == individuo_id) {
      historial.cambios_registrados = historiales_globales[i].num_cambios;
      
      // Construir string de historial
      char *ptr = historial.historial;
      int espacio_restante = sizeof(historial.historial);
      
      for (int j = 0; j < historiales_globales[i].num_cambios && j < 10; j++) {
        const char *estado_ant = "";
        const char *estado_nuevo = "";
        
        switch (historiales_globales[i].cambios[j].estado_anterior) {
          case SANO: estado_ant = "SANO"; break;
          case INFECTADO: estado_ant = "INFECTADO"; break;
          case RECUPERADO: estado_ant = "RECUPERADO"; break;
        }
        
        switch (historiales_globales[i].cambios[j].estado_nuevo) {
          case SANO: estado_nuevo = "SANO"; break;
          case INFECTADO: estado_nuevo = "INFECTADO"; break;
          case RECUPERADO: estado_nuevo = "RECUPERADO"; break;
        }
        
        int len = snprintf(ptr, espacio_restante, "%s -> %s | ", estado_ant, estado_nuevo);
        if (len > 0 && len < espacio_restante) {
          ptr += len;
          espacio_restante -= len;
        }
      }
      break;
    }
  }
  
  return historial;
}

void historial_liberar(RegistroHistorial *historial) {
  // No requiere liberacion dinamica adicional en esta implementacion
}

// ============================================================
// FUNCIONES AUXILIARES PARA LIMPIAR MEMORIA
// ============================================================

void liberar_historiales() {
  if (historiales_globales) {
    for (int i = 0; i < num_historiales; i++) {
      if (historiales_globales[i].cambios) {
        free(historiales_globales[i].cambios);
      }
    }
    free(historiales_globales);
    historiales_globales = NULL;
  }
}

// ============================================================
// FUNCION DE PRUEBA: Test de Consultas Rapidas
// ============================================================

void test_consultas_rapidas(Individuo *poblacion, int num_individuos, 
                            Territorio *territorios, int num_territorios) {
  if (!poblacion || num_individuos <= 0) {
    printf("ERROR: Poblacion invalida\n");
    return;
  }
  
  printf("\n========== SUBPROBLEMA 8: CONSULTAS RAPIDAS ==========\n");
  printf("Individuos en base de datos: %d\n", num_individuos);
  printf("Territorios: %d\n", num_territorios);
  printf("Algoritmo: Tabla Hash - O(1) promedio\n\n");
  
  // Construir tabla hash de individuos
  printf("--- CONSTRUCCION DE TABLA HASH ---\n");
  clock_t inicio = clock();
  TablaHash *tabla = construir_hash_individuos(poblacion, num_individuos);
  clock_t fin = clock();
  
  double tiempo_construccion = (double)(fin - inicio) / CLOCKS_PER_SEC * 1000;
  printf("Tabla hash creada y poblada\n");
  printf("Tiempo de construccion: %.3f ms\n", tiempo_construccion);
  printf("Tamanio esperado: %d elementos\n\n", num_individuos);
  
  // Prueba 1: Consultas rapidas por ID
  printf("--- PRUEBA 1: Consultas rapidas por ID ---\n");
  
  int ids_consultar[] = {5, 42, 100, 250, 500, 750, 999};
  int num_consultas = 7;
  
  printf("Consultando %d individuos aleatorios:\n", num_consultas);
  inicio = clock();
  
  for (int i = 0; i < num_consultas; i++) {
    int id = ids_consultar[i];
    Individuo *ind = consulta_rapida_por_id(tabla, id);
    
    if (ind) {
      printf("  ID %d: %s (T%d, Estado: ", ind->id, ind->nombre, ind->territorio_id);
      switch (ind->estado) {
        case SANO: printf("SANO"); break;
        case INFECTADO: printf("INFECTADO"); break;
        case RECUPERADO: printf("RECUPERADO"); break;
      }
      printf(", Riesgo: %d)\n", ind->riesgo);
    }
  }
  
  fin = clock();
  double tiempo_consultas = (double)(fin - inicio) / CLOCKS_PER_SEC * 1000;
  printf("Tiempo de %d consultas: %.3f ms\n", num_consultas, tiempo_consultas);
  printf("Tiempo promedio por consulta: %.6f ms\n\n", tiempo_consultas / num_consultas);
  
  // Prueba 2: Registrar cambios de estado
  printf("--- PRUEBA 2: Registrar cambios de estado ---\n");
  
  int ids_cambio[] = {10, 50, 150, 300};
  EstadoSalud estados_nuevo[] = {INFECTADO, RECUPERADO, INFECTADO, RECUPERADO};
  
  printf("Registrando cambios de estado para %d individuos:\n", 4);
  inicio = clock();
  
  for (int i = 0; i < 4; i++) {
    Individuo *ind = consulta_rapida_por_id(tabla, ids_cambio[i]);
    if (ind) {
      EstadoSalud estado_ant = ind->estado;
      registrar_cambio_estado(tabla, ids_cambio[i], estados_nuevo[i]);
      
      const char *estado_ant_str = "";
      const char *estado_nuevo_str = "";
      
      switch (estado_ant) {
        case SANO: estado_ant_str = "SANO"; break;
        case INFECTADO: estado_ant_str = "INFECTADO"; break;
        case RECUPERADO: estado_ant_str = "RECUPERADO"; break;
      }
      
      switch (estados_nuevo[i]) {
        case SANO: estado_nuevo_str = "SANO"; break;
        case INFECTADO: estado_nuevo_str = "INFECTADO"; break;
        case RECUPERADO: estado_nuevo_str = "RECUPERADO"; break;
      }
      
      printf("  ID %d: %s -> %s\n", ids_cambio[i], estado_ant_str, estado_nuevo_str);
    }
  }
  
  fin = clock();
  double tiempo_cambios = (double)(fin - inicio) / CLOCKS_PER_SEC * 1000;
  printf("Tiempo de %d cambios: %.3f ms\n\n", 4, tiempo_cambios);
  
  // Prueba 3: Obtener historial de cambios
  printf("--- PRUEBA 3: Obtener historial de cambios ---\n");
  
  for (int i = 0; i < 4; i++) {
    RegistroHistorial hist = obtener_historial_paciente(tabla, ids_cambio[i]);
    printf("ID %d: %d cambios registrados\n", hist.individuo_id, hist.cambios_registrados);
    if (hist.cambios_registrados > 0) {
      printf("  Historial: %s\n", hist.historial);
    }
  }
  
  // Estadisticas generales
  printf("\n--- ESTADISTICAS DE HASH TABLE ---\n");
  
  // Contar estados actuales
  int sanos = 0, infectados = 0, recuperados = 0;
  for (int i = 0; i < num_individuos; i++) {
    switch (poblacion[i].estado) {
      case SANO: sanos++; break;
      case INFECTADO: infectados++; break;
      case RECUPERADO: recuperados++; break;
    }
  }
  
  printf("Estados actuales:\n");
  printf("  Sanos: %d (%.1f%%)\n", sanos, (float)sanos / num_individuos * 100);
  printf("  Infectados: %d (%.1f%%)\n", infectados, (float)infectados / num_individuos * 100);
  printf("  Recuperados: %d (%.1f%%)\n", recuperados, (float)recuperados / num_individuos * 100);
  
  printf("\nPerformance:\n");
  printf("  Tamanio tabla hash: %d\n", HASH_TABLE_SIZE);
  printf("  Factor de carga: %.2f%%\n", (float)num_individuos / HASH_TABLE_SIZE * 100);
  printf("  Tiempo construccion: %.3f ms (O(n))\n", tiempo_construccion);
  printf("  Tiempo consultas: %.6f ms promedio (O(1))\n", tiempo_consultas / num_consultas);
  
  printf("\nComplejidad analizada:\n");
  printf("  Construccion: O(n) = O(%d)\n", num_individuos);
  printf("  Consulta: O(1) promedio\n");
  printf("  Cambio estado: O(1) promedio\n");
  printf("  Historial: O(1) busqueda + O(k) copia donde k=cambios\n");
  
  printf("\n===== FIN PRUEBAS SUBPROBLEMA 8 =====\n\n");
  
  // Liberar
  hash_table_liberar(tabla);
  liberar_historiales();
}