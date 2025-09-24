/*
 * Grupo 1
 * Adrian Rodriguez C39321
 * Luis Sanchez C17232
 * Jose Pablo C4L627
 * 
 */
#include <LiquidCrystal.h>
int boton1 = 13;
int boton2 = 10;
int trigger = 12;
int echo = 11;
int rojo = 3;
int verde = 2;
int estado = 0;
int tiempo = 0;
int distancia = 0;
int preset = 0;
LiquidCrystal lcd(4,5,6,7,8,9);

void setup()
{
  int brightness = 100;
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT); 
}

void loop(){
  
  int estadoBoton1 = digitalRead(boton1);
  int estadoBoton2 = digitalRead(boton2);
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  digitalWrite(trigger, HIGH);
  digitalWrite(trigger, LOW);
  pinMode(echo, INPUT);
  int tiempo = pulseIn (echo, HIGH);
  distancia = (tiempo/2) * 0.0342;
  
  if (estadoBoton1 == HIGH) {
    preset = 0;
  }
  if (estadoBoton2 == HIGH) {
    preset = 1;
  }
  if(preset == 0){
    if(distancia >= 150){
      digitalWrite(rojo, HIGH);
      digitalWrite(verde, LOW);
      lcd.print("denegado");
      delay(800);
      lcd.clear();
    }
    if(distancia < 150){
      digitalWrite(rojo, LOW);
      digitalWrite(verde, HIGH);
      lcd.print("aceptado");
      delay(800);
      lcd.clear();
    }
  }
  if(preset == 1){
    if(distancia >= 220){
      digitalWrite(rojo, HIGH);
      digitalWrite(verde, LOW);
      lcd.print("denegado");
      delay(800);
      lcd.clear();
    }
    if(distancia < 220){
      digitalWrite(rojo, LOW);
      digitalWrite(verde, HIGH);
      lcd.print("aceptado");
      delay(800);
      lcd.clear();
    }
  }
}
