/* Clase 7 */

#include "functions.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

/* Ejercicio de ejemplo: implementación de una correlación cruzada */
// Implementación en C
void corr(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud){
	int32_t acumulador;
	for(uint32_t l = 0; l < longitud; l ++){
		acumulador = 0;
		for(uint32_t n = l; n < longitud; n ++){
			acumulador += vectorX[n]*vectorY[n-l];
		}
		vectorCorr[l] = acumulador;
	}
}

// Implementación en C con intrinsic functions DSP:
void corr_intrinsic(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud){
	uint32_t l;  // No es necesario el índice n
	int32_t acumulador, x_paquete, y_paquete;
	for(l = 0; l < longitud; l ++){
		acumulador = 0;
		int16_t *pX = &vectorX[l]; // En realidad es vector[n], pero siempre se comienza en n = l
		int16_t *pY = &vectorY[0]; // Siempre empieza en cero

		// Se abarcan 2 datos de cada vector, mitad de vueltas
		uint16_t iteraciones = (longitud - l) >> 1;
		while (iteraciones > 0){
			// Se arman paquetes de 2 datos de 16 bits, cada uno
			x_paquete = *(int32_t*)pX;
			pX += 2;
			y_paquete = *(int32_t*)pY;
			pY += 2;
			// Llamado a la función DSP de la CMSIS:
			acumulador = __SMLAD(x_paquete, y_paquete, acumulador);
			iteraciones --;
		}
		if(l&1){
			// Si l es imapr, queda un par de datos por procesar
			acumulador += (*pX)*(*pY);
		}
		vectorCorr[l] = acumulador;
	}
}

static char* int2str(uint16_t num) {
	// 5 dígitos + '\0' (65.536 + '\0')
    static char buf[6];
    char *ptr = &buf[5];
    // Terminador de string
    *ptr = '\0';

    if (num == 0) {
        *(--ptr) = '0';
        return ptr;
    }

    // Desarmamos el número desde las unidades hacia atrás
    while (num > 0) {
        *(--ptr) = (num % 10) + '0';
        num /= 10;
    }

    // Retorna el puntero al inicio de la cadena resultante
    return ptr;
}


// Envía por uart los vectores filtrados para el script de Python
/* Parámetros:
 * - vector: datos a ser enviados al script de Python
 * - len: longitud del vector
 * - lenguaje: debe ser uno de los typedef definidos en "config.h"
 * - mode: debe ser uno de los typedef definidos en "confi.h"
*/
void ExportarVector(int32_t* vector, uint16_t len, ExportType lenguaje, ExportMode mode) {
	char newline = '\n';
	char *lang;
	switch (lenguaje) {
		case EXPORT_TYPE_C:      lang = "C";      break;
		case EXPORT_TYPE_C_INTR: lang = "C_INTR"; break;
		case EXPORT_TYPE_ASM:    lang = "ASM";    break;
		default:                 lang = "DSP";    break;
	}
	// Enviar encabezado
    HAL_UART_Transmit(&huart2, (uint8_t*)lang, strlen(lang), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)&newline, 1, HAL_MAX_DELAY);

    // Enviar metadatos: tamaño de los datos
    uint8_t tamanio_dato = sizeof(vector[0]);
    char* strptr = int2str((uint16_t)tamanio_dato);
    HAL_UART_Transmit(&huart2, (uint8_t*)strptr, strlen(strptr) , HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)",", 1, HAL_MAX_DELAY);
    // Enviar metadatos: cantidad de datos
    uint32_t total_bytes = len * tamanio_dato;
    strptr = int2str(total_bytes);
    HAL_UART_Transmit(&huart2, (uint8_t*)strptr, strlen(strptr) , HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)&newline, 1, HAL_MAX_DELAY);

    // Enviar datos
    HAL_UART_Transmit(&huart2, (uint8_t*)vector, total_bytes, HAL_MAX_DELAY);

    // Enviar "end\n" si es el fin de la transmisión
	if (EXPORT_MODE_CLOSE_FILE == mode) {
		HAL_UART_Transmit(&huart2, (uint8_t*)"end", 3, HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)&newline, 1, HAL_MAX_DELAY);
}
