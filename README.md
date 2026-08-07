<!----------------------------------------------------------------------------------->

<div align="center"> <!-- abre center para el titulo Nanonauta-->

# 🛰️ Nanonauta

</div> <!-- cierra center del titulo Nanonauta-->

<div align="center"> <!-- abre center para el titulo ¿Qué hace el proyecto?-->

## 🚀 ¿Qué hace el proyecto?

</div> <!-- cierra center para el titulo que hace nuestro proyecto-->

<div align="center"> <!-- abre center para parrafo de que hace el proyecto-->

Nanonauta es un picosatélite (CanSat). Su objetivo es recolectar datos ambientales y de orientación
durante el vuelo, incluyendo aceleración, rotación, temperatura, presión
atmosférica, altitud, campo magnético y posición GPS.

Estos datos se transmiten en tiempo real por radio LoRa (433 MHz) hacia
una estación de tierra, y al mismo tiempo se guardan localmente en una
tarjeta microSD como respaldo, en caso de que la comunicación se
interrumpa durante el vuelo.

</div> <!-- cierra center para el parrafo de que hace nuestro proyecto-->

<div align="center"> <!-- abre center para el titulo Hardware usado-->

## 🧩 Hardware usado

</div> <!-- cierra center para el titulo Hardware usado-->

<div align="center"> <!-- abre center para el subtitulo Picosatélite-->

### Picosatélite

</div> <!-- cierra center para el subtitulo Picosatélite-->

<div align="center"> <!-- abre center para la tabla de hardware del picosatelite-->

| Componente | Función |
|---|---|
| Waveshare RP2040-Zero | Microcontrolador para el picosatélite|
| Módulo LoRa Ra-02 (SX1278) | Transmisión de telemetría |
| MPU6050 | Acelerómetro + giroscopio + temperatura del mismo MPU6050|
| BMP280 | Temperatura, presión y altitud |
| QMC5883P | Magnetómetro (campo magnético) |
| GPS ATGM336H | Posicionamiento (latitud/longitud) |
| Lector microSD | Registro local de cada sensor y GPS |
| Batería LiPo 3.7V 800mAh | Alimentación |
| Regulador step-up XL6009 | Eleva 3.7V a 5V para el sistema |

</div> <!-- cierra center para la tabla de hardware del picosatelite-->

<div align="center"> <!-- abre center para el subtitulo Estación de tierra-->

### Estación de tierra

*(pendiente)*

</div> <!-- cierra center para el subtitulo Estación de tierra-->

<div align="center"> <!-- abre center para el titulo Cómo conectar todo-->

## 🔌 Cómo conectar todo

*(pendiente)*

</div> <!-- cierra center para el titulo Cómo conectar todo-->

<div align="center"> <!-- abre center para el titulo Cómo subir el código-->

## 💻 Cómo subir el código en la RP2040-Zero

</div> <!-- cierra center para el titulo Cómo subir el código-->

<div align="center"> <!-- abre center para el subtitulo Picosatélite RP2040-Zero-->

### Picosatélite (RP2040-Zero)

</div> <!-- cierra center para el subtitulo Picosatélite RP2040-Zero-->

<div align="center"> <!-- abre center para los pasos de como subir el codigo-->

1. Selecciona la placa correcta en Arduino IDE:
   `Herramientas → Placa → Raspberry Pi RP2040 → Waveshare RP2040-Zero`

2. Para generar el binario, ve a `Sketch → Exportar binario compilado`.
   Esto crea una carpeta `build`, y dentro de ella, tras entrar a varias
   subcarpetas, encontrarás el archivo `.uf2`.

3. Presiona el botón **BOOT** de la placa y conéctala a la PC (aparecerá
   como una memoria USB), o simplemente dale clic al botón de **Subir**
   en Arduino IDE, también saldrá como una memoria
   USB y ahí dentro tenemos que poner o pegar el binario `.uf2`.

4. Después de subir el binario, es posible que aparezca un error al
   intentar abrir el Monitor Serie (por ejemplo, en Linux:
   *"Could not connect to /dev/ttyACM0 serial port"*; en Windows
   podría mostrar un error similar referenciando el puerto COM). Este
   error se puede ignorar, si vuelves a abrir el Monitor Serie
   manualmente, los mensajes se muestran correctamente.

</div> <!-- cierra center para los pasos de como subir el codigo-->

<div align="center"> <!-- abre center para el titulo Formato de los datos transmitidos-->

## 📡 Formato de los datos transmitidos

</div> <!-- cierra center para el titulo Formato de los datos transmitidos-->

<div align="center"> <!-- abre center para la explicacion del formato CSV-->

Cada paquete enviado por LoRa incluye primero un identificador de equipo
(`SAT7X:`) seguido de los datos en formato CSV, en este orden:

```
SAT7X:acelerómetro eje X, acelerómetro eje Y, acelerómetro eje Z, giroscopio eje X, giroscopio eje Y, giroscopio eje Z, TemperaturaMPU, temperatura del BMP280, presión atmosférica, altitud calculada por presión, magnetómetro eje X, magnetómetro eje Y, magnetómetro eje Z, latitud GPS, longitud GPS, satélites GPS
```

Ejemplo real:

```
SAT7X:-0.05,0.53,0.72,-1.84,2.11,-0.15,44.20,24.83,844.75,1508.03,1638,-727,-472,19.432608,-99.133209,7
```

</div> <!-- cierra center para la explicacion del formato CSV-->

<div align="center"> <!-- abre center para la tabla de campos del CSV-->

| Campo | Descripción |
|---|---|
| acelerómetro eje X | Aceleración en el eje X |
| acelerómetro eje Y | Aceleración en el eje Y |
| acelerómetro eje Z | Aceleración en el eje Z |
| giroscopio eje X | Velocidad angular en el eje X |
| giroscopio eje Y | Velocidad angular en el eje Y |
| giroscopio eje Z | Velocidad angular en el eje Z |
| TemperaturaMPU | Temperatura del chip MPU6050 |
| temperatura del BMP280 | Temperatura ambiental medida por el BMP280 |
| presión atmosférica | Presión medida por el BMP280 |
| altitud calculada por presión | Altitud estimada a partir de la presión |
| magnetómetro eje X | Campo magnético en el eje X |
| magnetómetro eje Y | Campo magnético en el eje Y |
| magnetómetro eje Z | Campo magnético en el eje Z |
| latitud GPS | Latitud obtenida del GPS |
| longitud GPS | Longitud obtenida del GPS |
| satélites GPS | Número de satélites detectados por el GPS |

</div> <!-- cierra center para la tabla de campos del CSV-->

<div align="center"> <!-- abre center para el titulo Equipo-->

## 👥 Equipo

- [pazangel](https://github.com/pazangel)
- [ErickRodriguezR](https://github.com/ErickRodriguezR)
- [gustavocalderon067](https://github.com/gustavocalderon067)
- [robertytocerva](https://github.com/robertytocerva)

</div> <!-- cierra center para el titulo Equipo-->

<div align="center"> <!-- abre center para el titulo Licencia-->

## 📄 Licencia

Este proyecto está bajo la licencia MIT, ver [LICENSE](LICENSE) para más detalles.

</div> <!-- cierra center para el titulo Licencia-->


