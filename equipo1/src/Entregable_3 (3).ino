#include <LiquidCrystal.h>
int boton = 5;
int trigger = 2;
int echo = 3;
int rojo = 6;
int verde = 7;
int estado = 0;
int tiempo = 0;
int distancia = 50;
int preset = 0;
LiquidCrystal lcd(8,9,10,11,12,13);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(boton, INPUT_PULLUP);
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT); 
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

void loop(){

  unsigned long t_loop_start = micros();   
  unsigned long t1 = micros();            
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  unsigned long t2 = micros();           
  int estadoBoton = digitalRead(boton);
  int tiempo = pulseIn (echo, HIGH);
  distancia = (tiempo/2) * 0.0342;

  if (estadoBoton == HIGH) {
    if(preset==0){
        preset = 1;
     }else if(preset==1){
        preset = 0;
     }
  }
  Serial.println(preset);

  if(preset == 0){
    if(distancia >= 15){
      digitalWrite(rojo, HIGH);
      digitalWrite(verde, LOW);
      lcd.print("denegado");
      lcd.print(distancia);
      delay(800);
      lcd.clear();
    }
    if(distancia < 15){
      digitalWrite(rojo, LOW);
      digitalWrite(verde, HIGH);
      lcd.print("aceptado");
      lcd.print(distancia);
      delay(800);
      lcd.clear();
    }
  }

  if(preset == 1){
    if(distancia >= 30){
      digitalWrite(rojo, HIGH);
      digitalWrite(verde, LOW);
      lcd.print("denegado");
      lcd.print(distancia);
      delay(800);
      lcd.clear();
    }
    if(distancia < 30){
      digitalWrite(rojo, LOW);
      digitalWrite(verde, HIGH);
      lcd.print("aceptado");
      lcd.print(distancia);
      delay(800);
      lcd.clear();
    }
  }

  unsigned long t_loop_end = micros(); 

  Serial.print("Pulso digitalWrite: ");
  Serial.print(t2 - t1);
  Serial.print(" us   |   Loop total: ");
  Serial.print(t_loop_end - t_loop_start);
  Serial.println(" us");

  delay(200);
}

