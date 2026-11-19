/* ################## Guía de Ejercicios 2 ################## */

#include <stdint.h>

/* Ejercicio 1 */
void GPS_unpack(int32_t *GPS_data, int16_t *GPS_Lat, int16_t *GPS_Long, uint16_t longitud){
    int32_t dato;
    int16_t latitud, longitudGPS;

    for (uint32_t i = longitud; i > 0; i --){
        dato = GPS_data[i-1];

        // Longitud: bits [11:0]
        longitudGPS = dato & 0xFFF;             // Se aíslan los 12 bits del campo
        if (longitudGPS & 0x800){               // Si el bit 11 (signo) es 1, el número es negativo
            longitudGPS = longitudGPS | 0xF000; // Extensión de signo: se completan con 1 los bits [15:12]
        }

        // Latitud: bits [23:12]
        latitud = (dato >> 12) & 0xFFF;         // Se desplaza el campo a la parte baja y se aísla
        if (latitud & 0x800){                   // Si el bit 11 (signo) es 1, el número es negativo
            latitud = latitud | 0xF000;         // Extensión de signo
        }

        GPS_Lat[i-1]  = latitud;
        GPS_Long[i-1] = longitudGPS;
    }
}

/* Ejercicio 2 */
uint32_t max(int32_t *vectorIn, uint32_t longitud){
    uint32_t indiceMax = 0;                     // Si no hay muestras se devuelve 0
    int32_t valorMax;

    if (longitud > 0){
        valorMax = vectorIn[0];
        for (uint32_t i = 1; i < longitud; i ++){
            if (vectorIn[i] > valorMax){        // Mayor estricto: ante empate queda el primer índice
                valorMax = vectorIn[i];
                indiceMax = i;
            }
        }
    }
    return indiceMax;
}

/* Ejercicio 3 */
void invertir(uint16_t *buffer, uint16_t longitud){
    uint16_t auxiliar;

    // Se intercambia el elemento i con su simétrico (longitud-1-i) hasta llegar a la mitad.
    // Si longitud es impar, el elemento central queda en su lugar.
    for (uint32_t i = 0; i < longitud/2; i ++){
        auxiliar = buffer[i];
        buffer[i] = buffer[longitud-1-i];
        buffer[longitud-1-i] = auxiliar;
    }
}
