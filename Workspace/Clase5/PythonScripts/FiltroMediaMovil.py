import numpy as np
import matplotlib.pyplot as plt
from Signal_C import senoFilt_C
from Signal_asm import senoFilt_ASM

# ************** Script para corroborar filtro de media móvil ************** #
t = np.arange(0, 1, 0.01)
signal_noise = np.zeros(len(t))
signal_clean = np.zeros(len(t))
# Frecuencia de muestreo
fs = 100  # Hz
# Parámetros de la señal
A_signal = 100
f_signal = 1  # Hz
# Parámetros del ruido
f_noise = 20  # Hz
A_noise = 0.1

# Generación de seno con ruido
for i in range(0, len(t)):
    signal_clean[i] = 2*A_signal + A_signal*np.sin(2*np.pi*t[i])
    signal_noise[i] = (2*A_signal + 2*A_noise) + A_signal*(np.sin(2*np.pi*t[i]) + A_noise*np.sin(2*f_noise*np.pi*t[i]))

# Discretizar la señal para obtener valores uint16_t
signal_noise = np.uint16(signal_noise)
signal_clean = np.uint16(signal_clean)

# Copiar desde la consola los datos para llevar al CubeIDE
print(*signal_noise, sep=',')

plt.title("Señal en formato uint16")
plt.plot(t, signal_noise)
plt.show()

# Copiar los vectores del debugger del CubeIDE, y pegarlo en
# los respectivos archivos (Signal_asm y Signal_C). Luego descomentar para graficar.
#fig, ax = plt.subplots(2, 1, figsize=(10, 10))
#samples = t*fs
#ax[0].set_ylabel("Código en C")
#ax[0].plot(samples, signal_noise, color="blue")
#ax[0].plot(samples, senoFilt_C, color="red")
#ax[1].set_ylabel("Código en Assembly")
#ax[1].plot(samples, signal_noise, color="blue")
#ax[1].plot(samples, senoFilt_ASM, color="red")
#ax[1].set_xlabel("Muestras")
#plt.show()

# Comparativa entre la señal limpia y filtrada
#samples = t*fs
#plt.title("Señal limpia vs filtrada")
#plt.plot(samples, signal_clean, color="green")
#plt.plot(samples, senoFilt_ASM, color="violet")
#plt.legend(["Señal limpia", "señal filtrada"])
#plt.show()
