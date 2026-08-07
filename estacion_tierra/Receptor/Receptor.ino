#include <SPI.h>
#include <LoRa.h>

#define LORA_NSS 10
#define LORA_RST 9
#define LORA_DIO0 2

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Iniciando LoRa receptor...");

  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);  //  Le dice a la funcion setPins cuáles pines usar para el control del chip

  if (!LoRa.begin(433E6)) {  //inicializa la comunicacion SPI con la antena a 433,000,000 Hz (433E6 es notación científica: 433 × 10⁶), si da falso manda un mensaje y entra en un bucle vacio
    Serial.println("Error al iniciar LoRa, revisa conexiones!");
    while (1);
  }
  LoRa.setSyncWord(0xA5);
  LoRa.enableCrc();
  /*
  LoRa.setSyncWord(0xA5); le pone un codigo seccreto a cada paquete de datos que transmitimos, si el recpetor escucha en la misma frecuencia el CHIP a nivel de hardware
  revisa:
  ¿Este paquete que llegó tiene el Sync Word 0xA5?
  Si coincide procesa el paquete, lo entrega
  Si NO coincide lo descarta automáticamente, ni siquiera nos enterariamos de que llegó

  "Aunque este no descarta por completo los paquetes que no le pertenecen, pueden seguir llegando paquetes no todos pero si algunos y algunos llegaran corruptos"

  LoRa.enableCrc();
  este es el que quita los paquetes corruptos que recibimos apartir del tamaño de datos que manda el tranmisor, asi se da cuenta de si un paquete llega corrompido

  si el receptor lo

  */
  Serial.println("LoRa receptor listo, esperando paquetes...");
}

void loop() {
  /*int packetSize = LoRa.parsePacket();  // la funcion LoRa.parsePacket() verifica si llego un mensaje, si llego regresa el tamaño en bytes de ese paquete
  if (packetSize) {                     // cualquier numero diferente de 0 se evalua como true, asi que si el tamaño de bytes es mayor a 0, se cumplue la condicion
    Serial.println("Paquete recibido: ");
    while (LoRa.available()) {          // mientras todavia alla bytes por leer
      Serial.print((char)LoRa.read());  // LoRa.read() lee un solo byte y avanza al siguiente, luego con (char) lo convertimos a caracter, ya que el byte que llega es un numero ascii
    }
    Serial.println("\n");
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());  // la función LoRa.packetRssi() nos da la fuerza de la señal (RSSI) del paquete que acabas de recibir
    Serial.println();
  }
*/
int packetSize = LoRa.parsePacket(); // la funcion LoRa.parsePacket() verifica si llego un mensaje, si llego regresa el tamaño en bytes de ese paquete
if (packetSize) { // cualquier numero diferente de 0 se evalua como true, asi que si el tamaño de bytes es mayor a 0, se cumplue la condicion
  String mensaje = "";
  while (LoRa.available()) {// mientras todavia alla bytes por leer
    mensaje += (char)LoRa.read();// lee un byte y avanza al siguienete,lo convierte a char al numero ascii y lo agrega al string
  }

  if (mensaje.startsWith("SAT7X:")) { // si mensaje empieza con SAT7X:
    mensaje = mensaje.substring(6); // dividimos el string, es decir nso quedamos solo con el indice 6 en adelante, del indice 0 al 5 lo ignoramos esos indices corresponden a SAT7X:, cambiamos el valor de String mensaje = "";
    Serial.println("Paquete recibido: ");
    Serial.println(mensaje);// imprimimos el paquete 
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi()); // la función LoRa.packetRssi() nos da la fuerza de la señal (RSSI) del paquete que acabas de recibir
  } else {// si el mensaje no empieza con SAT7X: no es nuestro paquete 
    Serial.println("Paquete ignorado (no es nuestro)");
    Serial.println(mensaje);
  }
}
}
/*
significado de RSSI:
Received Signal Strength Indicator

o en español:
Indicador de Intensidad de Señal Recibida


=============¿como sabe el receptor que un paquete es para el?==================

El receptor solo sabe "estoy sintonizado a 433 MHz, y algo llegó en esa frecuencia, lo capto y lo recibe

Frecuencia 433 MHz = la estación de radio
LoRa.begin(433E6)  = sintonizar tu radio a esa estación específica

Cualquier radio sintonizado ahí escucha lo mismo,
sin importar si el locutor "quería hablarle a ese radio en particular" o no
*/