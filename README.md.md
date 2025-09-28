# Proyecto Etapa 2 - Documentación

## 1. Diseño de la máquina de estados

### Objetivo
El sistema debe encender/apagar una luz con aplausos detectados en el sensor 1 y un ventilador con aplausos detectados en el sensor 2.  
El control se realiza seleccionando un canal activo (canal 1 = luz, canal 2 = ventilador).  
Las entradas y salidas se simulan con el Monitor Serial de Arduino IDE.

### Estados definidos
- **ESTADO_INICIAL**: luz y ventilador apagados.
- **ESTADO_LUZ_ENCENDIDA**: solo la luz encendida.
- **ESTADO_VENT_ENCENDIDO**: solo el ventilador encendido.
- **ESTADO_AMBOS_ENCENDIDOS**: luz y ventilador encendidos.

### Eventos (entradas)
- `aplauso`: alterna Encendido/Apagado en el dispositivo del canal activo.
- `canal1`: selecciona directamente el canal de la luz.
- `canal2`: selecciona directamente el canal del ventilador.
- `b`: cambia de canal (simula un botón físico).
- `estado`: imprime el estado actual.

### Acciones (salidas)
- `Serial.println("LUZ encendida.")`
- `Serial.println("LUZ apagada.")`
- `Serial.println("VENTILADOR encendido.")`
- `Serial.println("VENTILADOR apagado.")`
- `Serial.println("Estado actual: ...")`

### Tabla de transiciones

| Estado actual   | Entrada        | Acción                        | Estado siguiente   |
|-----------------|----------------|-------------------------------|--------------------|
| INICIAL         | aplauso canal1 | LUZ encendida                 | LUZ_ENCENDIDA             |
| LUZ_ENCENDIDA          | aplauso canal1 | LUZ apagada                   | INICIAL            |
| INICIAL         | aplauso canal2 | VENT encendido                | VENT_ENCENDIDO            |
| VENT_ENCENDIDO         | aplauso canal2 | VENT apagado                  | INICIAL            |
| LUZ_ENCENDIDA          | aplauso canal2 | VENT encendido                | AMBOS_ENCENDIDOS           |
| VENT_ENCENDIDO         | aplauso canal1 | LUZ encendida                 | AMBOS_ENCENDIDOS           |
| AMBOS_ENCENDIDOS        | aplauso canal1 | LUZ apagada                   | VENT_ENCENDIDO            |
| AMBOS_ENCENDIDOS        | aplauso canal2 | VENT apagado                  | LUZ_ENCENDIDO             |

---

## 2. Implementación en Arduino IDE (C++)

El código se implementa en un archivo `proyecto_et2.ino`.  
Está organizado en funciones que cumplen cada responsabilidad:

- **ElegirCanal()**: permite seleccionar el canal manualmente (`canal1` o `canal2`).
- **CambiarCanal()**: cambia entre canal 1 (LUZ) y canal 2 (VENTILADOR).
- **Luces()**: enciende/apaga la luz simulando un aplauso.
- **Ventilador()**: enciende/apaga el ventilador simulando un aplauso.
- **ImprimirEstado()**: muestra en Serial el estado actual del sistema.
- **EstadoL() y EstadoV()**: devuelven `true/false` según si la luz o el ventilador están encendidos.

### Simulación de entradas
Se hace desde el **Monitor Serial** del Arduino IDE:  
- `aplauso`  simula un aplauso en el canal activo.  
- `canal1` / `canal2`  selecciona el canal activo.  
- `b`  alterna el canal.  
- `estado`  imprime estados.  

### Simulación de salidas
En vez de usar hardware, el programa usa `Serial.println()` para mostrar qué pasaría:  
- `"LUZ encendida."`, `"VENTILADOR apagado."`, etc.  

---

## 3. Casos de prueba

| Nº | Estado inicial | Entrada             | Resultado esperado                     |
|----|----------------|---------------------|----------------------------------------|
| 1  | Todo apagado   | `aplauso` (canal1)  | LUZ encendida                          |
| 2  | LUZ encendida  | `aplauso` (canal1)  | LUZ apagada                            |
| 3  | Todo apagado   | `aplauso` (canal2)  | VENTILADOR encendido                   |
| 4  | VENT encendido | `aplauso` (canal2)  | VENTILADOR apagado                     |
| 5  | Luz ON         | `b`  `aplauso`     | VENTILADOR encendido (luz sigue ON)    |
| 6  | Ambos ON       | `aplauso` (canal1)  | LUZ apagada, VENTILADOR sigue ON       |
| 7  | Ambos ON       | `aplauso` (canal2)  | VENTILADOR apagado, LUZ sigue ON       |
| 8  | Todo apagado   | `estado`            | Muestra `LUZ=OFF VENT=OFF`             |

---

## 4. Conclusión

- Se diseñó e implementó una máquina de estados sencilla pero completa para controlar dos dispositivos con aplausos.  
- Las pruebas se realizan únicamente con el Monitor Serial.  
- El diseño modular (funciones separadas) facilita modificar o ampliar la lógica en futuras etapas del proyecto.
