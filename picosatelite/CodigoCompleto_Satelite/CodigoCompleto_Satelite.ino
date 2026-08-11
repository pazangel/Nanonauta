//librerias 
#include <Adafruit_QMC5883P.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <TinyGPS++.h>
#include <SPI.h> 
#include <SD.h>
#include <LoRa.h>

Adafruit_BMP280 bmp;//objeto para el BMP280
Adafruit_QMC5883P qmc; //objeto para el magnetometro
TinyGPSPlus gps; //objeto para el gps

#define MPU_ADDR 0x68 // es la direccion del sesnor MPU-6050 con la que nos vamos a comunicar 

#define CS_PIN 13 // es el pin para controlar la coneccion de MOSI,MISO  y CLK de el lector SD

#define LORA_NSS  3 // es el pin para controlar la coneccion de MOSI,MISO  y CLK de la antena 
#define LORA_RST  2// para resetear el chip de la antena por software
#define LORA_DIO0 6 //para la señal de interrupción (avisa cuando terminó de transmitir o llegó un paquete)

// ===== Variables globales donde se guardan los datos ya listos =====

//variables de MPU-6050
float aceleracionX, aceleracionY, aceleracionZ;
float giroX, giroY, giroZ;
float TemperaturaMPU; // temperatura del MPU

// variables de BMP280
float bmpTemp, presion, altitud;

//variables de QMC5883P(Magnetometro)
int16_t magX = 0, magY = 0, magZ = 0;

//funciones para el mpu
void escribirRegistro(byte registro, byte valor) {
  Wire.beginTransmission(MPU_ADDR); 
  Wire.write(registro);
  Wire.write(valor);
  Wire.endTransmission();
}

void leerRegistros(byte registroInicial, byte *datos, byte cantidad) {
  Wire.beginTransmission(MPU_ADDR); 
  Wire.write(registroInicial);
  Wire.endTransmission(false); 

  Wire.requestFrom(MPU_ADDR, cantidad);

  byte i = 0;

  while (Wire.available() && i < cantidad) { 
    datos[i] = Wire.read(); 
    i++; 
  }
}

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial1.setRX(1);// le decimos al pin 1 como debe comportarse
  Serial1.setTX(0); //le decimos al pin 0 como debe comportarse
  Serial1.begin(9600); //inicializamos la comunicacion serial1

  Wire.setSDA(4);  // le decimos al pin 4 como debe comportarse
  Wire.setSCL(5);  // le decimos al pin 5 como debe comportarse

  Wire.begin();// ocupamos inicializar manualmente la comunicacion SPI

// Sacar el MPU del modo suspensión, es decir lo activa 
  escribirRegistro(0x6B, 0x00);

// se desactiva el modo maestro para que los datos pasen por XDA y XCL, esto permite que el MPU deje de controlar esos pines
  escribirRegistro(0x6A, 0x00);

// Activar BYPASS_EN, significa que hay que conectar el SCL y SDA del arduino con XDA y XCL del MPU para que alla paso de datos
  escribirRegistro(0x37, 0x02);

  Serial.println("Iniciando BMP280...");

   if (!bmp.begin()) { 
    Serial.println("No se encontró el BMP280");
  }
  else{
  Serial.println("BMP280 iniciado");
  }

    if (!qmc.begin()) {
    Serial.println("No se encontró el QMC5883P");
  }
  Serial.println("QMC5883P iniciado");

// Configuración deseada del magnetometro
  qmc.setMode(QMC5883P_MODE_NORMAL); // se usa QMC5883P_MODE_NORMAL para activar el sensor, si se pone QMC5883P_MODE_STANDBY se apaga el sensor
  qmc.setRange(QMC5883P_RANGE_8G); // Esto define el rango de sensibilidad del sensor, en Gauss (unidad de campo magnético), si se pone QMC5883P_RANGE_2G etecta campos magnéticos pequeños, con más precisión, 
  //si se pone QMC5883P_RANGE_8G  detecta campos magnéticos más grandes, con menos precisión

  SPI1.setRX(12); // le decimos al pin 12 como debe comportarse (como MISO)
  SPI1.setTX(11); // le decimos al pin 11 como debe comportarse (como MOSI)
  SPI1.setSCK(10); // le decimos al pin 10 como debe comportarse

  //configurar los pines cs de la antena y el lector sd
  pinMode(CS_PIN, OUTPUT);// del lector
  pinMode(LORA_NSS, OUTPUT);// de la antena 

