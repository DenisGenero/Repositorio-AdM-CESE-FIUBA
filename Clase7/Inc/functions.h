#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "inttypes.h"
#include "config.h"
#include "stm32g4xx_hal.h"

extern UART_HandleTypeDef huart2;

/* Clase 6*/
void corr(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud);
void corr_intrinsic(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud);
void corr_ASM(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud);
void corr_DSP(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud);

/* Función auxiliar */
void ExportarVector(int32_t* vector, uint16_t len, ExportType lenguaje, ExportMode EOT);

#endif /* INC_FUNCTIONS_H_ */
