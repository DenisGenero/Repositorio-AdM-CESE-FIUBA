from pathlib import Path
import numpy as np

def inicializar_modulos(muestras):
    """
    Genera los archivos para guardar las señales provenientes del micro.
    """

    modulos = ["Correlation_C", "Correlation_C_Intr", "Correlation_ASM", "Correlation_DSP"]
    
    for modulo in modulos:
        file_path = Path(f"{modulo}.py")
        with open(file_path, "w", encoding="utf-8") as f:
            f.write("import numpy as np\n\n")
            f.write(f"vecCorr_{modulo.replace('Correlation_', '')} = np.zeros({muestras})\n")
            f.write(f"vecCorr_{modulo.replace('Correlation_', '')} = np.int32(vecCorr_{modulo.replace('Correlation_', '')})\n")

def generar_archivos_header(path_destino, senialX, senialY):
    """
    Genera los archivos config.h y senialRuido.h en la ruta especificada.
    """
    base_path = Path(path_destino)
    
    # 1. Generar config.h
    config_path = base_path / "config.h"
    with open(config_path, "w", encoding="utf-8") as f:
        f.write("#ifndef CONFIG_H\n")
        f.write("#define CONFIG_H\n\n")
        f.write(f"#define VEC_SIZE    {len(senialX)}\n\n")

        f.write("// Vector a exportar\n")
        f.write("typedef enum {\n")
        f.write("    EXPORT_TYPE_C,\n")
        f.write("    EXPORT_TYPE_C_INTR,\n")
        f.write("    EXPORT_TYPE_ASM,\n")
        f.write("    EXPORT_TYPE_ASM_DSP,\n")
        f.write("} ExportType;\n\n")

        f.write("// Señal para control de finalización de envío\n")
        f.write("typedef enum {\n")
        f.write("    EXPORT_MODE_CONTINUE,\n")
        f.write("    EXPORT_MODE_CLOSE_FILE,\n")
        f.write("} ExportMode;\n\n")
        f.write("#endif\n")

    # 2. Generar señales para correlación cruzada:
    senial_path = base_path / "signals_corr.h"
    with open(senial_path, "w") as f:
        f.write("#ifndef CORRELATION_H\n")
        f.write("#define CORRELATION_H\n\n")
        f.write("#include <stdint.h>\n")
        f.write('#include "config.h"\n\n')
        f.write("int16_t vecX[VEC_SIZE] __attribute__((aligned(4))) = {\n")
        f.write(", ".join(map(str, senialX)))
        f.write("\n};\n")
        f.write("int16_t vecY[VEC_SIZE] __attribute__((aligned(4))) = {\n")
        f.write(", ".join(map(str, senialY)))
        f.write("\n};\n\n#endif\n")
        
    print(f"Archivos config.h y signals_corr.h generados correctamente en: {base_path}")
