#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "inttypes.h"

/* Clase 6*/
void corr(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void corr_Intrinsic(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void corr_ASM(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void corr_DSP(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);

#endif /* INC_FUNCTIONS_H_ */
