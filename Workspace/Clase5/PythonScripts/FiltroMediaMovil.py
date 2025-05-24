import numpy as np
import matplotlib.pyplot as plt
from uart_module import leer_uart

# Colocar la dirección donde están los archivos .h del proyecto actual. Colocar doble
# barra invertida para que no se interprete como un comando de escape. Ejemplo:
# path_senoRuido = "D:\\UBA\\CESE\\AdM\\Workspace\\MyProject\\Core\\Inc"
path_senoRuido = ""

# Por si se olvida cargar la ruta al ejecutar el script
if not path_senoRuido:
    path_senoRuido = input("Ingrese la ruta donde están los archivos .h de su proyecto:\n")

# Definición del vector temporal para la señal
t = np.arange(0, 1, 0.01)
signal_noise = np.zeros(len(t))
signal_clean = np.zeros(len(t))
# Frecuencia de muestreo
fs = 100  # Hz
# Parámetros de la señal
A_signal = 100  # Amplitud
f_signal = 1  # Hz
# Parámetros del ruido
f_noise = 20  # Hz
A_noise = 0.1

# Generación de seno con ruido
for i in range(0, len(t)):
    # Se añade offset de 2*A_signal para que los valores sean mayores o iguales a cero
    signal_clean[i] = 2*A_signal + A_signal*np.sin(2*np.pi*t[i])
    signal_noise[i] = (2*A_signal + 2*A_noise) + A_signal*(np.sin(2*np.pi*t[i]) + A_noise*np.sin(2*f_noise*np.pi*t[i]))

# Discretizar la señal para obtener valores uint16_t
signal_noise = np.uint16(signal_noise)
signal_clean = np.uint16(signal_clean)

# Descomentar estas líneas si se quiere ver la señal generada:
# plt.title("Señal en formato uint16")
# plt.plot(t, signal_noise)
# plt.show()

# Se genera el archivo config.h con la longitud del vector, y se guarda en el workspace
with open(path_senoRuido + "\\config.h", "w") as f:
    f.write("#ifndef CONFIG_H\n#define CONFIG_H\n\n")
    f.write(f"#define VEC_SIZE    {len(signal_noise)}\n\n")
    f.write("#endif\n")

# Se genera la señal con ruido y se guarda en el workspace (senoRuido.h)
with open(path_senoRuido + "\\senoRuido.h", "w") as f:
    f.write("#ifndef SENO_RUIDO_H\n#define SENO_RUIDO_H\n\n")
    f.write("#include <stdint.h>\n")
    f.write('#include "config.h"\n\n')
    f.write("uint16_t seno_Ruido[VEC_SIZE] = {\n")
    f.write(", ".join(map(str, signal_noise)))
    f.write("\n};\n\n#endif\n")

print("Archivo senoRuido.h y config.h generados correctamente.")

# Se espera por el envío de datos del microcontrolador
leer_uart()

# Leer los archivos luego de que se generen:
from Signal_C import senoFilt_C
from Signal_asm import senoFilt_ASM

# Se grafican los datos obtenidos
fig, ax = plt.subplots(2, 1, figsize=(10, 10))
samples = t*fs
ax[0].set_ylabel("Código en C")
ax[0].plot(samples, signal_noise, color="blue")
ax[0].plot(samples, senoFilt_C, color="red")
ax[1].set_ylabel("Código en Assembly")
ax[1].plot(samples, signal_noise, color="blue")
ax[1].plot(samples, senoFilt_ASM, color="red")
ax[1].set_xlabel("Muestras")
plt.show()

# Comparativa entre la señal limpia y filtrada
samples = t*fs
plt.title("Señal limpia vs filtrada")
plt.plot(samples, signal_clean, color="green")
plt.plot(samples, senoFilt_ASM, color="orange")
plt.legend(["Señal limpia", "señal filtrada"])
plt.show()
