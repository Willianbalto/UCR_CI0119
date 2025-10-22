#include <LiquidCrystal.h>

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

const int pinPower = 2;  
const int pinBuzzer = 9;

const int pinMotor = 10;  // falta conectar
const int pinGreen = 11;
const int pinBlue = 12;
const int pinRed = 13;
const int pinPoten = A1;
const int pinSensor = A0;

int motorSpeed = 0;
int targetTemp = 22;
int tressholds[5];
int alarm[5] = {1046, 523, 0, 220, 110};
int previousTemp = 22;
int counter = 2;
int red = 0;
int blue = 0;
int green = 0;

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

  for (int i = 0; i < 5; i++) {
    tressholds[i] = targetTemp  + (i * 15);
  }
}

void loop() {
  if (!status) {
    analogWrite(pinRed, 0);
    analogWrite(pinGreen, 0);
    analogWrite(pinBlue, 0);
    analogWrite(pinMotor, 0);
    noTone(pinBuzzer);

    lcd.setCursor(0, 0);
    lcd.print("APAGADO        ");
    delay(200);
    return; // no sigue si está apagado
  }
   
  int valueInput = map(analogRead(pinPoten), 0, 1023, -15, 100);
  if (valueInput != targetTemp) {
    targetTemp = valueInput;
    for (int i = 0; i < 5; i++) {
      tressholds[i] = targetTemp - 30 + (i * 15);
    }
  }

  int currentTemp = analogRead(pinSensor) / 10; // simulado

  //Control del motor según temperatura
  if (currentTemp > 25) {
    motorSpeed = map(currentTemp, 26, 100, 100, 255);
    motorSpeed = constrain(motorSpeed, 100, 255);
  } else {
    motorSpeed = 0; // Motor apagado
  }
  analogWrite(pinMotor, motorSpeed);

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
  }
  
  analogWrite(pinRed, red);
  analogWrite(pinGreen, green);
  analogWrite(pinBlue, blue);

 
  int c = 0;
  for (int i = 0; i < 4 && tressholds[i] < currentTemp; i++) c++;

  int delta = 0;
  if (tressholds[c] > previousTemp) delta = 1;
  else if (tressholds[c] < previousTemp) delta = -1;
  previousTemp = tressholds[c];

  tone(pinBuzzer, alarm[c]);

  counter = constrain(counter, 0, 4);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(currentTemp);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Motor: ");
  lcd.print(motorSpeed);
  
  Serial.print("Temp: "); Serial.println(currentTemp);
  Serial.print("Motor: "); Serial.println(motorSpeed);
  Serial.print("Buzzer: "); Serial.println(alarm[c]);
  Serial.print("LED: R="); Serial.print(red);
  Serial.print(" G="); Serial.print(green);
  Serial.print(" B="); Serial.println(blue);
  
  delay(800);
  noTone(pinBuzzer);
}

void power() {
  unsigned long now = millis();
  if (now - lastInterrupt > 300) { // antirrebote
    status = !status;
    lastInterrupt = now;
  }
}