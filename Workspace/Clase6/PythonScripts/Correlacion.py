import numpy as np
import matplotlib.pyplot as plt
from uart_module import leer_uart

# Colocar la dirección donde están los archivos .h del proyecto actual. Colocar doble
# barra invertida para que no se interprete como un comando de escape. Ejemplo:
# path_senoRuido = "D:\\UBA\\CESE\\AdM\\Workspace\\MyProject\\Core\\Inc"
path_functions = ""
if not path_functions:
    path_senoRuido = input("Ingrese la ruta donde están los archivos .h de su proyecto:\n")

# ************** Script para corroborar la correlación ************** #
# Vector de tiempo para graficar
time_step = 0.01
t = np.arange(0, 1, time_step)
fs = 1/time_step
vecX = np.zeros(len(t))
vecY = np.ones_like(vecX)
# Parámetros de la señal
X_Amp = 50

# Generación de seno con ruido
for i in range(0, len(t)):
    vecX[i] = X_Amp*np.exp(-t[i])

# Discretizar la señal para obtener valores uint16_t
vecX = np.int16(vecX)
vecY = np.int16(vecY)

# Se exportan archivos para incluirlos en el proyecto del CubeIDE
with open(path_functions + "\\config.h", "w") as f:
    f.write("#ifndef CONFIG_H\n#define CONFIG_H\n\n")
    f.write(f"#define VEC_SIZE    {len(vecX)}\n\n")
    f.write("#endif\n")

# Se genera las señales para efectuar la correlación (correlation.h)
with open(path_functions + "\\correlation.h", "w") as f:
    f.write("#ifndef CORRELATION_H\n#define CORRELATION_H\n\n")
    f.write("#include <stdint.h>\n")
    f.write('#include "config.h"\n\n')
    f.write("int16_t vecX[VEC_SIZE] = {\n")
    f.write(", ".join(map(str, vecX)))
    f.write("\n};\n")
    f.write("int16_t vecY[VEC_SIZE] = {\n")
    f.write(", ".join(map(str, vecY)))
    f.write("\n};\n\n#endif\n")

# Vector resultado procesado en Python
vecCorrPy = np.correlate(vecX, vecY, mode='full')

print("Archivo config.h y correlation.h generados correctamente")

# Se espera por el envío de datos del microcontrolador
leer_uart()

from Correlation_C import vecCorr_C
from Correlation_ASM import vecCorr_ASM
from Correlation_DSP import vecCorr_DSP

# Se gráfican los vectores de entrada y el resultado
fig, ax = plt.subplots(2, 1, figsize=(10, 10))
samples = t*fs
ax[0].set_ylabel("Señales")
ax[1].set_xlabel("Muestras")
ax[0].plot(samples, vecX, color="red", marker=".")
ax[0].plot(samples, vecY, color="blue")
ax[0].legend(["vecX", "vecY"])
ax[1].plot(samples, vecCorrPy[(len(t)-1):(2*len(t)-1)], color="green", marker=".")
ax[1].plot(samples, vecCorr_C, color="red")
ax[1].plot(samples, vecCorr_DSP, color="blue", marker=".")
ax[1].plot(samples, vecCorr_ASM, color="violet", marker=".")
ax[1].legend(["vecCorrPy", "vecCorr_C", "vecCorr_ASM", "vecCorr_DSP"])
plt.show()

# Descomentar si se quieren ver los datos por consola
# print("vecCorrPy:")
# print(*vecCorrPy[len(t)-1:(2*len(t)-1)], sep=',')
# print("vecCorr_C:")
# print(*vecCorr_C, sep=',')
# print("vecCorr_ASM:")
# print(*vecCorr_ASM, sep=',')
# print("vecCorr_DSP:")
# print(*vecCorr_DSP, sep=',')