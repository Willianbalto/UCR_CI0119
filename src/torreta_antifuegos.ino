// C++ code
// Definición de los pines para cada entrada y salida
// TODO: Cambiar de datatype a #define cuando se tenga los
// componentes a mano
 long flameSensor0to60 = 0;
 long flameSensor60to120 = 0;
 long flameSensor120to180 = 0; 
 long tmpPin = 0;
 
// Enum conteniendo los estados posibles de la máquina
enum States{
 SEARCHING_FLAMES,
 CHECKING_TEMPERATURE,
 EXTINGUISHING_FLAMES
};
// Estado Inicial de la máquina
States currentState = SEARCHING_FLAMES;
// Array que guarda cual de los 3 sensores detectó 
// una llama
 int detectedFlames[3];
void setup()
{
  Serial.begin(9600); 
}

void loop()
{
  // Crear una simulación para cada bucle.
  simulation();
  // Switch-case de estados
  switch (currentState){
    case SEARCHING_FLAMES:
      // Revisar si existe alguna llama, si existe pasar
      // al siguiente estado y avisar por lcd, de lo contrario
      // seguir revisando si existe una llama.
      if (isThereAFlame()){
        Serial.println("ALERTA Llama detectada");
        currentState = CHECKING_TEMPERATURE;
      } else {
        Serial.println("El sistema no ha encontrado anomalías");
      }
      
      break;
    case CHECKING_TEMPERATURE:
      // Si hay alguna llama, revisar si es muy intensa y hacer 
      // sonar la alarma antes de pasar al siguiente estado.
      flameAlarm();
      currentState = EXTINGUISHING_FLAMES;
      break;
    case EXTINGUISHING_FLAMES:
      // Si hay alguna llama, proceder a intentar extinguir la llama
      extinguishFlames();
      currentState = SEARCHING_FLAMES;
      break;
  }
  
  //TODO: Implementar las alarmas del buzzer y los mensajes por LCD
}

// Metodo que revisa si hay alguna llama en los 60° de rango de cada sensor
// "enciende" una celda de un arreglo correspondiente al sensor que detectó
// la llama. Retorna 1 si alguna llama se detectó, de lo contrario 0.
int isThereAFlame(){
  // TODO: Cambiar los parametros de los if a digitalRead 
  // cuando se conecten los sensores
  
  // Variable para revisar si al menos se ha encontrado una llama
  int foundFlame = 0;
  // Si es detectada una llama por alguno de los sensores, "encender" su
  // respectiva celda en el array.
  if (flameSensor0to60){
    Serial.println("Se detectó una llama con sensor 1");
  	detectedFlames[0] = 1;
    foundFlame = 1;
  }
  if (flameSensor60to120){
    detectedFlames[1] = 1;
    Serial.println("Se detectó una llama con sensor 2");
    foundFlame = 1;
  }
  if (flameSensor120to180){
    Serial.println("Se detectó una llama con sensor 3");
  	detectedFlames[2] = 1;
    foundFlame = 1;
  }
  
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(2000);
  
  // Si algún sensor encontró una llama retornar 1, de lo contrario 0
  if (foundFlame){
  	return 1;
  } else {
  	return 0;
  }
  
}

// Metodo que revisa la temperatura ambiente creada por la llama, si está por
// encima de un valor x activa una alarma de llama intensa.
void flameAlarm(){
  // TODO: Cambiar tempPin por el valor de temperatura y definir el rango
  // "llama peligrosa".
  
  // Si la temperatura es mayor igual a x activar la alarma.
  if (tmpPin >= 70) {
  	Serial.println("Alerta, llama intensa activar alarma de buzzer");
  }
  
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(2000);
}

// Metodo encargado de mover el servo para extinguir cada una de las llamas presentes
// detectadas en un estado anterior
void extinguishFlames(){
  // TODO: Programar el movimiento del servo, al llegar a la posición destino
  // girar 60° a la derecha a una velocidad eficiente. Propuesta: 60° en 10s
  
  // Si alguna llama ha sido detectado por algún sensor, mover el
  // servo a la posición del sensor que detectó de la llama y extinguir
  // devolviendo la celda del array correspondiente al sensor que notó la llama a 0.
  if (detectedFlames[0]){
    detectedFlames[0] = 0;
    Serial.println("Moviendo servo a 0°");
    Serial.println("Extinguiendo llama de 0° a 60°");
  }
  
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(1000);
  
  if (detectedFlames[1]){
    detectedFlames[1] = 0;
    Serial.println("Moviendo servo a 60°");
    Serial.println("Extinguiendo llama de 60° a 120°");
  }
  
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(1000);
  
  if (detectedFlames[2]){
    detectedFlames[2] = 0;
    Serial.println("Moviendo servo a 120°");
    Serial.println("Extinguiendo llama de 120° a 180°");
  }
  
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(1000);
  
}


// Simula el input de distintos sensores para cambiar estados.
void simulation(){
 flameSensor0to60 = random(0, 2);
 flameSensor60to120 = random(0, 2);
 flameSensor120to180 = random(0, 2);
 tmpPin = random(100);
}