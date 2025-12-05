// Proyecto: Encender luz y ventilador con aplauso

// Canal 1: Luz
// Canal 2: Ventilador

// Variables globales

int canalActivo = 1;            // Canal actualmente activo (1 = Luz, 2 = Ventilador)
bool luzEncendida = false;      // Estado actual de la luz (false = apagada, true = encendida)
bool ventiladorEncendido = false; // Estado actual del ventilador

// Prototipos de funciones

void ElegirCanal(String comando);  // Selecciona canal directamente
void CambiarCanal();               // Cambia canal (simula botón)
void Ventilador();                 // Alterna estado del ventilador
void Luces();                      // Alterna estado de la luz
void ImprimirEstado();             // Muestra en Serial los estados actuales
bool EstadoV();                    // Devuelve estado actual del ventilador
bool EstadoL();                    // Devuelve estado actual de la luz

// Configuración inicial (setup)

void setup() {
  Serial.begin(9600);   // Inicializamos comunicación Serial con PC
  Serial.println("--- Proyecto Aplausos ---");
  Serial.println("Canal inicial: LUZ");
  Serial.println("Opciones:");
  Serial.println("canal1 -> seleccionar canal de luz");
  Serial.println("canal2 -> seleccionar canal de ventilador");
  Serial.println("b -> cambiar de canal (simula botón)");
  Serial.println("aplauso -> enciende o apaga");
  Serial.println("estado -> imprime estado actual");
  ImprimirEstado(); 
}

// Bucle principal (loop)

void loop() {
  // Verificamos si hay datos disponibles en el monitor serial
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n'); // Leemos hasta salto de línea
    comando.trim(); // Quitamos espacios extra
    
    if (comando == "canal1" || comando == "canal2") {
      ElegirCanal(comando);
    } 
    else if (comando == "b") {
      CambiarCanal();
    }
    else if (comando == "aplauso") {
      // Según el canal activo, se controla luz o ventilador
      if (canalActivo == 1) {
        Luces();
      } else {
        Ventilador();
      }
    }
    else if (comando == "estado") {
      ImprimirEstado();
    }
    else {
      Serial.println("Comando no reconocido.");
    }
  }
}

// Funciones de control

// Selecciona directamente el canal (1 o 2) según el comando recibido
void ElegirCanal(String comando){
  if (comando == "canal1"){
    canalActivo = 1;
    Serial.println("Canal seleccionado: Luz");
  } else if (comando == "canal2"){
    canalActivo = 2;
    Serial.println("Canal seleccionado: Ventilador");
  }
}

// Cambia de canal (simula un botón físico que alterna entre luz y ventilador)
void CambiarCanal(){
  if (canalActivo == 1){
    canalActivo = 2;
    Serial.println("Canal cambiado a: Ventilador");
  } else {
    canalActivo = 1;
    Serial.println("Canal cambiado a: Luz");
  }
}

// Alterna el estado de la luz con cada "aplauso"
void Luces(){
  luzEncendida = !luzEncendida;  // Cambia de apagada a encendida y viceversa
  if (luzEncendida) {
    Serial.println("Luz encendida");
  } else {
    Serial.println("Luz apagada");
  }
}

// Alterna el estado del ventilador con cada "aplauso"
void Ventilador(){
  ventiladorEncendido = !ventiladorEncendido;
  if (ventiladorEncendido) {
    Serial.println("Ventilador encendido");
  } else {
    Serial.println("Ventilador apagado");
  }
}

// Imprime el estado actual de cada dispositivo
void ImprimirEstado(){
  Serial.println("------ ESTADO ACTUAL ------");

  // Canal activo
  if (canalActivo == 1){
    Serial.println("Canal activo: Luz");
  } else {
    Serial.println("Canal activo: Ventilador");
  }

  // Estado de la luz
  Serial.print("Luz: ");
  if (EstadoL()){
    Serial.println("Encendida");
  } else {
    Serial.println("Apagada");
  }

  // Estado del ventilador
  Serial.print("Ventilador: ");
  if (EstadoV()){
    Serial.println("Encendido");
  } else {
    Serial.println("Apagado");
  }
  Serial.println("---------------------------");
}

// Devuelve true si la luz está encendida
bool EstadoL(){
  return luzEncendida;
}

// Devuelve true si el ventilador está encendido
bool EstadoV(){
  return ventiladorEncendido;
}
