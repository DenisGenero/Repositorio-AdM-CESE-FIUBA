from pathlib import Path
import numpy as np

def generar_archivos_header(path_destino, senial):
    """
    Genera los archivos config.h y senialRuido.h en la ruta especificada.
    """
    base_path = Path(path_destino)
    
    # 1. Generar config.h
    config_path = base_path / "config.h"
    with open(config_path, "w", encoding="utf-8") as f:
        f.write("#ifndef CONFIG_H\n")
        f.write("#define CONFIG_H\n\n")
        f.write(f"#define VEC_SIZE    {len(senial)}\n\n")

        f.write("// Vector a exportar\n")
        f.write("typedef enum {\n")
        f.write("    EXPORT_TYPE_C,\n")
        f.write("    EXPORT_TYPE_ASM,\n")
        f.write("} ExportType;\n\n")

        f.write("// Señal para control de finalización de envío\n")
        f.write("typedef enum {\n")
        f.write("    EXPORT_MODE_CONTINUE,\n")
        f.write("    EXPORT_MODE_CLOSE_FILE,\n")
        f.write("} ExportMode;\n\n")
        f.write("#endif\n")

    # 2. Generar senialRuido.h
    senial_path = base_path / "signalRuido.h"
    with open(senial_path, "w") as f:
        f.write("#ifndef SENIAL_RUIDO_H\n#define SENIAL_RUIDO_H\n\n")
        f.write("#include <stdint.h>\n")
        f.write('#include "config.h"\n\n')
        f.write("int16_t senial_Ruido[VEC_SIZE] = {\n    ")
        f.write(", ".join(map(str, senial)))
        f.write("\n};\n\n#endif\n")
        
    print(f"Archivos config.h y senialRuido.h generados correctamente en: {base_path}")


def generar_ppg (fs, duracion, bpm=60):
    """
    Genera una señal PPG sintética limpia basada en funciones gaussianas.
    """
    t = np.arange(0, duracion, 1/fs)
    ppg_puro = np.zeros(len(t))
    
    # Período de un latido en segundos
    periodo_latido = 60.0 / bpm
    
    # Parámetros de las ondas gaussianas para simular el pulso PPG
    # Pico Sistólico
    a1, b1, c1 = 1.0, 0.2, 0.05
    # Pico Diastólico (onda dicroica)
    a2, b2, c2 = 0.35, 0.4, 0.08
    
    for i, ti in enumerate(t):
        # Fase dentro del ciclo cardíaco actual (0 a periodo_latido)
        fase = ti % periodo_latido
        
        # Superposición de las dos curvas gaussianas
        sistole = a1 * np.exp(-((fase - b1) / c1) ** 2)
        diastole = a2 * np.exp(-((fase - b2) / c2) ** 2)
        
        ppg_puro[i] = sistole + diastole
        
    return t, ppg_puro