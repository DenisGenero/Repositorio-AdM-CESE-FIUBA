.text

.syntax unified
.thumb

.global asm_MediaMovil

#define SAMPLE_OFFSET	4
#define WINDOW_SIZE	(2*SAMPLE_OFFSET + 1)
.thumb_func
    asm_MediaMovil:
    	push {r4-r10}		// Se resguardan los registros
		mov r8, WINDOW_SIZE // r8 = L (Cantidad de muestras de la ventana)
    	mov r3, r2		// índice para recorrer el vector
		sub r2, #1		// Resguardo del valor: longitud - 1
    // Bucle que recorre el vector de muestras
    _for3:
    	subs r3, #1		// Decrementamos el índice
		bmi _exit6		// Si r3 < 0, terminó la función: volver a main
    	mov r7, #0		// Reset del acumulador
    	add r4, r3, SAMPLE_OFFSET // Límite superior e índice para recorrer en la ventana
    	cmp r4, r2		// Comparo con la cantidad de muestras
    	it hi			// Límite superior > (longitud-1)?
		movhi r4, r2		// Si es verdad: límite superior = longitud - 1
		subs r5, r3, SAMPLE_OFFSET // Límite inferior
		it mi			// Límite inferior < 0 ?
		movmi r5, 0		// Si es verdad: límite inferior = 0

    // Bucle que recorre las muestras en la ventana
    _for4:
    	ldrh r6, [r0, r4, LSL #1] // Se cargan elementos para ir operando
		add r7, r6		// Se suma en el acumulador
		cmp r4, r5
		ittt eq			// Índice == límite inferior?
		udiveq r7, r8		// Si es verdad: acumulador = acumulador / WINDOW_SIZE
		strheq r7, [r1, r3, LSL #1] // Se almacena el valor filtrado
		beq _for3 		// Se vuelve al bucle de muestras
		sub r4, #1		// Si no es verdad: Se decrementa el índice de la ventana
		b _for4 		// Se vuelve a iterar
	_exit6:
		pop {r4-r10}
		bx lr
