#include <iostream>
#include <cmath>
#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 4, 3, 2, 1, 0)


int buzzer = 0; //simulando el buzzer, borrar luego
int motorSpeed = 0; //simulando el motor, borrar luego
bool status;
const int pinBuzzer = 11;
const int pinMotor = 9;
const int pinGreen = 6;
const int pinBlue = 7;
const int pinRed = 8;
const int pinPower = 10;
const int pinPoten = A1;
const int pinSensor = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600)

  //Estado: apagado = false, encendido = true, iniciamos apagados, hay que presionar el boton para dar inicio
  pinMode(pinPower, INPUT);
  status =  true;

  //Velocidad del motor
  motorSpeed = 0; //asumiendo que asi funciona el motor
  pinMode(pinMotor, OUTPUT);


  //Datos del Led RGB 
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(pinRed, OUTPUT);

  //int green = 0; posiblemente si usamen el led rgb
  //Revisar datos de colores rgb

  
}

  //Colocar rango de temperatura minima y maxima default
  int targetTemp = 22; // con 22 en grados centiogrados
  int tressholds[] = {targetTemp - 30, targetTemp - 15, targetTemp, targetTemp + 15, targetTemp + 30};

  //Cologar valores de pitch del buzzer
  int alarm[] = {1046, 523, 0, 220, 110};

  int previousTemp = targetTemp;

  int counter = 2; //para uso con el buzzer, sujeto a cambios para optimizar

void loop() {
  // put your main code here, to run repeatedly:
  //Leer el valor de temperatura deseable si hubo un cambio con el potenciometro
  valueInput = map(analogRead(pinPoten), 0, 1023,-15, 100);
  if(valueInput != targetTemp){
    targetTemp = valueInput;
    //Actualizar la pantalla
  }

  //encendido-apagado
  attachInterrupt(digitalPinToInterrupt(pinPower), power, change); //Manda a la funcion power si el boton se toco  
  if (!status){ //esto deria terminar el loop() si no esta encendido el aparato
    return;
  }

  counter > 4 ? counter = 4 : counter = counter;
  counter < 0 ? counter = 0 : counter = counter;

  std::cout << "Temperatura: \n";
  int currentTemp;
  std::cin >> currentTemp;
  int c = 0;

  for (int i = 0; tressholds[i] < currentTemp && i < 4; i++){
    c++;
  }

  int change = 0;
  if (tressholds[c] >= previousTemp){
    change = 1;
  } else if (tressholds[c] <= previousTemp){
    change = -1;
  } else {
    change = 0;
  }

  std::cout << "preeviousTemp: " << previousTemp << "\n";

  previousTemp = tressholds[c];

  //Si esta encendido
  //  Leer datos analogicos del sensor
  //  Hacer conversiones de temperatura
  //  Encender leds dependiendo de la temperatura 
  //  Darle sonido al buzzer
  //  Encender ventilador si llega a temperatura de limite a la velocidad requerida utilizando el motor
  buzzer = alarm[c]; //Posiblemente cambie para optimizacion
  //Recordar darle delay al buzzer
  tone(pinBuzzer, buzzer);
  switch (change)
  {
  case 1:
    motorSpeed = motorSpeed + 20; //No hemos experimentado con el motor por lo que los valores correctos estan sujetos a cambios
    counter++;
    red++;
    blue--;
    break;
  
  case -1:
    motorSpeed = motorSpeed - 20; //No hemos experimentado con el motor por lo que los valores correctos estan sujetos a cambios
    counter--;
    red--;
    blue++;
    break;

  default:
    break;
  }
  //Tests
  std::cout << "Velocidad del motor: " << motorSpeed << "\n";
  std::cout << "Valor del buzzer: " << buzzer << "\n";
  std::cout << "Red: " << red << "\n";
  std::cout << "Blue: " << blue << "\n";
}

void power(){
  status = !status;
  return;
}

//* Notas: ciertos valores no funcionan adecuadamente simulandolos sin embargo si los vemos en la realidad debido a que la temperatura no sube discretamente 
//*deberian funcionar, ademas muchas cosas estan sujetas a optimizacion
