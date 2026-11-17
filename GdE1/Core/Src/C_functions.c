/* ################## Guía de Ejercicios 1 ################## */

#include "functions.h"

/* Ejercicio 1 */
void productoEscalar32(uint32_t *vectorIn, uint32_t *vectorOut, uint32_t longitud, uint16_t escalar){
    // IMPORTANTE: no colocar i>=0, ya que el tipo de datos es sin signo!
    for (uint32_t i = longitud; i > 0; i --){
        vectorOut[i-1] = vectorIn[i-1]*escalar;
    }
}

/* Ejercicio 2 */
void productoEscalar12(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud, uint16_t escalar){
    uint32_t producto;              // 32 bits: el producto 16x16 no desborda
    for (uint32_t i = longitud; i > 0; i --){
        producto = (uint32_t)vectorIn[i-1]*escalar;
        if (producto > 4095){       // 4095 = 0xFFF, máximo valor de 12 bits
            producto = 4095;        // Saturación
        }
        vectorOut[i-1] = (uint16_t)producto;
    }
}

/* Ejercicio 3 */
uint32_t bitfield_clear(uint32_t dato, uint32_t ancho, uint32_t inicio){
    uint32_t auxiliar = 1<<ancho;   // 1<<5  = 0000 0010 0000 (32 en decimal)
    auxiliar -= 1;                  // 32-1  = 0000 0001 1111
    auxiliar = auxiliar<<inicio;    // 31<<3 = 0000 1111 1000 (248 en decimal)
    auxiliar = ~auxiliar;           // not(248) = 1111 0000 0111 --> máscara obtenida
    auxiliar = dato&auxiliar;       // Se aplica operación "and" bit a bit
    return auxiliar;                // Se devuelve resultado
}

/* Ejercicio 4 */
uint32_t bitfield_toggle(uint32_t dato, uint32_t ancho, uint32_t inicio){
    uint32_t auxiliar = 1<<ancho;   // 1<<5  = 0000 0010 0000 (32 en decimal)
    auxiliar -= 1;                  // 32-1  = 0000 0001 1111
    auxiliar = auxiliar<<inicio;    // 31<<3 = 0000 1111 1000 --> máscara obtenida
    auxiliar = dato^auxiliar;       // Se aplica operación "xor" bit a bit (invierte el campo)
    return auxiliar;                // Se devuelve resultado
}
