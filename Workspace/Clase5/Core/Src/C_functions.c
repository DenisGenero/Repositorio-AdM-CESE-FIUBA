/* Clase 5 */

#include <stdio.h>
#include <string.h>
#include "functions.h"
#include "config.h"

/* Ejercicio de ejemplo: implementación de un filtro de media móvil */
void MediaMovil(int16_t *signal_noise, int16_t *signal_fil, uint16_t longitud){
    int32_t limiteSup, limiteInf;
    uint8_t SampleOffset = 4;
    uint8_t L = 2*SampleOffset + 1;
	
	// Bucle principal
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
		// Se define variable de 32 bits para evitar overflow
        int32_t acumulador = 0;
		
		// Bucle ventana
        for (int32_t j = limiteSup; j >= limiteInf; j--){
            acumulador += signal_noise[j];
        }
        vectorOut[i-1] = acumulador/L;
		// Propuesta para abarcar los extermos:
		//signal_fil[i-1] = acumulador / ((limiteSup - limiteInf) + 1);
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
void ExportarVector(int16_t* vector, uint16_t len, ExportType lenguaje, ExportMode mode) {
	char newline = '\n';
    // Enviar encabezado: "C\n" o "asm\n"
	if (EXPORT_TYPE_C == lenguaje){
		HAL_UART_Transmit(&huart2, (uint8_t*)"C", 1, HAL_MAX_DELAY);
	} else {
		HAL_UART_Transmit(&huart2, (uint8_t*)"asm", 3, HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)&newline, 1, HAL_MAX_DELAY);

    // Enviar datos binarios (2 bytes por entero)
	uint32_t total_bytes = len * sizeof(uint16_t);
	HAL_UART_Transmit(&huart2, (uint8_t*)vector, total_bytes, HAL_MAX_DELAY);

    // Enviar fin de señal: 0xFFFF
    uint16_t fin = 0xFFFF;
    HAL_UART_Transmit(&huart2, (uint8_t*)&fin, sizeof(uint16_t), HAL_MAX_DELAY);

    // Enviar "end\n" si es el fin de la transmisión
    if (EXPORT_MODE_CLOSE_FILE == mode) {
		HAL_UART_Transmit(&huart2, (uint8_t*)"end", 3, HAL_MAX_DELAY);
	}
    HAL_UART_Transmit(&huart2, (uint8_t*)&newline, 1, HAL_MAX_DELAY);
}