// se les manda una señal para que ambos empiezen desconectados de MOSI,MISO y CLK, para que los dispositivos no esten conectados al mismo tiempo y no alla confictos
  digitalWrite(CS_PIN, HIGH);    
  digitalWrite(LORA_NSS, HIGH);  

//iniciar lector SD
   if (!SD.begin(CS_PIN, SPI1)) {  // si no inicia correctamente 
    Serial.println("Error al iniciar la SD");
    delay(500);
  }
  else{ // caso contrario si el lector SD inicia correctamente
    Serial.println("SD iniciada correctamente");
  }

//iniciar lora
  LoRa.setSPI(SPI1);
  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Error al iniciar LoRa, revisa conexiones!");
  }
  else{ //caso contrario si inicia bien la antena 
  
    // pequeñas configuraciones para la antena 
  LoRa.setSyncWord(0xA5);// le ponemos una clave a cada paquete que mandemos
  LoRa.enableCrc();// para calcular cuantos datos mandamos para saber si llegan corrompidos o no 
  Serial.println("LoRa iniciado correctamente");
  }

}

void loop() {
  //===================== MPU-6050 ===============================================
  byte datos[14];

  leerRegistros(0x3B, datos, 14);

  int16_t ax = (datos[0] << 8) | datos[1];
  int16_t ay = (datos[2] << 8) | datos[3];
  int16_t az = (datos[4] << 8) | datos[5];

  int16_t temperaturaRaw = (datos[6] << 8) | datos[7];

  int16_t gx = (datos[8] << 8) | datos[9];
  int16_t gy = (datos[10] << 8) | datos[11];
  int16_t gz = (datos[12] << 8) | datos[13];

  aceleracionX = ax / 16384.0;
  aceleracionY = ay / 16384.0;
  aceleracionZ = az / 16384.0;

  giroX = gx / 131.0;
  giroY = gy / 131.0;
  giroZ = gz / 131.0;

  TemperaturaMPU = temperaturaRaw / 340.0 + 36.53;

//================ BMP280 =========================

  bmpTemp = bmp.readTemperature();
  presion = bmp.readPressure() / 100.0F;
  altitud = bmp.readAltitude(1013.25);

//=============== Magnetometro =====================
  if (qmc.isDataReady()) {// si los datos estan listos para leer
    if (qmc.getRawMagnetic(&magX, &magY, &magZ)) {// guarda los datos en las variables de 16 bits, la funcion ocupa las direcciones de memoria de las variables, luego el if pregunta se leyeron correctamenete los datos?
// no ocupamos hacer nada ya lo hace la funcion qmc.getRawMagnetic
    }
  }

//================ Antena Lora =====================

  while (Serial1.available() > 0) {// mientras serial1 tenga datos por leer 
  gps.encode(Serial1.read());// lee byte por bye y se lo esntrga a la funcion encode
  }

LoRa.beginPacket();// crea un paquete de datos
// con la funcion LoRa.print agregamos lo que queramos en el paqute de datos 

//mandamos en forma de CSV
LoRa.print("SAT7X:" + String(aceleracionX) + "," + String(aceleracionY) + "," + String(aceleracionZ) + "," +String(giroX) + "," + String(giroY) + "," + String(giroZ) + "," +String(TemperaturaMPU) + "," +String(bmpTemp) + "," + String(presion) + "," + String(altitud) + "," +
String(magX) + "," + String(magY) + "," + String(magZ) + "," +String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," +String(gps.satellites.value()));

//Estructura: claveEntre antenas: acelerómetro eje X, aacelerómetro eje Y, acelerómetro eje Z, giroscopio eje X, giroscopio eje Y, giroscopio eje Z, TemperaturaMPU, temperatura del BMP280, presión atmosférica, altitud calculada por presión, magnetómetro eje X, magnetómetro eje Y, magnetómetro eje Z, latitud GPS, longitud GPS, satélites GPS

LoRa.endPacket();// termina de crear el paquete de datos y los envia 
  
  Serial.println("Datos Enviados por Lora");

  
  Serial.println("----------------------------------------------------------------------------------");

  // ===== Serial =====
  Serial.println("---MPU-6050---                     ---BMP280---                 ---Magnetometro---");
  Serial.println("Acelerometro en g: X:              Temperatura: " + String(bmpTemp)+ "          X: " + String(magX) + " Y: " + String(magY) + "Z:" + String(magZ));
  Serial.println(String(aceleracionX) + "  Y:" + String(aceleracionY) + "  Z:" + String(aceleracionZ) +"             Presion: " + String(presion) + " hPa");
  Serial.println("Giroscopio en grados/s: X:         Altitud: " + String(altitud) + " m");
  Serial.println(String(giroX) + " Y:" + String(giroY) + " Z:" + String(giroZ));
  Serial.println("Temp del MPU: " + String(TemperaturaMPU) + " °C");

    
    while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }
    Serial.println("\n---GPS---");
  if (gps.location.isUpdated()) {//accedemos al sub-objeto del obejo gps, dentro del sub-objeto accdemos a la funcion isUpdated(), la funcion nos dice si tenemos o no una posicion nueva 
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);//obtenemos latitud, y le decimos que queremos 6 decimales
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);//obtenemos longitud, y le decimos que queremos 6 decimales
    Serial.print("Altitud: ");
    Serial.print(gps.altitude.meters()); // obtenemos altitud
    Serial.println(" m");
  } else {// caso contrario si no tenemos una posicion 
    Serial.print("Sin fix aun | Satelites: ");
    Serial.print(gps.satellites.value()); // la funcion value() regresa un número entero con la cantidad de satélites que el GPS está usando actualmente para calcular la posición
    Serial.print(" | Caracteres procesados: ");
    Serial.println(gps.charsProcessed());//la funcion charsProcessed() es un contador acumulativo, cuenta todos los caracteres que le hemoss ido pasando a gps.encode() desde que arrancó el programa
    //sin importar si tiene o no posicion
  }
  Serial.println("----------------------------------------------------------------------------------\n");

