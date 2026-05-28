#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "inttypes.h"
#include "config.h"
#include "stm32g4xx_hal.h"

extern UART_HandleTypeDef huart2;

/* Clase 5 */
/* Funciones implementadas en C */
void MediaMovil(int16_t *signal_noise, int16_t *signal_fil, uint16_t longitud);

/* Funciones implementadas en Assembly */
void asm_MediaMovil(int16_t *signal_noise, int16_t *signal_fil, uint16_t longitud);

/* Función auxiliar */
void ExportarVector(int16_t* vector, uint16_t len, ExportType lenguaje, ExportMode mode);

#endif /* INC_FUNCTIONS_H_ */
