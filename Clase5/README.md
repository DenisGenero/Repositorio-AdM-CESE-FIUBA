## Códigos desarrollados en la Clase 5

***

**Funciones implementadas:**  
- MediaMovil: función que realiza un filtrado de media móvil sobre un vector de muestras con un ancho fijo de ventana.  
- asm_MediaMovil: implementar la función anterior en Assembly. Se introducen las instrucciones condicionales y se ve cómo utilizarlas.  
Se introduce una manera de hacer mediciones de ciclos y se comparan los ciclos que cada función toma en ejecutarse.  
Para saber cómo realizar el conteo de ciclos, se sugiere consultar el siguiente instructivo:
https://docs.google.com/document/d/1tLKs_rYYyTVa9waYE1NFW5jg_mPJ3Ra1IOnu-qKCysw/edit#heading=h.i5grnkbqdp4n  

**Script de Python:**  
Archivos que sirven de complemento para corroborar el correcto funcionamiento del filtro implementado en C y Assembly.  
En primer lugar se debe ejecutar el Script. Esto generará el archivo senoRuido.h en el path que se indique (si se deja 
vacío, se pedirá que se ingrese una dirección por consola). Luego, el Script quedará a la espera de datos provenientes de la consola
uart. En este momento se debe compilar y ejecutar el firmware, el cuál efectuará los filtrados y enviará los datos procesados
al Script para que se grafiquen.  


**Para hacer funcionar el Script (comandos bash):**  
- Ejecutar una sola vez:  
    - Crear un entorno virtual: `python -m venv .venv`  
    - Instalar dependencias: `pip install -r requirements.txt`  
- Cada vez que se quiera ejecutar el Script:  
    - Activar el entorno virtual:  
        - En Windows: `source .venv/Scripts/activate`  
        - En Linux: `source ./.venv/bin/activate`  
- Para ejecutar el Script: `python FiltroMediaMovil.py`  