//=================== lector SD =============================================

if (SD.exists("DatosSatelite.txt")) {
  SD.remove("DatosSatelite.txt"); // se quito |O_TRUNC y se puso la funcion SD.remove
}

  File archivo;
  if (!(archivo = SD.open("DatosSatelite.txt", O_WRITE | O_CREAT))) { // si el archivo no se abrio correctamente, si no existe lo crea con O_CREAT, le da permisos para escribir en el archivo con O_WRITE

  Serial.println("El archivo no abrio");
  delay(500);
}
else{ // caso contrario si el archivo abrio correctamente

// con la funcion archivo.println escribimos en el archivo
  archivo.println("---MPU-6050---                     ---BMP280---                 ---Magnetometro---");
  archivo.println("Acelerometro en g: X:              Temperatura: " + String(bmpTemp)+ "          X: " + String(magX) + " Y: " + String(magY) + "Z:" + String(magZ));
  archivo.println(String(aceleracionX) + "  Y:" + String(aceleracionY) + "  Z:" + String(aceleracionZ) +"             Presion: " + String(presion) + " hPa");
  archivo.println("Giroscopio en grados/s: X:         Altitud: " + String(altitud) + " m");
  archivo.println(String(giroX) + " Y:" + String(giroY) + " Z:" + String(giroZ));
  archivo.println("Temp del MPU: " + String(TemperaturaMPU) + " °C");

  while (Serial1.available() > 0) {
  gps.encode(Serial1.read());
}

archivo.println("\n---GPS---");
if (gps.location.isUpdated()) {
  archivo.print("Latitud: ");
  archivo.println(gps.location.lat(), 6);
  archivo.print("Longitud: ");
  archivo.println(gps.location.lng(), 6);
  archivo.print("Altitud: ");
  archivo.print(gps.altitude.meters());
  archivo.println(" m");
} else {
  archivo.print("Sin fix aun | Satelites: ");
  archivo.print(gps.satellites.value());
  archivo.print(" | Caracteres procesados: ");
  archivo.println(gps.charsProcessed());
}
archivo.println("----------------------------------------------------------------------------------\n");

  archivo.close(); // importante cerrar el archivo simepre que tenerminemos de escirbir en el archivo
  delay(200); // delay antes de leer
}

    // Leer archivo
  archivo = SD.open("DatosSatelite.txt");// abrimos el archivo 
  if (archivo) {// verfica si el archivo abrio bien 
  Serial.println("Datos del satelite en la SD:");
    while (archivo.available()) {//miestra el archivos todavia tenda datos por leer 
    byte caracter = archivo.read();
      Serial.write(caracter); // la funcion archivo.read() lee caracter por caracter y los imprimimos, el bucle while se repite asta que ya no tenga datos por leer
    }
    archivo.close();// despues de leer el archivo completo se cierra el archivo
  } else { // si el archivo no abre manda un mensaje de error
    Serial.println("Error al abrir archivo para leer");
  }

  delay(1000);
}


/*
NOS SOBRO MEMORIA FLASH Y RAM:
Flash:  5% usado   95% libre para código
RAM:    4% usado   96% libre para trabajo temporal

*/