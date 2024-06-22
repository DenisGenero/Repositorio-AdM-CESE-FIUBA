/* Clase 8 */

/* Ejemplo de uso: determinar el promedio y desvío estándar de un vector */
void standardDeviation(float *vectorIn, float *prom, float *desvio, uint32_t lon){
    // Se calcula el promedio
    float acum = 0.0;
    for(uint32_t i = lon; i > 0; i--){
        acum += vectorIn[i-1];
    }
    // Se asigna el promedio a la dirección de memoria pasada como parámetro
    *prom = acum/lon;
    //Se calcula el desvío estándar
    acum = 0.0;
    for(uint32_t i = lon; i > 0; i--){
        acum += (*prom - vectorIn[i-1])*(*prom - vectorIn[i-1]);
    }
    acum /= lon;
    // Se asigna el desvío a la dirección de memoria pasada como parámetro
    *desvio = sqrt(acum);
}
