import numpy as np
import matplotlib.pyplot as plt
from uart_module import leer_uart
from generate_module import generar_archivos_header, inicializar_modulos

# Configuración de ruta para archivos .h
path_seniales = "D:\\UBA\\Dictado de clases\\AdM\\MaterialPractica\\Practica\\Clase06\\Core\\Inc"
# path_seniales = "D:\\UBA\\Dictado de clases\\AdM\\MaterialPractica\\Practica\\Clase07\\Core\\Inc"
if not path_seniales:
    path_seniales = input("Ingrese la ruta donde están los archivos .h de su proyecto:\n")

# ************** Parámetros del Radar (360 muestras = 1 muestra por grado) ************** #
muestras = 360
grados = np.arange(0, muestras, 1)

# Crear los archivos para almacenar los vectores provenientes del micro
inicializar_modulos(muestras)

# 1. Definir la señal clave: Pulso triangular
ancho_pulso = 20
pulso_triangular = np.zeros(muestras)
for i in range(ancho_pulso):
    if i <= ancho_pulso // 2:
        pulso_triangular[i] = i * 10  # Rampa ascendente
    else:
        pulso_triangular[i] = (ancho_pulso - i) * 10  # Rampa descendente

# 2. Definir la señal devuelta (Puro ruido + pulso oculto por ejemplo en el grado 120)
posicion_oculta = 120
ruido = np.random.normal(0, 70, muestras)  # Ruido gaussiano con desvío estándar de 80

vecX = np.zeros(muestras)
# Insertamos la señal triangular oculta desplazada
vecX[posicion_oculta:posicion_oculta+ancho_pulso] = pulso_triangular[0:ancho_pulso]
vecX_con_ruido = vecX + ruido

# 3. Casteos para el micro (int16)
vecY_micro = np.int16(pulso_triangular)
vecX_micro = np.int16(vecX_con_ruido)

# Exportar archivos .h para el micro 
generar_archivos_header(path_seniales, vecX_micro, vecY_micro)

# Vector resultado procesado en Python (float64)
vecCorrPy_full = np.correlate(vecX_con_ruido, pulso_triangular, mode='full')
# Extraemos la ventana idéntica al micro (Segunda mitad de 'full')
vecCorrPy = vecCorrPy_full[muestras - 1 : 2 * muestras - 1]

# ========================================================================= #
# Se espera por el envío de datos del microcontrolador mediante UART
print("Esperando datos de la UART...")
leer_uart()
# ========================================================================= #

# Importación de los vectores recibidos del micro
try:
    from Correlation_C import vecCorr_C
    from Correlation_C_Intr import vecCorr_C_Intr
    from Correlation_ASM import vecCorr_Asm
    from Correlation_DSP import vecCorr_DSP
except ImportError as e:
    print(f"Error al importar los vectores de la UART: {e}")

# Graficación de las señales y resultados
fig, ax = plt.subplots(3, 1, figsize=(10, 9))

# Gráfico 1: Señal Recibida
ax[0].set_title("Señal Recibida por el Radar (Ruido + Objetivo Oculto)")
ax[0].set_ylabel("Amplitud")
ax[0].plot(grados, vecX_micro, color="gray", alpha=0.7, label="vecX (Input int16)")
ax[0].legend(loc="upper right")
ax[0].grid(True)

# Gráfico 2: Señal Clave
ax[1].set_title("Señal Clave del Radar (Patrón de Búsqueda)")
ax[1].set_ylabel("Amplitud")
ax[1].plot(grados, vecY_micro, color="blue", linewidth=2, label="vecY (Template int16)")
ax[1].legend(loc="upper right")
ax[1].grid(True)

# Gráfico 3: Resultados de la Correlación
ax[2].set_title("Resultado de la Correlación Cruzada (Detección)")
ax[2].set_xlabel("Ángulo (Grados °)")
ax[2].set_ylabel("Intensidad")
ax[2].plot(grados, vecCorrPy, color="green", label="Python (Referencia)", linewidth=2)
ax[2].plot(grados, vecCorr_C, color="red", linestyle="--", label="Micro: C")
ax[2].plot(grados, vecCorr_C_Intr, color="orange", linestyle=":", label="Micro: Intrinsics")
ax[2].plot(grados, vecCorr_Asm, color="violet", linestyle="-.", label="Micro: ASM")
ax[2].plot(grados, vecCorr_DSP, color="cyan", linestyle="-.", label="Micro: DSP")

# Encontrar el máximo en Python (x azul)
posicion_detectada_py = np.argmax(vecCorrPy)
max_valor_py = vecCorrPy[posicion_detectada_py]
ax[2].plot(posicion_detectada_py, max_valor_py, color="darkblue", marker="X", markersize=10, 
           linestyle="None", label=f"Max Python ({posicion_detectada_py}°)")

# Encontrar el máximo en el micro (punto rojo)
pico_micro = np.argmax(vecCorr_C)
max_valor_micro = vecCorr_C[pico_micro]
ax[2].plot(pico_micro, max_valor_micro, 'ro', markersize=8, label=f'Pico Detectado ({pico_micro}°)')

ax[2].legend(loc="upper right")
ax[2].grid(True)

plt.tight_layout()
plt.show()