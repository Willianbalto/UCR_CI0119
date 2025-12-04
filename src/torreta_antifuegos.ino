// C++ code
#include <Servo.h>
#include <LiquidCrystal.h>
// Definición de los pines para cada entrada y salida
const int flameSensor0to60 = 6;
const int flameSensor60to120 = 13; 
const int tmpPin = A0;
float baselineTemp = 0;
 
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

// inicializar la libreria necesaria para controlar la pantalla lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  extinguishServo.attach(9);
  extinguishServo.write(0);
  Serial.begin(9600); 
  pinMode(flameSensor0to60, INPUT);
  pinMode(flameSensor60to120, INPUT);

  // configurar las filas y columnas de la pantalla
  lcd.begin(16, 2);
  // Mensaje inicial
  lcd.clear();
  lcd.print("Sistema iniciado");
  delay(2000);
  lcd.clear();

  analogWrite(10, 1024.0);

    // Referencia AVcc (5V)
  ADMUX = (1 << REFS0);
  // Reescalado de la divsión a 32, no es necesario precisión completa
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0);
  // Descartar primera lectura para evitar delay de estabilización
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC)){}
  volatile int rejectedRead = ADCL | (ADCH << 8);
  // Autocalibrador para el sensor de temperatura, durante 5 segundos captura la 
  // temperatura más alta y le suma 2 como margen.
  while (millis() < 5000){
      // Comenzar lectura
    ADCSRA |= (1 << ADSC);
    // Esperar mientras el bit de terminar lectura se apaga
    while (ADCSRA & (1 << ADSC)){}
    // Guardar el voltaje a digital
    int val = ADCL | (ADCH << 8);
    float temp = (((val/1024.0) * 5.0) - .5) * 100;
    if (temp > baselineTemp){
     baselineTemp = temp; 
    }
  }
  baselineTemp += 2;

}

void loop()
{
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
  
}

// Metodo que revisa si hay alguna llama en los 60° de rango de cada sensor
// "enciende" una celda de un arreglo correspondiente al sensor que detectó
// la llama. Retorna 1 si alguna llama se detectó, de lo contrario 0.
int isThereAFlame(){
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
  int inicio = micros();
  // Comenzar lectura
  ADCSRA |= (1 << ADSC);
  // Esperar mientras el bit de terminar lectura se apaga
  while (ADCSRA & (1 << ADSC)){}
  // Guardar el voltaje a digital
  int val = ADCL | (ADCH << 8);
  int Final = micros();

 Serial.print("microsegundos usados en la conversión: " );
 Serial.println(Final-inicio);
  float temp = (((val/1024.0) * 5.0) - .5) * 100;
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
  
  delay(2000);
}

// Metodo encargado de mover el servo para extinguir cada una de las llamas presentes
// detectadas en un estado anterior
void extinguishFlames(){
  
  // Si alguna llama ha sido detectado por algún sensor, mover el
  // servo a la posición del sensor que detectó de la llama y extinguir
  // devolviendo la celda del array correspondiente al sensor que notó la llama a 0.
  if (detectedFlames[0]){
    detectedFlames[0] = 0;
    Serial.println("Moviendo servo a 0°");
    Serial.println("Extinguiendo llama de 0° a 60°");
    rotateServo(0);
  }

  delay(1000);
  
  if (detectedFlames[1]){
    detectedFlames[1] = 0;
    Serial.println("Moviendo servo a 60°");
    Serial.println("Extinguiendo llama de 60° a 120°");
    rotateServo(90);
  }

  delay(2000);

}

// Simula el input de distintos sensores para cambiar estados.
void simulation(){
  //flameSensor0to60 = random(0, 2);
  //flameSensor60to120 = random(0, 2);
  //flameSensor120to180 = random(0, 2);
  //tmpPin = random(100);
}

// Metodo encargado para rotar el brazo del motor servo 90 grados a la izquierda y a la derecha
// Recibe el angulo en el cual inicia el brazo del motor
void rotateServo(int angle){
  extinguishServo.write(angle);
  delay(20);
  for (int i = 0; i < 90; i++){
    angle += 1;
    extinguishServo.write(angle);
    delay(50);
  }
  
  for (int i = 0; i < 90; i++){
    angle -= 1;
    extinguishServo.write(angle);
    delay(50);
  }
  delay(1000);
}

