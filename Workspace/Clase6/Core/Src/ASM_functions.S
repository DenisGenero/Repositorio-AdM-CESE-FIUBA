.text

.syntax unified
.thumb

.global corr_ASM
.global corr_DSP


// Prototipo en archivo .h:
// void asm_corr_ASM(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud)

// Parámetros:
//  r0: dirección de vectorX
//  r1: dirección de vectorY
//  r2: dirección de vectorCorr
//  r3: longitud de los vectores

.thumb_func
	corr_ASM:
		push {r4-r9}	// Resguardar registros
		mov r4, #0		// índice l = 0
		mov r5, #0		// índice n = 0
		mov r9, #0		// acumulador = 0
		sub r3, #1		// longitud = longitud - 1
	// Loop principal
	_for1:
		sub r6, r5, r4	// r6 = r5 - r4 (n-l)
		ldrsh r7, [r0, r5, LSL #1]  // x[n]
		ldrsh r8, [r1, r6, LSL #1]  // y[n-l]
		//mul r7, r8	// r7 = x[n]*y[n-l]
		//add r9, r7	// acumulador = acumulador + x[n]*y[n-l]
		mla r9, r7, r8, r9	// r9 = (r7*r8) + r9
		cmp r5, r3		// n == longitud - 1?
		itt eq			// si es cierto:
		strheq r9, [r2, r4, LSL #1]  // Terminó el ciclo, se guarda el resultado acumulado
		beq _for2		// Se pasa al otro bucle, donde se incrementa l
		add r5, #1		// Si el proceso continúan, incremento r5
		b _for1

	_for2:
		mov r9, #0  // Se resetea el acumulador
		add r4, #1  // Se incrementa l
		mov r5, r4  // n = l, ya que para n menores la comparación no tiene sentido (n<0)
		cmp r4, r3  // Si l == longitud, termina la función
		bls _for1  // Sino se sigue iterando

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
corr_DSP:
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
		// Si l es impar, se debe determinar de nuevo n-l:
		sub r6, r5, r4  // r6 = r5 - r4 (n - l)
		ands r10, r4, #1 // si: l and 1 = 1, entonces l es impar
		ittt ne		// Si l impar, entonces:
		ldrhne r7, [r0, r5, LSL #1] // Último valor de vectorX
		ldrhne r8, [r1, r6, LSL #1] // Último valor de vectorY
		mlane r9, r7, r8, r9  // acumulador = r7*r8 + acumulador
		strh r9, [r2, r4, LSL #1] // Se guarda el resultado acumulado

		mov r9, #0  	// Se resetea el acumulador
		add r4, #1  	// l = l + 1
		mov r5, r4  	// n = l, ya que para n menores la comparación no tiene sentido (n<0)
		cmp r4, r3  	// Si l < longitud-1? Si es verdad termina la función
		blo _for15  	// Sino se sigue iterando

		// Se calcula el último valor (cuando l = N-1)
		sub r6, r5, r4 // r6 = r5 - r4 (n-l)
		ldrh r7, [r0, r5, LSL #1] // Se cargan los valores de 16 bits
		ldrh r8, [r1, r6, LSL #1]
		mla r9, r7, r8, r9  // acumulador = r7*r8 + acumulador
		strh r9, [r2, r4, LSL #1] // Se guarda el resultado acumulado

		pop {r4-r10}
		bx lr
