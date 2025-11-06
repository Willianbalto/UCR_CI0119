
#include <LiquidCrystal.h>

LiquidCrystal lcd(3, 4, 12, 13, 7, 8);

const int pinPower = 2;  
const int pinBuzzer = 11;
const int pinMotor = 10;  
const int pinGreen = 9;
const int pinBlue = 6;
const int pinRed = 5;
const int pinPoten = A1; //Potenciometro del cambio de temperatura objetivo
const int pinSensor = A0;
const int colors[5][3] = {{0, 14, 255}, {0, 255, 40}, {0,0,0}, {5, 225, 1}, {255, 25, 0}}; //Colores del LED RGB

int motorSpeed = 0; //Usado para la velocidad del motor
int motorState = 0; //Usado para control del cambio gradual de la temperatura del motor
int targetTemp = 22; //Te,peratura Objetivo
int tressholds[5] = {targetTemp - 20, targetTemp - 10, targetTemp, targetTemp + 10, targetTemp + 20}; //Limites de temperatura en orden de muy frio, frio, objetivo, caliente, muy caliente 
int alarm[5] = {1046, 523, 0, 110, 220}; //Notas para el buzzer
int previousTemp = targetTemp; //Temperatura anterior medida
bool buzzerStatus = 0; //Si el buzzer encendido o no
unsigned int timePass = 0; //Tiempo desde el ultimo tono del buzzer
unsigned int lcdWait = 2000; //Control de la pantalla LCD
int c = 0; //Valor actual de la temperatura leida en terminos de los intervalos

volatile bool status = false; // Encender y Apagar
volatile unsigned long lastInterrupt = 0; //Ultimo Apagado

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinPower, INPUT_PULLUP);
  pinMode(pinMotor, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(pinRed, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(pinPower), power, CHANGE); //Encendido y apagado
}

void loop() {
  c = 0;
  
  int valueInput = map(analogRead(pinPoten), 0, 1023, -15, 100); //Cambia el valor de la temperatura objetivo

  if (valueInput != targetTemp) { //Cambios de intervalos de temperatura y pantalla lcd
    targetTemp = valueInput;
    for (int i = 0; i < 5; i++) {
      tressholds[i] = targetTemp - 20 + (i * 10);
    }
    Serial.println("Start "); 
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("targetTemp: ");
    lcd.print(targetTemp);
    lcdWait = millis();
    Serial.print(" LcdWait :"); Serial.println(lcdWait);
  }

  if (!status) {
    return; // no sigue si está apagado
  }
  
  int currentTemp = map(analogRead(pinSensor), 0, 1023, 125, -55) ; 
  
  for(int i = 0; i < 3; i++){  //Intervalo en el que se encuentra la temperatura
    if(tressholds[i] > currentTemp){
      c=i;
      break;
    }
    if(tressholds[4-i] <= currentTemp){
      c=4-i;
      break;
    }
  } 

  if(previousTemp != c){ //Cambios de LEDs
    analogWrite(pinRed, colors[c][0]);
    analogWrite(pinGreen, colors[c][1]);
    analogWrite(pinBlue, colors[c][2]);
    
  }

  //buzzer
  buzzerControl(alarm[c], c);
  timePass =  millis();

  Serial.print("c: ");
  Serial.println(c);

  Serial.print("tresshold: ");
  Serial.println(tressholds[c]);

  //motor
  if(c == 3){
    motorState = 1;
  } else if (c == 4){
    motorState = 2;
  } else {
    motorState = 3;
  }

  if(motorState){
    motorState = motorControll(motorState);
  }

  //Print a la pantalla LCD
  printLCDSerial(currentTemp, colors[c]);

  //Setear nueva tenmperatura anterior leida
  previousTemp = c;
}

int motorControl(int state){
  switch (state)
  {
  case  1:
    motorSpeed += 5 * (25 - motorSpeed/abs(25 - motorSpeed)); //suma o resta si esta por encima o debajo de 25
    analogWrite(pinMotor, motorSpeed);
    if(motorSpeed == 25){
      state = 0 //no mas cambios al motor
    }
    return state;
  
  case 2:
    motorSpeed += 5;
    analogWrite(pinMotor, motorSpeed);
    if(motorSpeed == 50){
      state = 0
    }
    return state;

  case 3: //apagar el motor gradualmente
    motorSpeed -= 5;
    if(motorSpeed == 0){
      state = 0
    }
    return state;
  }
}

void buzzerControl(int note, int interval){
  if(buzzerStatus){
    switch (interval)
    {
    case 1:
    case 3:
      if(millis() - timePass >= 200){ //tiempo entre sonidos para frio y caliente
        buzzerControl(alarm[c]);
        timePass =  millis();
      }
      break;
    case 0:
    case 4:
      if(millis() - timePass >= 50){ //tiempo entre sonidos para muy frio y muy caliente
        buzzerControl(alarm[c]);
        timePass =  millis();
      }
    break;
    }
  } else{
    noTone(pinBuzzer);
  }
  buzzerStatus = !buzzerStatus;
}

void printLCDSerial(int currentTemp, const int color[3]){
  //Prints para control y debug
  Serial.print("target Temp: "); Serial.println(targetTemp);
  Serial.print("Temp: "); Serial.println(currentTemp);
  Serial.print("Motor: "); Serial.println(motorSpeed);
  Serial.print("Buzzer: "); Serial.println(alarm[c]);
  Serial.print("LED: R="); Serial.print(color[0]);
  Serial.print(" G="); Serial.print(color[1]);
  Serial.print(" B="); Serial.println(color[2]);
  Serial.print(" TimePass :"); Serial.println(timePass);

  //cuanto tiempo llevamos esperando 
  unsigned int wait = millis() - lcdWait;
  //LCD si es necesario cambiar toda la pantalla
  if(wait > 2000){
    Serial.print(" Update :"); Serial.println(wait);
    if (lcdWait == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
    } else{
      lcd.setCursor(7, 0);
      if(currentTemp >= 10){
        lcd.print(0);
      }
    }
    
    //LCD general
    lcd.print(currentTemp);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Motor: ");
    lcd.print(motorSpeed);
    lcdWait = 0;
  }
}

void power() {
  unsigned long now = millis();
  if (now - lastInterrupt > 300) { // antirrebote
    status = !status;
    lastInterrupt = now;

    //Refactorizacion para que no lo haga en cada loop
    analogWrite(pinRed, 0);
    analogWrite(pinGreen, 0);
    analogWrite(pinBlue, 0);
    analogWrite(pinMotor, 0);
    noTone(pinBuzzer);

    lcd.setCursor(0, 0);
    lcd.print("APAGADO        ");
  }
}