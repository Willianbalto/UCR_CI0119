void setup() {
  // put your setup code here, to run once:

  //Colocar rango de temperatura minima y maxima default
  int targetTemp = 22; // con 22 en grados centiogrados

  //Cologar valores de pitch del buzzer
  int alarm[] = {220, 110, 523, 104};

  //Estado: apagado = false, encendido = true, iniciamos apagados, hay que presionar el boton para dar inicio
  bool status =  false;

  //Velocidad del motor
  int motorSpeed = 0; //asumiendo que asi funciona el motor

  //Datos del Led RGB 
  int blue = 0;
  int red = 0;
  int green = 0;
  //Revisar datos de colores rgb
}

void loop() {
  // put your main code here, to run repeatedly:

  //Leer el valor de temperatura deseable si hubo un cambio con el potenciometro
  //  Actualizar temperatura minima y maxima

  //Si esta encendido
  //  Leer datos analogicos del sensor
  //  Hacer conversiones de temperatura
  //  Encender leds dependiendo de la temperatura 
  //  Darle sonido al buzzer
  //  Encender ventilador si llega a temperatura de limite a la velocidad requerida utilizando el motor

  //Revisar boton para encendido y apagado

  
}
