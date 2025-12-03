#ifndef CONSULTAS_RAPIDAS_H
#define CONSULTAS_RAPIDAS_H

#include "estructuras.h"
#include "hash_table.h"

// ============================================================
// SUBPROBLEMA 8: Consultas Rápidas
// Buscar historial o datos de paciente
// Restricción: O(1) promedio
// Estructura: Tabla Hash con manejo de colisiones
// ============================================================

typedef struct {
  int individuo_id;
  char historial[1000];  // Historial de cambios de estado
  int cambios_registrados;
} RegistroHistorial;

/**
 * Crea una tabla hash poblada con todos los individuos
 * Complejidad: O(n) donde n = número de individuos
 */
TablaHash* construir_hash_individuos(Individuo *poblacion, int num_individuos);

/**
 * Busca un individuo por ID en O(1) promedio
 * Complejidad: O(1) promedio
 */
Individuo* consulta_rapida_por_id(TablaHash *tabla, int individuo_id);

/**
 * Registra un cambio de estado en el historial de un individuo
 * Complejidad: O(1) promedio para consulta + O(1) para registro
 */
void registrar_cambio_estado(TablaHash *tabla, int individuo_id, EstadoSalud nuevo_estado);

/**
 * Obtiene el historial de cambios de un individuo
 * Complejidad: O(1) promedio para búsqueda + O(k) para copiar k cambios
 */
RegistroHistorial obtener_historial_paciente(TablaHash *tabla, int individuo_id);

/**
 * Libera los registros de historial
 * Complejidad: O(1)
 */
void historial_liberar(RegistroHistorial *historial);

/**
 * Funcion de prueba para Subproblema 8
 * Demuestra consultas hash y estadisticas
 */
void test_consultas_rapidas(Individuo *poblacion, int num_individuos,
                            Territorio *territorios, int num_territorios);

#endif // CONSULTAS_RAPIDAS_H
