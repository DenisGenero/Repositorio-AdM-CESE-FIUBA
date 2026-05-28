import numpy as np
import matplotlib.pyplot as plt
from uart_module import leer_uart
from generate_module import generar_archivos_header, generar_ppg

# Colocar la dirección donde están los archivos .h del proyecto actual. Colocar doble
# barra invertida para que no se interprete como un comando de escape. Ejemplo:
# path_senialRuido = "D:\\UBA\\CESE\\AdM\\Workspace\\MyProject\\Core\\Inc"
path_senialRuido = "D:\\UBA\\Dictado de clases\\AdM\\MaterialPractica\\Practica\\Clase05_V2\\Core\\Inc"
# path_senialRuido = ""
if not path_senialRuido:
    path_senialRuido = input("Ingrese la ruta donde están los archivos .h de su proyecto:\n")

# ************** Script para corroborar filtro de media móvil ************** #
t = np.arange(0, 1, 0.01)
signal_noise = np.zeros(len(t))
signal_clean = np.zeros(len(t))

fs = 500  # Frecuencia de muestreo
duracion = 2  # 2 segundos = 1000 muestras totales en el vector
t, ppg_clean_base = generar_ppg(fs, duracion, bpm=75)

# Escalar la señal PPG limpia
A_signal = 500
offset = 100
signal_clean = offset + ppg_clean_base * A_signal

# Ruido de línea real (50 Hz)
f_noise = 50  # 50 Hz
A_noise = 30   # Amplitud del ruido de alterna
ruido = A_noise * np.sin(2 * np.pi * f_noise * t)

# Señales finales discretizadas
signal_noise = np.int16(signal_clean + ruido)
signal_clean = np.int16(signal_clean)

# Discretizar la señal para obtener valores uint16_t
signal_noise = np.uint16(signal_noise)
signal_clean = np.uint16(signal_clean)

# Descomentar estas líneas si se quiere ver la señal generada:
plt.title("Señal en formato int16")
plt.plot(t, signal_noise)
plt.show()

# Se genera el archivo config.h con la longitud del vector, y se guarda en el workspace
generar_archivos_header(path_senialRuido, signal_noise)

# Se espera por el envío de datos del microcontrolador
leer_uart()

# Leer los archivos luego de que se generen:
from Signal_C import signalFilt_C
from Signal_asm import signalFilt_ASM

# Se grafican los datos obtenidos
fig, ax = plt.subplots(2, 1, figsize=(10, 10))
samples = t*fs
ax[0].set_ylabel("Código en C")
ax[0].plot(samples, signal_noise, color="blue")
ax[0].plot(samples, signalFilt_C, color="red")
ax[1].set_ylabel("Código en Assembly")
ax[1].plot(samples, signal_noise, color="blue")
ax[1].plot(samples, signalFilt_ASM, color="red")
ax[1].set_xlabel("Muestras")
plt.show()

# Comparativa entre la señal limpia y filtrada
samples = t*fs
plt.title("Señal limpia vs filtrada")
plt.plot(samples, signal_clean, color="green")
plt.plot(samples, signalFilt_ASM, color="red")
plt.legend(["Señal limpia", "señal filtrada"])
plt.show()
