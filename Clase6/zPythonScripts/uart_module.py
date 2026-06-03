import serial
import struct


# Función para guardar en un archivo los datos provenientes del micro
def guardar_archivo(nombre_archivo, nombre_vector, datos):
    with open(nombre_archivo, "w") as f:
        f.write("import numpy as np\n\n")
        f.write(f"{nombre_vector} = np.zeros({len(datos)})\n")
        f.write(f"{nombre_vector} = np.int32({nombre_vector})\n\n")
        for i, val in enumerate(datos):
            f.write(f"{nombre_vector}[{i}] = {val}\n")


def leer_uart():
    archivo_base = "Correlation_"
    vector_base = "vecCorr_"
    com_num = input("Ingresá el número del COM (ej: 5 para COM5): ").strip()
    puerto = f"COM{com_num}"

    try:
        ser = serial.Serial(puerto, 115200, timeout=10)
        print(f"Conectado a {puerto}. Esperando transmisión...\n")

        while True:
            # 1. Leer lenguaje ("C", "C_INTR", "ASM", "DSP")
            linea_lang = ser.readline().decode('utf-8').strip().upper()
            if not linea_lang: continue
            if linea_lang not in ("C", "C_INTR", "ASM", "DSP"): continue

            # 2. Leer línea de metadatos 
            meta_linea = ser.readline().decode('utf-8').strip()
            try:
                tam_bytes, bytes_totales_a_leer = map(int, meta_linea.split(','))
            except (ValueError, IndexError):
                print(f"[WARN] Error al procesar metadatos basura: '{meta_linea}'. Reintentando sincronizar...")
                continue # Si vino basura, volvemos al lazo principal a buscar el próximo encabezado

            # Calcular la cantidad real de muestras para
            cantidad_muestras = bytes_totales_a_leer // tam_bytes

            # 3. Leer el bloque binario
            buffer_datos = ser.read(bytes_totales_a_leer)
            if len(buffer_datos) < bytes_totales_a_leer:
                print("[ERROR] Transmisión binaria incompleta. Fin de captura.")
                break
            
            # 4. Desempaquetar dinámicamente usando cantidad_muestras
            char_formato = 'i' if tam_bytes == 4 else 'h'
            formato_unpack = f"<{cantidad_muestras}{char_formato}"
            datos_recibidos = list(struct.unpack(formato_unpack, buffer_datos))

            # 5. Leer línea final de control ("end" o vacío)
            linea_eot = ser.readline().decode('utf-8').strip().lower()

            # Normalizar nombres para los archivos .py
            lang_archivo = "C_Intr" if linea_lang == "C_INTR" else linea_lang.capitalize()
            if linea_lang == "DSP": lang_archivo = "DSP"

            nombre_archivo = f"{archivo_base}{lang_archivo}.py"
            nombre_vector = f"{vector_base}{lang_archivo}"

            guardar_archivo(nombre_archivo, nombre_vector, datos_recibidos)
            print(f"Archivo generado: {nombre_archivo}\n")

            if "end" in linea_eot:
                print("Transmisión finalizada con éxito!")
                break

    except Exception as e:
        print("Error en la lectura de la UART:", e)
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    leer_uart()

