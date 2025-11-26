#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "estructuras.h"

// ============================================================
// TABLA HASH - Subproblema 8: Consultas Rápidas
// Complejidad: O(1) promedio para inserción, búsqueda y eliminación
// ============================================================

/**
 * Inicializa una tabla hash vacía
 * Complejidad: O(HASH_TABLE_SIZE)
 */
TablaHash* hash_table_crear();

/**
 * Inserta un individuo en la tabla hash
 * Complejidad: O(1) promedio
 */
void hash_table_insertar(TablaHash *tabla, int individuo_id, Individuo *individuo);

/**
 * Busca un individuo por su ID
 * Complejidad: O(1) promedio
 * Retorna: Puntero al Individuo o NULL si no existe
 */
Individuo* hash_table_buscar(TablaHash *tabla, int individuo_id);

/**
 * Elimina un individuo de la tabla hash
 * Complejidad: O(1) promedio
 */
void hash_table_eliminar(TablaHash *tabla, int individuo_id);

/**
 * Libera toda la memoria de la tabla hash
 * Complejidad: O(HASH_TABLE_SIZE + elementos)
 */
void hash_table_liberar(TablaHash *tabla);

#endif // HASH_TABLE_H
