// Ejemplo de cómo se realiza un archivo para escribir código en Assembly.
// Los comentarios son iguales que en C, y además se puede utilizar @:
@ Un comentario

// Directivas del compilador: Son aquellas que comienzan con un "."
// Para que nuestro código funcione, necesitamos:
.text 	// Sección de texto: se indica que en esta sección hay código para ejecutar

// Otras opcionales, pero muy recomendadas, son (no importa el orden):
.syntax unified		// Uso del UAL
.thumb		 	// El ISA que se utilizará es Thumb-2

//Otra directiva que es mandatoria es .global. Sirve para que nuestra función sea "visible" por los demás archivos. No olvidar colocar el prototipo de la función en el respectivo archivo .h

// Exportar todas las funciones aquí (.global + nombre_función)
.global asm_sum
.global asm_svc

// Se recomienda colocar el prototipo de la función en C, como ayuda:
// Prototipo en archivo .h:
// uint32_t asm_sum(uint32_t primerOperando, uint32_t segundoOperando)

// Parámetros recibidos:
// r0: primerOperando
// r1: segundoOperando

// Valor de retorno:
// r0: resultado de la suma entre los parámetros pasados
// Se recomienda respetar la identación: ayuda a leer mejor el código

.thumb_func			// Se le indica al Ensamblador la función
	asm_sum:		// Nombre de la función. Los ":" sirven para indicar etiquetas
		add	r0, r1	// r0 = r0 + r1

		bx lr		// Vuelve adonde fue llamada (específicamente, si "pc"
                    // es la dirección, en el código del programa, de la
                    // instrucción "bl asm_sum" usada para saltar a
                    // esta función, entonces "lr" contiene ese "pc" + 4).
                    // el valor de retorno debe quedar en el registro r0.

// Prototipo en archivo .h:
// void asm_svc()

// Parámetros recibidos: void

// Valor de retorno: void
.thumb_func
	asm_svc:
		svc 0

		bx lr
