## Códigos desarrollados en la Clase 6  

***

**Funciones implementadas:**  
- corr: función en C que realiza la relación cruzada entre 2 vectores de igual longitud.  
- corr_Intrinsic: función similar a la anterior pero que ilustra el uso de una Intrinsic Function de la CMSIS para instrucciones de DSP.  
- corr_ASM: la función anterior implementada en Assembly.  
- corr_DSP: la función anterior pero optimizada con instrucciones DSP.  

**Script de Python:**  
Archivos que sirven de complemento para corroborar el correcto funcionamiento de las funciones implementadas en C, Assembly Y Assembly con instrucciones para DSP.  
En primer lugar se debe ejecutar el Script. Esto generará los archivos config.h y correlation.h en el path que se indique (si se deja vacío, se pedirá que se ingrese una dirección 
por consola). Luego, el Script quedará a la espera de datos provenientes de la consola uart. En este momento se debe compilar y ejecutar el firmware, el cuál efectuará la correlación
 y enviará los datos procesados al Script para que se grafiquen.  
