## GdE1 – Guía de Ejercicios 1 (NUCLEO-F446RE)

Proyecto STM32CubeIDE para probar la Guía de Ejercicios 1 (Introducción a Assembly) en la NUCLEO-F446RE.

**Archivos de trabajo:**
- `Core/Inc/functions.h`: prototipos
- `Core/Src/C_functions.c`: funciones en C
- `Core/Src/ASM_functions.S`: funciones en Assembly
- `Core/Src/main.c`: llamadas de prueba (bloque `USER CODE BEGIN 2`)

**Ejercicios:**
- Ej. 1 `productoEscalar32` y Ej. 3 `bitfield_clear`: solución del profesor (Clase4).
- Ej. 2 `productoEscalar12`: producto escalar de 16 bits con saturación a 12 bits.
- Ej. 4 `bitfield_toggle`: invierte un campo de bits.

**Importar en CubeIDE:** File → Import → General → Existing Projects into Workspace → seleccionar la carpeta `GdE1`.
Para probar: Debug As → STM32 C/C++ Application, poner un breakpoint al final del bloque `USER CODE 2` y comparar en *Variables* los resultados de C y Assembly.
