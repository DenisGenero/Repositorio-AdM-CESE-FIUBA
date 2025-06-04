#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "inttypes.h"
#include "config.h"
#include "stm32g4xx_hal.h"

/* Clase 6*/
void corr(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void corr_Intrinsic(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void corr_ASM(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void corr_DSP(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);

/* Función auxiliar */
void ExportarVector(int16_t* vector, uint32_t len, char* lenguaje, char* EOT);

#endif /* INC_FUNCTIONS_H_ */
