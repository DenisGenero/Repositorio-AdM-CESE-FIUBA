/* Clase 6 */

#include "functions.h"
#include "stm32g4xx_hal.h"

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
