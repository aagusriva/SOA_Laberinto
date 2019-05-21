# Sistemas Operativos Avanzados

## Trabajo Práctico IOT, Sistemas Embebidos y Android

Integrantes:
- Ezequiel Focaraccio
- Luciano Galluzzo
- Agustin Riva


## LaberintoSMART

El proyecto se basa en un auto a escala capaz de resolver un laberinto construido con lineas negras sobre una base blanca.
El mismo utilizará sensores infrarrojo para detectar las líneas y así recorrer el laberinto, como también un sensor de distancia para evitar choques.

### Componentes:

- Arduino UNO
- Modulo Bluetooth
- Sensor Infrarrojo CNY70
- Sensor Ultrasónico HC-SR04
- 2 Motores con caja reductora
- 2 ruedas
- Rueda "loca"
- Protoboard
- Doble puente H L298

### Mobile module

El usuario dispondrá de una apliación mobile que le permitirá conectarse vía Bluetooth al auto. Una vez conectado, el usuario será capaz de:
- Iniciar recorrido
- Volver al inicio
- Calcular y registrar tiempos de resolución