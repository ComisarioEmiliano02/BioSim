#include "consultas_rapidas.h"
#include <stdlib.h>
#include <string.h>

// ============================================================
// SUBPROBLEMA 8: Consultas Rápidas
// Búsquedas con Tabla Hash
// ============================================================

TablaHash* construir_hash_individuos(Individuo *poblacion, int num_individuos) {
  TablaHash *tabla = hash_table_crear();
  
  for (int i = 0; i < num_individuos; i++) {
    hash_table_insertar(tabla, poblacion[i].id, &poblacion[i]);
  }
  
  return tabla;
}

Individuo* consulta_rapida_por_id(TablaHash *tabla, int individuo_id) {
  return hash_table_buscar(tabla, individuo_id);
}

void registrar_cambio_estado(TablaHash *tabla, int individuo_id, EstadoSalud nuevo_estado) {
  Individuo *ind = hash_table_buscar(tabla, individuo_id);
  if (ind) {
    ind->estado = nuevo_estado;
  }
}

RegistroHistorial obtener_historial_paciente(TablaHash *tabla, int individuo_id) {
  RegistroHistorial historial;
  historial.individuo_id = individuo_id;
  historial.cambios_registrados = 0;
  memset(historial.historial, 0, sizeof(historial.historial));
  
  // TODO: Obtener historial del individuo
  return historial;
}

void historial_liberar(RegistroHistorial *historial) {
  // No requiere liberación dinámica en esta implementación
}
