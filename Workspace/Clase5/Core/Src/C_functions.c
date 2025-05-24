/* Clase 5 */

#include "functions.h"
#include <stdio.h>
#include <string.h>

/* Ejercicio de ejemplo: implementación de un filtro de media móvil */
void MediaMovil(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud){
    int32_t limiteSup, limiteInf;
    uint8_t SampleOffset = 4;
    uint8_t L = 2*SampleOffset + 1;
    for (int32_t i = longitud; i > 0; i--){
        // Se define el límite superior para no tomar muestras fuera del vector
        limiteSup = (i - 1) + SampleOffset;
        if ( limiteSup > (longitud - 1)){
            limiteSup = longitud - 1;
        }
        // Se define el límite inferior para no tomar muestras fuera del vector
        limiteInf = (i - 1) - SampleOffset;
        if ( limiteInf < 0){
            limiteInf = 0;
        }
        vectorOut[i-1] = 0;
        for (int32_t j = limiteSup; j >= limiteInf; j--){
            vectorOut[i-1] += vectorIn[j];
        }
        vectorOut[i-1] /= L;
    }
}

// Envía por uart los vectores filtrados para el script de Python
/* Parámetros:
 * - vector: datos a ser enviados al script de Python
 * - len: longitud del vector
 * - lenguaje: debe ser "C" o "asm". Indica qué vector se está enviando
 * - EOT: End Of Transmition. Para indicar que no se enviarán más datos.
 *   Puede ser "", si se seguirá enviando datos, o "end" para finalizar.
*/
void ExportarVector(uint16_t* vector, uint32_t len, char* lenguaje, char* EOT) {
    // Enviar encabezado: "C\n" o "asm\n"
    HAL_UART_Transmit(&huart2, (uint8_t*)lenguaje, strlen(lenguaje), HAL_MAX_DELAY);
    char newline = '\n';
    HAL_UART_Transmit(&huart2, (uint8_t*)&newline, 1, HAL_MAX_DELAY);

    // Enviar datos binarios (2 bytes por entero)
    for (uint32_t i = 0; i < len; i++) {
        HAL_UART_Transmit(&huart2, (uint8_t*)&vector[i], sizeof(uint16_t), HAL_MAX_DELAY);
    }

    // Enviar fin de señal: 0xFFFF
    uint16_t fin = 0xFFFF;
    HAL_UART_Transmit(&huart2, (uint8_t*)&fin, sizeof(uint16_t), HAL_MAX_DELAY);

    // Enviar EOT: "end\n" para fin de transmisión
    HAL_UART_Transmit(&huart2, (uint8_t*)EOT, strlen(EOT), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)&newline, 1, HAL_MAX_DELAY);
}