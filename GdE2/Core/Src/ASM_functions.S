/* ################## Guía de Ejercicios 2 ################## */
.text
.syntax unified
.thumb

/* Exportar funciones*/
.global asm_GPS_unpack
.global asm_max
.global asm_invertir

/* Ejercicio 1 */
// Prototipo en archivo .h:
// void asm_GPS_unpack(int32_t *GPS_data, int16_t *GPS_Lat, int16_t *GPS_Long, uint16_t longitud)

// Parámetros recibidos
//	r0: dirección vector de datos del GPS
//	r1: dirección vector de latitudes
//	r2: dirección vector de longitudes
//	r3: longitud de los vectores

// Uso de registros:
//	r4 --> muestra del GPS
//	r5 --> longitud extraída
//	r6 --> latitud extraída

// Valor de retorno: void
.thumb_func
	asm_GPS_unpack:
		push {r4 - r7}           // Se resguardan los valores de los registros en el stack

		// Guarda de seguridad
		cbz r0, _exit1           // Salir si GPS_data es una dirección nula
		cbz r1, _exit1           // Salir si GPS_Lat es una dirección nula
		cbz r2, _exit1           // Salir si GPS_Long es una dirección nula
		cbz r3, _exit1           // Salir si longitud = 0

	_loop1:
		sub r3, #1               // longitud = longitud - 1
		ldr r4, [r0, r3, LSL #2] // Se carga la muestra (32 bits) desde la última hacia la primera
		// sbfx: extrae un campo de bits y le extiende el signo a 32 bits
		sbfx r5, r4, #0, #12     // Longitud: 12 bits a partir del bit 0  -> bits [11:0]
		sbfx r6, r4, #12, #12    // Latitud:  12 bits a partir del bit 12 -> bits [23:12]
		strh r6, [r1, r3, LSL #1] // Se guarda la latitud (16 bits)
		strh r5, [r2, r3, LSL #1] // Se guarda la longitud (16 bits)
		cbz r3, _exit1           // Si r3 = 0, la función termina
		b _loop1                 // De lo contrario seguimos iterando

	_exit1:
		pop {r4 - r7}            // Se recuperan los valores de los registros
		bx lr

/* Ejercicio 2 */
// Prototipo en archivo .h:
// uint32_t asm_max(int32_t *vectorIn, uint32_t longitud)

// Parámetros recibidos
//	r0: dirección del vector de entrada
//	r1: longitud del vector

// Uso de registros:
//	r2 --> índice del valor máximo
//	r3 --> valor máximo
//	r4 --> elemento actual

// Valor de retorno: índice del valor máximo (0 si no hay muestras)
.thumb_func
	asm_max:
		push {r4 - r7}           // Se resguardan los valores de los registros en el stack
		mov r2, #0               // Índice por defecto = 0

		// Guarda de seguridad
		cbz r0, _fin2            // Si vectorIn es una dirección nula, se devuelve 0
		cbz r1, _fin2            // Si no hay muestras, se devuelve 0

		sub r1, #1               // r1 = último índice
		ldr r3, [r0, r1, LSL #2] // Valor máximo inicial = último elemento
		mov r2, r1               // Índice del máximo inicial = último índice

	_loop2:
		cbz r1, _fin2            // Si r1 = 0, ya se recorrió todo el vector
		sub r1, #1               // Índice = índice - 1
		ldr r4, [r0, r1, LSL #2] // Se carga el elemento actual
		cmp r4, r3               // Se compara con el máximo hasta el momento
		itt ge                   // Elemento >= máximo? (comparación CON signo)
		  movge r3, r4           // Si es verdad: se actualiza el valor máximo
		  movge r2, r1           // Si es verdad: se actualiza el índice del máximo
		// Se usa >= porque se recorre de atrás hacia adelante: ante empate
		// queda el primer índice, igual que en la versión en C
		b _loop2                 // Se sigue iterando

	_fin2:
		mov r0, r2               // Se devuelve el índice en r0
		pop {r4 - r7}            // Se recuperan los valores de los registros
		bx lr

/* Ejercicio 3 */
// Prototipo en archivo .h:
// void asm_invertir(uint16_t *buffer, uint16_t longitud)

// Parámetros recibidos
//	r0: dirección del buffer
//	r1: longitud del buffer

// Uso de registros:
//	r2 --> índice desde el inicio
//	r3 --> índice desde el final
//	r4 --> elemento del inicio
//	r5 --> elemento del final

// Valor de retorno: void
.thumb_func
	asm_invertir:
		push {r4 - r7}           // Se resguardan los valores de los registros en el stack

		// Guarda de seguridad
		cbz r0, _exit3           // Salir si buffer es una dirección nula
		cmp r1, #2               // Con menos de 2 elementos no hay nada que invertir
		blo _exit3               // (también evita que r3 = longitud - 1 quede en 0xFFFFFFFF)

		mov r2, #0               // Índice inicio = 0
		sub r3, r1, #1           // Índice final = longitud - 1

	_loop3:
		ldrh r4, [r0, r2, LSL #1] // Se carga el elemento del inicio
		ldrh r5, [r0, r3, LSL #1] // Se carga el elemento del final
		strh r5, [r0, r2, LSL #1] // Se intercambian: el del final va al inicio
		strh r4, [r0, r3, LSL #1] // y el del inicio va al final
		add r2, #1               // Avanza el índice del inicio
		sub r3, #1               // Retrocede el índice del final
		cmp r2, r3               // Mientras inicio < final, sigue intercambiando
		blo _loop3               // (si son iguales, es el elemento central de un vector impar)

	_exit3:
		pop {r4 - r7}            // Se recuperan los valores de los registros
		bx lr
