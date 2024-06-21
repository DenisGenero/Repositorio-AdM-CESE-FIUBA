/* Clase 5 */

#include "functions.h"

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
