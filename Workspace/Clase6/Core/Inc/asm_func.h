/*
 * asm_func.h
 *
 *  Created on: May 15, 2024
 *      Author: denis
 */

#ifndef INC_ASM_FUNC_H_
#define INC_ASM_FUNC_H_

/* Clase 6*/
void corr(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void asm_corr_ASM(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);
void asm_corr_DSP(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud);

#endif /* INC_ASM_FUNC_H_ */
