import numpy as np
import matplotlib.pyplot as plt
from Correlation_C import vecCorr_C
from Correlation_ASM import vecCorr_ASM
from Correlation_DSP import vecCorr_DSP

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
vecX = np.uint16(vecX)
vecY = np.uint16(vecY)
print("vecX:")
print(*vecX, sep=',')
print("vecY:")
print(*vecY, sep=',')

# Vector resultado
vecCorrPy = np.correlate(vecX, vecY, mode='full')

# Descomentar las siguientes líneas luego de tener los datos del micro
# Se muestran los datos por consola
#print("vecCorrPy:")
#print(*vecCorrPy[len(t)-1:(2*len(t)-1)], sep=',')
#print("vecCorr_C:")
#print(*vecCorr_C, sep=',')
#print("vecCorr_ASM:")
#print(*vecCorr_ASM, sep=',')
#print("vecCorr_DSP:")
#print(*vecCorr_DSP, sep=',')

# Se gráfican los vectores de entrada y el resultado
#fig, ax = plt.subplots(2, 1, figsize=(10, 10))
#samples = t*fs
#ax[0].set_ylabel("Señales")
#ax[1].set_xlabel("Muestras")
#ax[0].plot(samples, vecX, color="red", marker=".")
#ax[0].plot(samples, vecY, color="blue")
#ax[0].legend(["vecX", "vecY"])
#ax[1].plot(samples, vecCorrPy[(len(t)-1):(2*len(t)-1)], color="green", marker=".")
#ax[1].plot(samples, vecCorr_C, color="red")
#ax[1].plot(samples, vecCorr_ASM, color="violet", marker=".")
#ax[1].plot(samples, vecCorr_DSP, color="blue", marker=".")
#ax[1].legend(["vecCorrPy", "vecCorr_C", "vecCorr_ASM", "vecCorr_DSP"])
#plt.show()
