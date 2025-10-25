// C++ code
#include <Servo.h>
#include <LiquidCrystal.h>
// Definición de los pines para cada entrada y salida
// TODO: Cambiar de datatype a #define cuando se tenga los
// componentes a mano
const int flameSensor0to60 = 6;
const int flameSensor60to120 = 1; 
const int tmpPin = A0;
const float baselineTemp = 24.0;
 
// Declaración de Servo
Servo extinguishServo;
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
int detectedFlames[2];

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  extinguishServo.attach(9);
  extinguishServo.write(0);
  Serial.begin(9600); 
  pinMode(flameSensor0to60, INPUT);
  pinMode(flameSensor60to120, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Mensaje inicial
  lcd.clear();
  lcd.print("Sistema iniciado");
  delay(2000);
  lcd.clear();

  analogWrite(10, 1024.0);
}

void loop()
{
  // Crear una simulación para cada bucle.
  //simulation();
  // Switch-case de estados
  switch (currentState){
    case SEARCHING_FLAMES:
      // Revisar si existe alguna llama, si existe pasar
      // al siguiente estado y avisar por lcd, de lo contrario
      // seguir revisando si existe una llama.
      if (isThereAFlame()){
        Serial.println("ALERTA Llama detectada");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ALERTA llama");
        lcd.setCursor(0, 1);
        lcd.print("detectada");
        currentState = CHECKING_TEMPERATURE;
      } else {
        Serial.println("El sistema no ha encontrado anomalías");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No se han");
        lcd.setCursor(0, 1);
        lcd.print("detectado llamas");
      }
      break;

    case CHECKING_TEMPERATURE:
      // Si hay alguna llama, revisar si es muy intensa y hacer 
      // sonar la alarma antes de pasar al siguiente estado.
      flameAlarm();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ALERTA llama");
      lcd.setCursor(0, 1);
      lcd.print("alta temperatura");
      currentState = EXTINGUISHING_FLAMES;
      break;

    case EXTINGUISHING_FLAMES:
      // Si hay alguna llama, proceder a intentar extinguir la llama
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Extinguiendo");
      lcd.setCursor(0, 1);
      lcd.print("llama...");
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
  Serial.println(digitalRead(flameSensor0to60));
  if (digitalRead(flameSensor0to60) == HIGH){
    Serial.println("Se detectó una llama con sensor 1");
    detectedFlames[0] = 1;
    foundFlame = 1;
  }
  if (digitalRead(flameSensor60to120) == HIGH){
    detectedFlames[1] = 1;
    Serial.println("Se detectó una llama con sensor 2");
    foundFlame = 1;
  }
  //if (digitalRead(flameSensor120to180) == HIGH){
  //  Serial.println("Se detectó una llama con sensor 3");
  //	detectedFlames[2] = 1;
  //  foundFlame = 1;
  //}
  
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
  int sensorVal = analogRead(tmpPin);
  float temp = (((sensorVal/1024.0) * 5.0) - .5) * 100;
  // Si la temperatura es mayor igual a x activar la alarma.
  Serial.println(temp);
  if (temp >= baselineTemp) {
    Serial.println("Alerta, llama intensa activar alarma de buzzer");
    for (int i = 0; i < 100; i++){
      tone(8, 600, 50);
      delay(100);
      tone(8, 300, 50);
    }
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
    rotateServo(0);
  }
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(1000);
  
  if (detectedFlames[1]){
    detectedFlames[1] = 0;
    Serial.println("Moviendo servo a 60°");
    Serial.println("Extinguiendo llama de 60° a 120°");
    rotateServo(90);
  }
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(1000);
  
  //if (detectedFlames[2]){
  //  detectedFlames[2] = 0;
  //  Serial.println("Moviendo servo a 120°");
  //  Serial.println("Extinguiendo llama de 120° a 180°");
  //  rotateServo(120);
  //}
  //TODO: eliminar delay, unicamente para probar funcionamiento de lógica
  delay(1000);
}

// Simula el input de distintos sensores para cambiar estados.
void simulation(){
  //flameSensor0to60 = random(0, 2);
  //flameSensor60to120 = random(0, 2);
  //flameSensor120to180 = random(0, 2);
  //tmpPin = random(100);
}

void rotateServo(int angle){
  extinguishServo.write(angle);
  delay(20);
  for (int i = 0; i < 90; i++){
    //Serial.println(angle);
    angle += 1;
    extinguishServo.write(angle);
    delay(50);
  }
  
  for (int i = 0; i < 90; i++){
    //Serial.println(angle);
    angle -= 1;
    extinguishServo.write(angle);
    delay(50);
  }

  // scroll 13 positions (string length) to the left
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // delay at the end of the full loop:
  delay(1000);
}
