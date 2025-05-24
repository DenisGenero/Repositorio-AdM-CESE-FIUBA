import serial
import struct


# Función para guardar en un archivo los datos provenientes del micro
def guardar_archivo(nombre_archivo, nombre_vector, datos):
    with open(nombre_archivo, "w") as f:
        f.write("import numpy as np\n\n")
        f.write(f"{nombre_vector} = np.zeros({len(datos)})\n\n")
        for i, val in enumerate(datos):
            f.write(f"{nombre_vector}[{i}] = {val}\n")


def leer_uart():
    archivo = "Signal_"
    vector = "senoFilt_"
    com_num = input("Ingresá el número del COM (ej: 5 para COM5): ").strip()
    puerto = f"COM{com_num}"

    try:
        ser = serial.Serial(puerto, 115200, timeout=10)
        print(f"Conectado a {puerto}. Esperando datos...\n")

        while True:
            # Leer encabezado: 'C\n' o 'asm\n'
            encabezado = ser.readline().decode('utf-8').strip().upper()
            if encabezado not in ("C", "ASM"):
                print(f"Encabezado inválido o fin inesperado: '{encabezado}'")
                continue

            datos_recibidos = []

            while True:
                bytes_leidos = ser.read(2)
                if len(bytes_leidos) < 2:
                    print("Timeout o transmisión incompleta.")
                    break

                valor = struct.unpack('<H', bytes_leidos)[0]

                # Señal que indica final de una señal
                if valor == 0xFFFF:
                    break

                datos_recibidos.append(valor)

            # Si EOT = 'end', se finaliza la recepción de datos
            eot = ser.readline().decode('utf-8').strip().lower()

            nombre_archivo = f"{archivo}{encabezado}.py"
            nombre_vector = f"{vector}{encabezado}"
            guardar_archivo(nombre_archivo, nombre_vector, datos_recibidos)
            print(f"Datos guardados en {nombre_archivo}.")

            if eot == "end":
                print("Transmisión finalizada.")
                break

    except Exception as e:
        print("Error al abrir el puerto o durante la lectura:", e)


if __name__ == "__main__":
    leer_uart()

