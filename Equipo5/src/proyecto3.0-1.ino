#include <LiquidCrystal.h>

LiquidCrystal lcd(3, 4, 12, 13, 7, 8);

const int pinPower = 2;  
const int pinBuzzer = 11;

const int pinMotor = 10;  
const int pinGreen = 9;
const int pinBlue = 6;
const int pinRed = 5;
const int pinPoten = A1;
const int pinSensor = A0;
const int colors[5][3] = {{0, 14, 255}, {0, 255, 40}, {0,0,0}, {5, 225, 1}, {255, 25, 0}};

int motorSpeed = 0;
int targetTemp = 22;
int tressholds[5] = {targetTemp - 20, targetTemp - 10, targetTemp, targetTemp + 10, targetTemp + 20};
int alarm[5] = {1046, 523, 0, 220, 110};
int previousTemp = targetTemp;
//int counter = 2;
bool buzzerStatus = 0;
unsigned int timePass = 0;
int c = 0;

volatile bool status = false;
volatile unsigned long lastInterrupt = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinPower, INPUT_PULLUP);
  pinMode(pinMotor, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(pinRed, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(pinPower), power, CHANGE);

}

void loop() {
  c = 0;
  
  int valueInput = map(analogRead(pinPoten), 0, 1023, -15, 100); //Cambia el valor de la temperatura objetivo
  if (valueInput != targetTemp) {
    targetTemp = valueInput;
    for (int i = 0; i < 5; i++) {
      tressholds[i] = targetTemp - 20 + (i * 10);
    }
    Serial.print("Start"); 
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("targetTemp: ");
    lcd.print(targetTemp);
  }

  if (!status) {
    return; // no sigue si está apagado
  }
  
  int currentTemp = map(analogRead(pinSensor), 0, 1023, 125, -55) ; 

  /*Control del motor según temperatura
  if (currentTemp > 25) {
    motorSpeed = map(currentTemp, 26, 100, 100, 255);
    motorSpeed = constrain(motorSpeed, 100, 255);
  } else {
    motorSpeed = 0; // Motor apagado
  }
  

  if (currentTemp > 25) {
    // Rojo para temperaturas mayores a 25°C
    red = 255;
    green = 0;
    blue = 0;
  } else if (currentTemp < 15) {
    // Azul para temperaturas menores a 15°C
    red = 0;
    green = 0;
    blue = 255;
  } else {
    // Verde para temperaturas entre 15°C y 25°C
    red = 0;
    green = 255;
    blue = 0;
  }*/
  
  for(int i = 0; i < 3; i++){ //Bug
    if(tressholds[i] > currentTemp){
      c=i;
      break;
    }
    if(tressholds[4-i] <= currentTemp){
      c=4-i;
      break;
    }
  } 

  /*for(int i = 0; i < 3 && currentTemp >= tressholds[0]; i++){ //Bug
    c++;
    if(currentTemp >= tressholds[i] && currentTemp <= tressholds[i+1]){
      c = i+1;
      break;
    }
  }
  if(currentTemp >= tressholds[1] && currentTemp <= tressholds[3]){
    c = 2;
  } else ()*/

  
  
  /*int delta = 0;
  if (tressholds[c] > previousTemp) delta = 1;
  else if (tressholds[c] < previousTemp) delta = -1;*/
  

  //counter = constrain(counter, 0, 4);
  if(previousTemp != c){
    analogWrite(pinRed, colors[c][0]);
    analogWrite(pinGreen, colors[c][1]);
    analogWrite(pinBlue, colors[c][2]);
  }

  Serial.print("c: ");
  Serial.println(c);

  Serial.print("tresshold: ");
  Serial.println(tressholds[c]);

  if(c == 3){
    if(timePass >= 2000){
      buzzerControl(alarm[c]);
      timePass = -millis();
    }
    motorSpeed = 25;
  } else if (c == 4){
    if(timePass >= 400){
      buzzerControl(alarm[c]);
      timePass = -millis();
    }
    motorSpeed = 50;
  } else {
    noTone(pinBuzzer);
    motorSpeed = 0;
  }

  analogWrite(pinMotor, motorSpeed);
    
  printLCDSerial(currentTemp, colors[c]);
  
  
  timePass = timePass + millis();

  previousTemp = c;
}

void buzzerControl(int note){
  if(buzzerStatus){
    tone(pinBuzzer, note);
  } else{
    noTone(pinBuzzer);
  }
  buzzerStatus = !buzzerStatus;
}

void printLCDSerial(int currentTemp, int color[3]){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(currentTemp);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Motor: ");
  lcd.print(motorSpeed);
  
  Serial.print("target Temp: "); Serial.println(targetTemp);
  Serial.print("Temp: "); Serial.println(currentTemp);
  Serial.print("Motor: "); Serial.println(motorSpeed);
  Serial.print("Buzzer: "); Serial.println(alarm[c]);
  Serial.print("LED: R="); Serial.print(color[0]);
  Serial.print(" G="); Serial.print(color[1]);
  Serial.print(" B="); Serial.println(color[2]);
  Serial.print(" TimePass :"); Serial.println(timePass);
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
    delay(200);
  }

  /*void phaseCorrectPWM(){
    pinMode(3, OUTPUT);
    pinMode(11, OUTPUT);
    TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM20);
    TCCR2B = _BV(WGM22) | _BV(CS22);
    OCR2A = 180;
    OCR2B = 50;
  }*/
}
