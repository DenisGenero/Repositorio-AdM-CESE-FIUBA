## Códigos desarrollados en la Clase 7  

***

En esta sesión se realiza una optimización más profunda sobre las funciones base desarrolladas en la Clase 6, buscando exprimir al máximo las capacidades de procesamiento del microcontrolador mediante técnicas avanzadas y un análisis riguroso de ciclos de reloj.

**Funciones implementadas:**  
- corr: función base en C para la correlación cruzada.  
- corr_Intrinsic: versión en C utilizando funciones intrínsecas de la CMSIS.  
- corr_ASM: implementación directa en Assembly.  
- corr_DSP: versión optimizada en Assembly explotando el set de instrucciones DSP del núcleo ARM Cortex-M.  

**Script de Python:** Para corroborar el funcionamiento y graficar los resultados de esta clase, **se utiliza exactamente el mismo script complementario de la Clase 6**. No se incluye una copia local en esta carpeta para evitar redundancia de archivos.

Podrá encontrar el script `Correlacion.py` y su archivo de dependencias directamente en el path de la **Clase 6**.

**Para hacer funcionar el Script (comandos bash):** *(Recuerde pararse en la carpeta de la Clase 6 para ejecutar los siguientes comandos)*:
- Ejecutar una sola vez:  
    - Crear un entorno virtual: `python -m venv .venv`  
    - Instalar dependencias: `pip install -r requirements.txt`  
- Cada vez que se quiera ejecutar el Script:  
    - Activar el entorno virtual:  
        - En Windows: `source .venv/Scripts/activate`  
        - En Linux: `source ./.venv/bin/activate`  
- Para ejecutar el Script: `python Correlacion.py`