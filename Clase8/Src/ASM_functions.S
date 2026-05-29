.text

.syntax unified
.thumb

.global asm_standardDeviation

// Prototipo en archivo.h:
//void asm_standardDeviation(float *vectorIn, float *prom, float *desvio, uint32_t lon)

// Parámetros recibidos:
// r0: dirección de vectorIn
// r1: dirección de promedio
// r2: dirección del desvío
// r3: longitud de vectorIn

// Uso de los registros de la FPU:
// s0: elementos cargados de r0
// s1: longitud (en tipo float)
// s2: acumulador, y donde se guardará el promedio
// s3: acumulador para el desvío estándar
// s4: desvío estándar calculado

.thumb_func
	asm_standardDeviation:
	push {r4}
		mov r4, r0	// Se resguarda la dirección del vector
		// Seteo de longitud
		vmov s1, r3
		// Conversión a flotante
		vcvt.f32.u32 s1, s1 // Se castea lon a tipo float: s1 = (float) lon
		// Acumulador en cero
		//vmov.f32 s2, #0.0 // En flotantes no podemos usar #0.0
		vsub.f32 s2, s2, s2	// s2 = 0.

	// Bucle para calcular el promedio
	_promedio:
		sub r3, #1
		// vldr.f32 s0, [r0, r3, LSL #2]	// No se puede hacer, no hay barrel shifter :(
		vldr.f32 s0, [r0]	// Nuevo elemento en s0
		vadd.f32 s2, s0		// s2 += s0
		add r0, #4			// Siguiente elemento (se saltan 4 bytes)
		cbz r3, _finprom
		b _promedio

	_finprom:
		vdiv.f32 s2, s2, s1	// acum = acum/lon --> dejamos en s2 el promedio
		vstr.32 s2, [r1]	// *prom = acum
		// Se recupera la longitud
		vcvt.u32.f32 s3, s1 // Se castea lon (s3) de float a uint32_t
		vmov r3, s3			// r3 = (uint32_t) s3
		// Nuevo acumulador
		vsub.f32 s4, s4, s4	// s4 = 0
		// Resultado parcial
		vsub.f32 s5, s5, s5	// s5 = 0
		// Se recupera la dirección del vector
		mov r0, r4			// También se podría operar sobre r4

	// Bucle para calcular el desvío estándar
	_desvio:
		sub r3, #1
		vldr.f32 s0, [r0]		// nuevo elemento en s0
		add r0, #4				// Se aumenta la dirección de r0
		vsub.f32 s5, s2, s0		// s5 = prom - vectorIn[i]
		vmla.f32 s4, s5, s5		// s4 = s4 + s5*s5
		cbz r3, _final
		b _desvio

	_final:
		vdiv.f32 s4, s4, s1		// acum = acum/lon
		vsqrt.f32 s4, s4		// Raíz cuadrada
		vstr.32 s4, [r2]		// *std_dev = acum

		pop {r4}
		bx lr
