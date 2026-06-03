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
//  r5  --> índice n
//  r6  --> n-l | verificar paridad de l
//  r7  --> vectorX[n] y vectorX[n+1] (x_paquete)
//  r8  --> vectorY[n-l] y vectorY[n-l+1] (y_paquete)
//  r9  --> acumulador
//  r10 --> longitud - 1

.thumb_func
	corr_DSP:
		push {r4-r10}	                         // Resguardar registros
		sub r10, r3, #1                          // r10 = longitud -= 1
		mov r4, #0		                         // índice l = 0

		_loop_exterior_dsp:
			cmp r4, r3                           // ¿l > longitud?
			bhi _exit_corr_dsp                   // Si es verdad, termina la función

			mov r9, #0                           // acumulador = 0
			mov r5, r4                           // n = l

			_loop_interior_dsp:
				ldr r7, [r0, r5, LSL #1]         // r7 = vectorX[n] || vectorX[n+1] (x_paquete)

				sub r6, r5, r4                   // r6 = n - l
				ldr r8, [r1, r6, LSL #1]         // r8 = vectorY[n-l] || vectorY[n-l+1] (y_paquete)
				smlad r9, r7, r8, r9             // r9 += x_paquete*y_paquete
				add r5, #2                       // n += 2
				cmp r5, r10                      // ¿n < longitud - 1?
				blo _loop_interior_dsp           // Si es verdad seguir iterando

				ands r6, r4, #1                  // Verificar paridad de l
				sub r5, #1                       // Corrección de n si hay que clacular nuevo índice
				itttt ne                          // Si es impar, queda un par de valores para procesar
					ldrshne r7, [r0, r5, LSL #1] // r7 = vectorX[n]
					subne r6, r5, r4             // r6 = n - l
					ldrshne r8, [r1, r6, LSL #1] // r7 = vectorY[n]
					mlane r9, r7, r8, r9         // r9 += r7*r8
				str r9, [r2, r4, LSL #2]         // vectorCorr[l] = acumulador
				add r4, #1                       // l ++
				b _loop_exterior_dsp

		_exit_corr_dsp:
			pop {r4-r10}
			bx lr
