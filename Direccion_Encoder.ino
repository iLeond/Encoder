#define ENCA 2 // Pin 2 del Arduino conectado al canal A del encoder (color amarillo)
#define ENCB 3 // Pin 3 del Arduino conectado al canal B del encoder (color blanco)

volatile long pulseCount = 0; // Variable para contar los pulsos del encoder
volatile bool direction = true; // Variable para indicar la dirección de rotación del encoder (true = adelante, false = atrás)

double rpm = 0; // Variable para guardar los RPMs del motor
double PPR = 400;

unsigned long prevMillis = 0; // Variable para guardar el tiempo del último muestreo
const unsigned long interval = 1000; // Intervalo de muestreo (en milisegundos)

void setup() {
  Serial.begin(2000000); // Inicializa la comunicación serial a 9600 baudios
  pinMode(ENCA, INPUT_PULLUP); // Configura el pin ENCA como entrada con resistencia pull-up activada
  pinMode(ENCB, INPUT_PULLUP); // Configura el pin ENCB como entrada con resistencia pull-up activada
  attachInterrupt(digitalPinToInterrupt(ENCA), updateEncoder, CHANGE); // Asigna la función updateEncoder() a la interrupción del pin ENCA
}

void loop() {
  unsigned long currentMillis = millis(); // Obtiene el tiempo actual en milisegundos
  if (currentMillis - prevMillis >= interval) { // Si ha pasado el intervalo de muestreo
    prevMillis = currentMillis; // Actualiza el tiempo del último muestreo
    detachInterrupt(digitalPinToInterrupt(ENCA)); // Desactiva la interrupción del pin ENCA
    double pulses = pulseCount; // Obtiene el número de pulsos del encoder durante el intervalo
    pulseCount = 0; // Reinicia el contador de pulsos del encoder
    attachInterrupt(digitalPinToInterrupt(ENCA), updateEncoder, CHANGE); // Reactiva la interrupción del pin ENCA
    rpm = (pulses * 35.93 ) / PPR; // Calcula los RPMs del motor ( pulsos por vuelta del eje)
  //  if (rpm > 0) {
    Serial.print("RPM: ");
    Serial.print(rpm);
    Serial.print(", dirección: ");
    Serial.println(direction ? "Adelante" : "Atrás");// Imprime la dirección de rotación del encoder
    //} 
    //else {
    //Serial.println("Sin movimiento");// Imprime que el carro esta sin movimiento 
    //}
  }
}

void updateEncoder() {
  bool a = digitalRead(ENCA); // Lee el estado actual del pin ENCA
  bool b = digitalRead(ENCB); // Lee el estado actual del pin ENCB
  if (a != b) { // Si los estados de ENCA y ENCB son diferentes, el encoder gira en sentido horario (adelante)
    direction = true; // Guarda la dirección de rotación del encoder (adelante)
    pulseCount++; // Incrementa el contador de pulsos del encoder
  } else { // Si los estados de ENCA y ENCB son iguales, el encoder gira en sentido antihorario (atrás)
    direction = false; // Guarda la dirección de rotación del encoder (atrás)
    pulseCount--; // Decrementa el contador de pulsos del encoder
  }

}
