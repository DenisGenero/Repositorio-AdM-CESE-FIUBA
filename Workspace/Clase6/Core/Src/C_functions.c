/* Clase 6 */

#include "functions.h"
#include <stdio.h>
#include <string.h>

/* Ejercicio de ejemplo: implementación de una correlación cruzada */
// Implementación en C
void corr(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud){
	int32_t acumulador;
	for(uint32_t l = 0; l < longitud; l ++){
		acumulador = 0;
		for(uint32_t n = l; n < longitud; n ++){
			acumulador += vectorX[n]*vectorY[n-l];
		}
		vectorCorr[l] = (int16_t)acumulador;
	}
}


// Implementación en C con intrinsic functions DSP:
void corr_intrinsic(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud){
	uint32_t n, l;
	int32_t acumulador, x_n_n_1, y_n_l_n_l_1;
	for(l = 0; l < longitud; l ++){
		acumulador = 0;
		for(n = l; n < longitud-1; n +=2){
			// Se arman paquetes de datos: 2 datos de 16 bits en cada uno
			x_n_n_1 = (vectorX[n+1]<<16) | vectorX[n];
			y_n_l_n_l_1 = (vectorY[n-l+1]<<16) | vectorY[n-l];
			// Llamado a la función DSP de la CMSIS:
			acumulador = __SMLAD(x_n_n_1, y_n_l_n_l_1, acumulador);
		}
		if(l&1){
			// Si l es imapr, queda un par de datos por procesar
			acumulador += vectorX[n]*vectorY[n-l];
		}
		vectorCorr[l] = (int16_t)acumulador;
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
void ExportarVector(int16_t* vector, uint32_t len, char* lenguaje, char* EOT) {
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