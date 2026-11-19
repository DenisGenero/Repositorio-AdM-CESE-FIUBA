## GdE2 – Guía de Ejercicios 2 (NUCLEO-F446RE)

Proyecto STM32CubeIDE para probar la Guía de Ejercicios 2 (Uso de instrucciones condicionales en Assembly) en la NUCLEO-F446RE.

**Archivos de trabajo:**
- `Core/Inc/functions_ge2.h`: prototipos y datos de prueba (provisto en Clase5)
- `Core/Src/C_functions.c`: funciones en C
- `Core/Src/ASM_functions.S`: funciones en Assembly
- `Core/Src/main.c`: llamadas de prueba y medición de ciclos con `DWT->CYCCNT` (bloque `USER CODE BEGIN 2`)

**Ejercicios:**
- Ej. 1 `GPS_unpack`: extrae latitud y longitud (enteros signados de 12 bits) de cada muestra.
- Ej. 2 `max`: devuelve el índice del valor máximo (0 si no hay muestras).
- Ej. 3 `invertir`: invierte el orden de un buffer de 16 bits sobre el mismo vector.
- Ej. 4 `downSample`: opcional, no implementado.

**Importar en CubeIDE:** File → Import → General → Existing Projects into Workspace → seleccionar la carpeta `GdE2`.
Para probar: Debug As → STM32 C/C++ Application, poner un breakpoint al final del bloque `USER CODE 2` y comparar en *Variables* los resultados y los ciclos de C y Assembly.
