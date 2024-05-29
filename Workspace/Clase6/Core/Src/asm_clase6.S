.text

.syntax unified
.thumb

.global asm_corr_ASM
.global asm_corr_DSP


// Prototipo en archivo .h:
// void asm_corr_ASM(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud)

// Parámetros:
//  r0: dirección de vectorX
//  r1: dirección de vectorY
//  r2: dirección de vectorCorr
//  r3: longitud de los vectores

.thumb_func
	asm_corr_ASM:
		push {r4-r9}	// Resguardar registros
		mov r4, #0		// índice l
		mov r5, #0		// índice n
		mov r9, #0		// acumulador
		sub r3, #1		// longitud = longitud - 1
	// Loop principal
	_for13:
    	sub r6, r5, r4	// r6 = r5 - r4 (n-l)
    	ldrh r7, [r0, r5, LSL #1]  // x[n]
    	ldrh r8, [r1, r6, LSL #1]  // y[n-l]
		//mul r7, r8	// r7 = x[n]*y[n-l]
		//add r9, r7	// acumulador = acumulador + x[n]*y[n-l]
		mla r9, r7, r8, r9	// r9 = (r7*r8) + r9
		cmp r5, r3		// n == longitud - 1?
		itt eq			// si es cierto:
		streq r9, [r2, r4, LSL #2]  // Terminó el ciclo, se guarda el resultado acumulado
		beq _for14		// Se pasa al otro bucle, donde se incrementa l
		add r5, #1		// Si el proceso continúan, incremento r5
		b _for13

	_for14:
		mov r9, #0  // Se resetea el acumulador
		add r4, #1  // Se incrementa l
		mov r5, r4  // n = l, ya que para n menores la comparación no tiene sentido (n<0)
		cmp r4, r3  // Si l == longitud, termina la función
		bls _for13  // Sino se sigue iterando

		pop {r4-r9}
		bx lr

// Prototipo en archivo .h:
// void asm_corr_DSP(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud)

// Parámetros:
//  r0: dirección de vectorX
//  r1: dirección de vectorY
//  r2: dirección de vectorCorr
//  r3: longitud de los vectores
.thumb_func
	asm_corr_DSP:
		push {r4-r10}
		mov r4, #0  	// índice l
		mov r5, #0  	// índice n
		mov r6, #0		// índice n - l
		mov r9, #0 		// acumulador
		sub r3, #1 		// longitud = longitud - 1

	// Bucle interior
	_for15:
		sub r6, r5, r4 // r6 = r5 - r4 (n-l)
		ldr r7, [r0, r5, LSL #1]  // x[n], x[n+1]
		ldr r8, [r1, r6, LSL #1]  // y[n-l], y[(n+1)-l]
		smlad r9, r7, r8, r9	 // r9 = x[n]*y[n-l] + x[n+1]*y[(n+1)- l]
		add r5, #2	  	// n = n + 2
		cmp r5, r3  	// n >= longitud - 1?
		bhs _for16 		// Si es verdad se va al bucle para almacenar
		b _for15

	// Bucle exterior
	_for16:
		sub r5, #1		// Si l es impar, el n está afuera del vector
		ands r10, r4, #1
		itttt ne		 // l es impar?
		subne r6, r5, r4 // Si es verdad r6 = r5 - r4 (n-l)
		ldrhne r7, [r0, r5, LSL #1] // Se cargan los valores de 16 bits
		ldrhne r8, [r1, r6, LSL #1]
		mlane r9, r7, r8, r9  // acumulador = r7*r8 + acumulador
		str r9, [r2, r4, LSL #2] // Se guarda el resultado acumulado
		mov r9, #0  	// Se resetea el acumulador
		add r4, #1  	// l = l + 1
		mov r5, r4  	// n = l, ya que para n menores la comparación no tiene sentido (n<0)
		cmp r4, r3  	// Si l < longitud-1? Si es verdad termina la función
		blo _for15  	// Sino se sigue iterando

		// Se calcula el último valor
		sub r6, r5, r4 // r6 = r5 - r4 (n-l)
		ldrh r7, [r0, r5, LSL #1] // Se cargan los valores de 16 bits
		ldrh r8, [r1, r6, LSL #1]
		mla r9, r7, r8, r9  // acumulador = r7*r8 + acumulador
		str r9, [r2, r4, LSL #2] // Se guarda el resultado acumulado

		pop {r4-r10}
		bx lr
