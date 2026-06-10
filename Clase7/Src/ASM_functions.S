.text

.syntax unified
.thumb

.global corr_ASM
.global corr_DSP


// Prototipo en archivo .h:
// void asm_corr_ASM(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud)

// Parámetros:
//  r0: dirección de vectorX
//  r1: dirección de vectorY
//  r2: dirección de vectorCorr
//  r3: longitud de los vectores

// Uso de registros
//  r4 --> índice l
//  r5 --> índice n
//  r6 --> n-l
//  r7 --> vectorX[n]
//  r8 --> vectorY[n-l]
//  r9 --> acumulador

.thumb_func
	corr_ASM:
		push {r4-r9}	                // Resguardar registros
		mov r4, #0		                // índice l = 0

		_loop_exterior:
			cmp r4, r3                  // ¿l == longitud?
			bhs _exit_corr              // Si es verdad, termina la función

			mov r9, #0                  // acumulador = 0
			mov r5, r4                  // n = l

		_loop_interior:
			ldrsh r7, [r0, r5, LSL #1]  // r7 = vectorX[n]
			sub r6, r5, r4              // r6 = n - l
			ldrsh r8, [r1, r6, LSL #1]  // r8 = vectorY[n-l]

			// mul r8, r7               // r8 = vectorX[n]*vectorY[n-l]
			// add r9, r8               // acumulador (r9) += r8
			mla r9, r8, r7, r9          // acumulador += vectorX[n]*vectorY[n-l]

			add r5, #1                  // n ++
			cmp r5, r3                  // ¿n < longitud?
			blo _loop_interior          // Si es verdad, seguir iterando

			// Si n >= longitud, se debe guardar lo acumulado a la salida
			str r9, [r2, r4, LSL #2]   // vectorCorr[l] = acumulador
			add r4, #1                  // l ++
			b _loop_exterior

		_exit_corr:
			pop {r4-r9}
			bx lr


// Prototipo en archivo .h:
// void asm_corr_DSP(int16_t *vectorX, int16_t *vectorY, int32_t *vectorCorr, uint16_t longitud)

// Parámetros:
//  r0: dirección de vectorX
//  r1: dirección de vectorY
//  r2: dirección de vectorCorr
//  r3: longitud de los vectores

// Uso de registros
//  r4  --> índice l
//  r5  --> iteraciones
//  r6  --> puntero: pX = &vector[l]
//  r7  --> puntero: pY = &vector[0]
//  r8  --> x_paquete
//  r9  --> y_paquete
//  r10 --> acumulador

.thumb_func
	corr_DSP:
		push {r4-r10}	                   // Resguardar registros
		mov r4, #0		                   // índice l = 0

		_loop_exterior_dsp:
			cmp r4, r3                     // ¿l == longitud?
			bhs _exit_corr_dsp              // Si es verdad, termina la función

			mov r10, #0                    // acumulador = 0

			// Preparación de punteros
			add r6, r0, r4, LSL #1         // pX = &vectorX[l]
			mov r7, r1                     // pY = &vectorY[0]

			// Determinar iteraciones
			sub r5, r3, r4                 // r5 = longitud - l
			lsrs r5, r5, #1                // r5 = r5/2
			beq _dato_impar                // Si da 0, hay un par de datos más por procesar

			_loop_interior_dsp:
				ldr r8, [r6], #4           // x_paquete = *pX; pX += 2;
				ldr r9, [r7], #4           // y_paquete = *pY; pY += 2;
				smlad r10, r8, r9, r10     // r10 += x_paquete*y_paquete
				subs r5, #1                // iteraciones --
				bne _loop_interior_dsp     // ¿iteraciones > 0?

			_dato_impar:
				ands r5, r4, #1            // ¿l&1 == 1?

				ittt ne                    // Si l es impar
					ldrshne r8, [r6]       // Última muestra de vectorX
					ldrshne r9, [r7]       // Última muestra de vectorY
					mlane r10,r8, r9, r10  // Se suman al acumulado

				str r10, [r2, r4, LSL #2] // vectorCorr[l] = acumulador
				add r4, #1                 // l ++
				b _loop_exterior_dsp

		_exit_corr_dsp:
			pop {r4-r10}
			bx lr
