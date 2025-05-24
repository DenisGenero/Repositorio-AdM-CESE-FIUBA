#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "inttypes.h"
#include "config.h"
#include "stm32g4xx_hal.h"

/* Clase 5*/
/* Funciones en implementadas en C */
void MediaMovil(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud);

/* Funciones en implementadas en Assembly */
void asm_MediaMovil(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud);

/* Función auxiliar */
void ExportarVector(uint16_t* vector, uint32_t len, char* lenguaje, char* EOT);

#endif /* INC_FUNCTIONS_H_ */
