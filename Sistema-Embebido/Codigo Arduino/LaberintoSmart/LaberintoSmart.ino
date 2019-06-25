char laberinto_path[100] = "";
unsigned char laberinto_longitud = 0;
int laberinto_indice = 0;
char band_inicio = 'N';
char band_manual = 'N';
int z;
int estaParado = 0;

//Led
#define ledPin 2

//Movimientos
#define GO_AHEAD 1
#define TURN_RIGHT 2
#define TURN_LEFT 3
#define TURN_RIGHT_SOFT 4
#define TURN_LEFT_SOFT 5
#define ADAPT_RIGHT 6
#define ADAPT_LEFT 7
#define ADAPT_RIGHT_SOFT 8
#define ADAPT_LEFT_SOFT 9
#define TURN_U 10
#define TURN_U_SOFT 11
#define STOP 12
#define TURN_RIGHT_MANUAL 13
#define TURN_LEFT_MANUAL 14

//Infrarrojo
int valorCI = 0;
int valorCD = 0;
int valorEI = 0;
int valorED = 0;
#define sensorCI 1
#define sensorCD 2
#define sensorEI 3
#define sensorED 4

//Motores
//Motor izquierda
#define pinENA 10
#define pinIN1 9
#define pinIN2 8
//Motor derecha
#define pinIN3 7
#define pinIN4 6
#define pinENB 5


//Ultrasonido
long t; //tiempo que demora en llegar el eco
long d; //distancia en centimetros
int finalizoLaberinto = 0;
#define pinUltraSonidoTrig  3
#define pinUltraSonidoEcho  4


//Bluethoot
#include <SoftwareSerial.h>
#define txBT 12
#define rxBT 11
char mensajeBT;
SoftwareSerial BT1(txBT,rxBT);

void setup() {
  Serial.begin(9600); // Comenzamos comunicacion serial.

  //LED
  pinMode(ledPin , OUTPUT); 
  
  //Motor
  pinMode (pinENA, OUTPUT);
  pinMode (pinENB, OUTPUT);
  pinMode (pinIN1, OUTPUT);
  pinMode (pinIN2, OUTPUT);
  pinMode (pinIN3, OUTPUT);
  pinMode (pinIN4, OUTPUT);
  
  //Ultrasonido
  pinMode(pinUltraSonidoTrig, OUTPUT); //pin como salida
  pinMode(pinUltraSonidoEcho, INPUT);  //pin como entrada
  digitalWrite(pinUltraSonidoTrig, LOW);//Inicializamos el pin con 0

  //Bluethoot
  BT1.begin(9600);
}

void loop() {  
  /*valorCI = analogRead(sensorCI);
  valorCD = analogRead(sensorCD);
  valorEI = analogRead(sensorEI);
  valorED = analogRead(sensorED);
  BT1.print(valorCI);
  BT1.print('\t');
  BT1.print(valorCD);
  BT1.print('\t');
  BT1.print(valorEI);
  BT1.print('\t');
  BT1.print(valorED);
  BT1.print('\n');
  if(valorEI <= 150){
    BT1.print('N');
  }else{
    BT1.print('B');
  }
  BT1.print('\t');
  if(valorCI <= 150){
    BT1.print('N');
  }else{
    BT1.print('B');
  }
  BT1.print('\t');
  if(valorCD <= 150){
    BT1.print('N');
  }else{
    BT1.print('B');
  }
  BT1.print('\t');
  if(valorED <= 150){
    BT1.print('N');
  }else{
    BT1.print('B');
  }
  BT1.print('\n');
  delay(150);*/
  
  while(band_inicio != 'B' && band_inicio != 'T' && band_inicio != 'F' && band_inicio != 'M' && band_inicio != 'L' && band_inicio != 'O' && band_inicio != 'S'){
    if (BT1.available()){
      band_inicio = BT1.read();
    }
  }
  
  if(band_inicio != 'T' && band_inicio != 'F' && band_inicio != 'M' && band_inicio != 'L' && band_inicio != 'O' && band_inicio != 'S'){
    encender_led();
    band_inicio = 'N';
  }else if(band_inicio != 'B' && band_inicio != 'F' && band_inicio != 'M' && band_inicio != 'L' && band_inicio != 'O' && band_inicio != 'S'){
    apagar_led();
    band_inicio = 'N';
  }else if(band_inicio != 'B' && band_inicio != 'T' && band_inicio != 'M' && band_inicio != 'L' && band_inicio != 'O' && band_inicio != 'S'){
    band_inicio = 'N';
  }else if(band_inicio != 'B' && band_inicio != 'T' && band_inicio != 'F' && band_inicio != 'L' && band_inicio != 'O' && band_inicio != 'S'){
    //Entramos en el modo Manual
    for(z=0;z<100;z++){
      laberinto_path[z] = 'Z';
    }
    laberinto_longitud = 0;
    laberinto_indice = 0;
    while(band_inicio != 'F'){
      if(band_inicio == 'B'){
        encender_led();
      }
      if(band_inicio == 'T'){
        apagar_led();
      }
      if(band_inicio == 'P'){
        //Debo frenar por lectura de sensor
        moverRobot(STOP);
      }else{
        digitalWrite(pinUltraSonidoTrig, HIGH);
        delayMicroseconds(10);//Enviamos un pulso de 10us
        digitalWrite(pinUltraSonidoTrig, LOW);
        
        t = pulseIn(pinUltraSonidoEcho, HIGH);//obtenemos el ancho del pulso
        d = t/59;//escalamos el tiempo a una distancia en cm
      
        if(d <= 10){
          if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) <= 150 && analogRead(sensorEI) <= 150 && analogRead(sensorED) <= 150){
            //Llego al final del camino
            moverRobot(STOP);
            BT1.print('F');
            //BT1.print('\n');
            band_inicio = 'F';
          }else{
            //Esta parado por un objeto
            moverRobot(STOP);
            //Aca debemos mandar un aviso por bluethoot de que esta parado por un objeto y no puede continuar
            if(estaParado == 0){
              BT1.print('P');
              //BT1.print('\n');
              estaParado = 1;
            }
          }
        }else{
          if(estaParado == 1){
            estaParado = 0;
            BT1.print('J');
            //BT1.print('\n');
          }
          if(analogRead(sensorEI) <= 150 && analogRead(sensorED) <= 150){
            while(analogRead(sensorEI) <= 150);
            if(analogRead(sensorCI) <= 150 || analogRead(sensorCD) <= 150){
              //Encontramos la combinacion I-A-D
              moverRobot(STOP);
              band_manual = recibir_senal(4);
              if(band_manual == 'I'){
                girarIzquierda(1);
              }else if(band_manual == 'D'){
                girarDerecha(1);
              }else{
                seguirAdelante(1);
              }
              band_manual = 'N';
            }else{
              //Encontramos la combinacion I-D
              moverRobot(STOP);
              band_manual = recibir_senal(3);
              if(band_manual == 'I'){
                girarIzquierda(1);
              }else{
                girarDerecha(1);
              }
              band_manual = 'N';
            }
          }else if(analogRead(sensorEI) <= 150 && analogRead(sensorED) > 150){
            while(analogRead(sensorEI) <= 150);
            if(analogRead(sensorCI) <= 150 || analogRead(sensorCD) <= 150){
              //Encontramos la combinacion I-A
              moverRobot(STOP);
              band_manual = recibir_senal(1);
              if(band_manual == 'I'){
                girarIzquierda(1);
              }else{
                seguirAdelante(1);
              }
              band_manual = 'N';
            }else{
              //Solo puede ir a la izquierda y lo hace
              girarIzquierda(1);
            }
          }else if(analogRead(sensorEI) > 150 && analogRead(sensorED) <= 150){
            while(analogRead(sensorED) <= 150);
            if(analogRead(sensorCI) <= 150 || analogRead(sensorCD) <= 150){
              //Encontramos la combinacion A-D
              moverRobot(STOP);
              band_manual = recibir_senal(2);
              if(band_manual == 'D'){
                girarDerecha(1);
              }else{
                seguirAdelante(1);
              }
              band_manual = 'N';
            }else{
              //Solo puede ir a la derecha y lo hace
              girarDerecha(1);
            }
          }else if(analogRead(sensorEI) > 150 && analogRead(sensorED) > 150 && analogRead(sensorCI) > 150 && analogRead(sensorCD) > 150){
            //Todos en blanco, girar en U
            girarEnU(1);
          }else{
            //Solo linea para adelante, seguir derecho
            if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) > 150){
              moverRobot(ADAPT_LEFT_SOFT);
              delay(2);
            }else if(analogRead(sensorCI) > 150 && analogRead(sensorCD) <= 150){
              moverRobot(ADAPT_RIGHT_SOFT);
              delay(2);
            }else{
              moverRobot(GO_AHEAD);
            }
          }
        }
      }
      //Leo por si recibo alguna señal de que debemos parar por lectura del sensor de distancia
      if (BT1.available()){
        band_inicio = BT1.read();
        if(band_inicio == 'F'){
          moverRobot(STOP);
        }
      }
    }
    //imprimirCaminoSimplificado();
  }else if(band_inicio != 'B' && band_inicio != 'T' && band_inicio != 'F' && band_inicio != 'M' && band_inicio != 'S' && band_inicio != 'L'){
    //Entramos en el modo Optimizado
    laberinto_indice = 0;
    while(band_inicio != 'F'){
      if(band_inicio == 'B'){
        encender_led();
      }
      if(band_inicio == 'T'){
        apagar_led();
      }
      if(band_inicio == 'P'){
        //Debo frenar por lectura de sensor
        moverRobot(STOP);
      }else{
        digitalWrite(pinUltraSonidoTrig, HIGH);
        delayMicroseconds(10);//Enviamos un pulso de 10us
        digitalWrite(pinUltraSonidoTrig, LOW);
        
        t = pulseIn(pinUltraSonidoEcho, HIGH);//obtenemos el ancho del pulso
        d = t/59;//escalamos el tiempo a una distancia en cm
      
        if(d <= 10){
          if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) <= 150 && analogRead(sensorEI) <= 150 && analogRead(sensorED) <= 150){
            //Llego al final del camino
            moverRobot(STOP);
            BT1.print('F');
            //BT1.print('\n');
            band_inicio = 'F';
          }else{
            //Esta parado por un objeto
            moverRobot(STOP);
            //Aca debemos mandar un aviso por bluethoot de que esta parado por un objeto y no puede continuar
            if(estaParado == 0){
              BT1.print('P');
              //BT1.print('\n');
              estaParado = 1;
            }
          }
        }else{
          if(estaParado == 1){
            estaParado = 0;
            BT1.print('J');
            //BT1.print('\n');
          }
          if(analogRead(sensorEI) <= 150 || analogRead(sensorED) <= 150){
            band_manual = sacar_path();
            if(band_manual == 'I'){
              girarIzquierda(2);
            }else if(band_manual == 'D'){
              girarDerecha(2);
            }else{
              seguirAdelante(2);
            }
            band_manual = 'N';
          }else{
            //Solo linea para adelante, seguir derecho
            if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) > 150){
              moverRobot(ADAPT_LEFT_SOFT);
              delay(2);
            }else if(analogRead(sensorCI) > 150 && analogRead(sensorCD) <= 150){
              moverRobot(ADAPT_RIGHT_SOFT);
              delay(2);
            }else{
              moverRobot(GO_AHEAD);
            }
          }
        }
      }
      //Leo por si recibo alguna señal de que debemos parar por lectura del sensor de distancia
      if (BT1.available()){
        band_inicio = BT1.read();
        if(band_inicio == 'F'){
          moverRobot(STOP);
        }
      }
    }
  }else if(band_inicio != 'B' && band_inicio != 'T' && band_inicio != 'F' && band_inicio != 'M' && band_inicio != 'S' && band_inicio != 'O'){
    //Entramos en el modo Aprender (Learning)
    laberinto_indice = 0;
    while(band_inicio != 'F'){
      if(band_inicio == 'B'){
        encender_led();
      }
      if(band_inicio == 'T'){
        apagar_led();
      }
      if(band_inicio == 'P'){
        //Debo frenar por lectura de sensor
        moverRobot(STOP);
      }else{
        digitalWrite(pinUltraSonidoTrig, HIGH);
        delayMicroseconds(10);//Enviamos un pulso de 10us
        digitalWrite(pinUltraSonidoTrig, LOW);
        
        t = pulseIn(pinUltraSonidoEcho, HIGH);//obtenemos el ancho del pulso
        d = t/59;//escalamos el tiempo a una distancia en cm
      
        if(d <= 10){
          if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) <= 150 && analogRead(sensorEI) <= 150 && analogRead(sensorED) <= 150){
            //Llego al final del camino
            moverRobot(STOP);
            BT1.print('F');
            //BT1.print('\n');
            band_inicio = 'F';
          }else{
            //Esta parado por un objeto
            moverRobot(STOP);
            //Aca debemos mandar un aviso por bluethoot de que esta parado por un objeto y no puede continuar
            if(estaParado == 0){
              BT1.print('P');
              //BT1.print('\n');
              estaParado = 1;
            }
          }
        }else{
          if(estaParado == 1){
            estaParado = 0;
            BT1.print('J');
            //BT1.print('\n');
          }
          if(analogRead(sensorEI) <= 150 || analogRead(sensorED) <= 150){
            band_manual = sacar_path();
            if(band_manual == 'I'){
              girarIzquierda(2);
            }else if(band_manual == 'D'){
              girarDerecha(2);
            }else{
              seguirAdelante(2);
            }
            band_manual = 'N';
          }else if(analogRead(sensorCI) <= 150 || analogRead(sensorCD) <= 150){
            if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) > 150){
              moverRobot(ADAPT_LEFT_SOFT);
              delay(2);
            }else if(analogRead(sensorCI) > 150 && analogRead(sensorCD) <= 150){
              moverRobot(ADAPT_RIGHT_SOFT);
              delay(2);
            }else{
              moverRobot(GO_AHEAD);
            }
          }else{
            girarEnU(2);
          }
        }
      }
      //Leo por si recibo alguna señal de que debemos parar por lectura del sensor de distancia
      if (BT1.available()){
        band_inicio = BT1.read();
        if(band_inicio == 'F'){
          moverRobot(STOP);
        }
      }
    }
  }else if(band_inicio != 'B' && band_inicio != 'T' && band_inicio != 'F' && band_inicio != 'M' && band_inicio != 'L' && band_inicio != 'O'){
    for(z=0;z<100;z++){
      laberinto_path[z] = 'Z';
    }
    laberinto_longitud = 0;
    laberinto_indice = 0;
    //Entramos en el modo Automatico
    while(band_inicio != 'F'){
      if(band_inicio == 'B'){
        encender_led();
      }
      if(band_inicio == 'T'){
        apagar_led();
      }
      if(band_inicio == 'P'){
        //Debo frenar por lectura de sensor
        moverRobot(STOP);
      }else{
        digitalWrite(pinUltraSonidoTrig, HIGH);
        delayMicroseconds(10);//Enviamos un pulso de 10us
        digitalWrite(pinUltraSonidoTrig, LOW);
        
        t = pulseIn(pinUltraSonidoEcho, HIGH);//obtenemos el ancho del pulso
        d = t/59;//escalamos el tiempo a una distancia en cm
      
        if(d <= 10){
          if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) <= 150 && analogRead(sensorEI) <= 150 && analogRead(sensorED) <= 150){
            //Llego al final del camino
            moverRobot(STOP);
            BT1.print('F');
            //BT1.print('\n');
            band_inicio = 'F';
          }else{
            //Esta parado por un objeto
            moverRobot(STOP);
            if(estaParado == 0){
              BT1.print('P');
              //BT1.print('\n');
              estaParado = 1;
            }
          }
        }else{
          if(estaParado == 1){
            estaParado = 0;
            BT1.print('J');
            //BT1.print('\n');
          }
          if(analogRead(sensorEI) <= 150){
            girarIzquierda(0);
          }else if(analogRead(sensorED) <= 150){
            //Lo hago avanzar hasta que deje de detectar la linea de la derecha
            while(analogRead(sensorED) <= 150);
            //Entonces pregunto si los de adelante son negros (alguno de los dos) y debe seguir adelante, sino girar a la derecha
            if(analogRead(sensorCI) <= 150 || analogRead(sensorCD) <= 150){
              //BT1.print('A');
              //BT1.print('\n');
              guardar_path('A',0);
            }else{
              girarDerecha(0);
            }
          }else if(analogRead(sensorCI) <= 150 || analogRead(sensorCD) <= 150){
            if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) > 150){
              moverRobot(ADAPT_LEFT_SOFT);
              delay(2);
            }else if(analogRead(sensorCI) > 150 && analogRead(sensorCD) <= 150){
              moverRobot(ADAPT_RIGHT_SOFT);
              delay(2);
            }else{
              moverRobot(GO_AHEAD);
            }
          }else{
            //Todos los sensores detectan blanco, debemos girar en U
            girarEnU(0);
          }
        }
      }
      //Leo por si recibo alguna señal de que debemos parar por lectura del sensor de distancia
      if (BT1.available()){
        band_inicio = BT1.read();
        if(band_inicio == 'F'){
          moverRobot(STOP);
        }
      }
    }
    //imprimirCaminoSimplificado();
  }
}

void encender_led(){
  digitalWrite(ledPin , HIGH);
}

void apagar_led(){
  digitalWrite(ledPin , LOW);
}

char recibir_senal(int combinacion){
  char band_rx = 'N';
  //BT1.print('R');
  //BT1.print('\n');
  while(band_rx != 'I' && band_rx != 'A' && band_rx != 'D'){
    if (BT1.available()){
      band_rx = BT1.read();
      if(combinacion == 1){
        if(band_rx == 'D'){
          //Solo puede recibir I o A
          band_rx = 'N';
        }
      }else if(combinacion == 2){
        if(band_rx == 'I'){
          //Solo puede recibir A o D
          band_rx = 'N';
        }
      }else if(combinacion == 3){
        if(band_rx == 'A'){
          //Solo puede recibir I o D
          band_rx = 'N';
        }
      }
      //Si la combinacion es la 4, no preguntamos nada porque cualquier cosa que recibamos (I, A o D) sirve 
      if(band_rx == 'B'){
        encender_led();
        band_rx = 'N';
      }
      if(band_rx == 'T'){
        apagar_led();
        band_rx = 'N';
      }
    }
  }
  return band_rx;
}

void girarEnU(int manual){
  moverRobot(STOP);
  if(manual != 2){
    guardar_path('U', manual);
  }
  //BT1.print('U');
  //BT1.print('\n');
  delay(100);
  moverRobot(TURN_RIGHT);
  delay(500);
  while(analogRead(sensorCI) > 150 && analogRead(sensorCD) > 150){
    moverRobot(TURN_RIGHT_SOFT);
  }
  moverRobot(STOP);
  delay(300);
}
void girarIzquierda(int manual){
  int velocidad;
  if(manual != 1){
    velocidad = TURN_LEFT;
  }else{
    velocidad = TURN_LEFT_MANUAL;
  }
  delay(100);//100
  moverRobot(STOP);
  if(manual != 2){
    guardar_path('I', manual);
  }
  //BT1.print('I');
  //BT1.print('\n');
  delay(100);
  moverRobot(velocidad);
  delay(500);
  while(analogRead(sensorCI) > 150 && analogRead(sensorCD) > 150){
    moverRobot(TURN_LEFT_SOFT);
  }
  moverRobot(STOP);
  delay(300);
}

void girarDerecha(int manual){
  int velocidad;
  if(manual != 1){
    velocidad = TURN_RIGHT;
  }else{
    velocidad = TURN_RIGHT_MANUAL;
  }
  delay(100);//100
  moverRobot(STOP);
  if(manual != 2){
    guardar_path('D', manual);
  }
  //BT1.print('D');
  //BT1.print('\n');
  delay(100);
  moverRobot(velocidad);
  delay(400);
  while(analogRead(sensorCI) > 150 && analogRead(sensorCD) > 150){
    moverRobot(TURN_RIGHT_SOFT);
  }
  moverRobot(STOP);
  delay(300);
}

void seguirAdelante(int manual){
  moverRobot(STOP);
  if(manual != 2){
    guardar_path('A', manual);
  }
  //BT1.print('A');
  //BT1.print('\n');
  int tiempo_inicial = millis();
  int tiempo_transcurrido = millis() - tiempo_inicial;
  while(tiempo_transcurrido < 500){
    if(analogRead(sensorCI) <= 150 && analogRead(sensorCD) > 150){
      moverRobot(ADAPT_LEFT_SOFT);
      delay(2);
    }else if(analogRead(sensorCI) > 150 && analogRead(sensorCD) <= 150){
      moverRobot(ADAPT_RIGHT_SOFT);
      delay(2);
    }else{
      moverRobot(GO_AHEAD);
    }
    tiempo_transcurrido = millis() - tiempo_inicial;
  }
}

char sacar_path(){
  char direccion = laberinto_path[laberinto_indice];
  laberinto_indice ++;
  return direccion;
}

void guardar_path(char direccion, int manual){
  laberinto_path[laberinto_longitud] = direccion;
  laberinto_longitud ++;
  if(manual == 0){
    resolver_laberinto();
  }
}

void resolver_laberinto(){
  if(laberinto_longitud < 3 || laberinto_path[laberinto_longitud-2] != 'U'){
    return; //sólo simplificar el camino si la segunda a la última vuelta fue una 'U'
  }
  int totalAngle = 0;
  int i;
  for(i=1;i<=3;i++)
  {
    switch(laberinto_path[laberinto_longitud-i]){
      case 'D':
        totalAngle += 90;
        break;
      case 'I':
        totalAngle += 270;
        break;
      case 'U':
        totalAngle += 180;
        break;
    }
  }
  totalAngle = totalAngle % 360; //Obtener el ángulo como un número entre 0 y 360 grados.
  switch(totalAngle){ //reemplazar todos los giros con una sola.
    case 0:
      laberinto_path[laberinto_longitud - 3] = 'A';
      break;
    case 90:
      laberinto_path[laberinto_longitud - 3] = 'D';
      break;
    case 180:
      laberinto_path[laberinto_longitud - 3] = 'U';
      break;
    case 270:
      laberinto_path[laberinto_longitud - 3] = 'I';
      break;
  }
  laberinto_longitud -= 2; //La ruta es ahora dos pasos más cortos.
}

void imprimirCaminoSimplificado(){
  int i;
  //BT1.print('\n');
  for(i=0;i<laberinto_longitud;i++){
    BT1.print(laberinto_path[i]);
    BT1.print('\t');
  }
  //BT1.print('\n');
  
}
String GetLine()
{   String S = "" ;
   if (Serial.available())
      {    char c = Serial.read(); ;
            while ( c != '\n')            //Hasta que el caracter sea intro
              {     S = S + c ;
                    delay(25) ;
                    c = Serial.read();
              }
            return( S + '\n') ;
      }
}

void moverRobot(int direccion){
  switch(direccion){
    case GO_AHEAD :  
                      analogWrite (pinENA, 170);
                      analogWrite (pinENB, 170);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_RIGHT : 
                      analogWrite (pinENA, 170);
                      analogWrite (pinENB, 0);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);//HIGH
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_LEFT :   
                      analogWrite (pinENA, 0);
                      analogWrite (pinENB, 170);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);//HIGH
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_RIGHT_SOFT : 
                      analogWrite (pinENA, 170);
                      analogWrite (pinENB, 0);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);//HIGH
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_LEFT_SOFT :
                      analogWrite (pinENA, 0);
                      analogWrite (pinENB, 170);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);//HIGH
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_RIGHT : 
                      analogWrite (pinENA, 105);//85
                      analogWrite (pinENB, 200);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);
                      digitalWrite (pinIN4, HIGH);
                      break;
    case TURN_LEFT : 
                      analogWrite (pinENA, 170);
                      analogWrite (pinENB, 100);//85
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_RIGHT_SOFT : 
                      analogWrite (pinENA, 95);//70
                      analogWrite (pinENB, 95);//70
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);
                      digitalWrite (pinIN4, HIGH);
                      break;
    case TURN_LEFT_SOFT : 
                      analogWrite (pinENA, 95);//70
                      analogWrite (pinENB, 95);//70
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_RIGHT_MANUAL : 
                      analogWrite (pinENA, 150);//70
                      analogWrite (pinENB, 150);//70
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);
                      digitalWrite (pinIN4, HIGH);
                      break;
    case TURN_LEFT_MANUAL : 
                      analogWrite (pinENA, 150);//70
                      analogWrite (pinENB, 150);//70
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_U :
                      analogWrite (pinENA, 170);
                      analogWrite (pinENB, 100);//85
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_U_SOFT :
                      analogWrite (pinENA, 110);//70
                      analogWrite (pinENB, 110);//70
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case STOP :
                      analogWrite (pinENA, 0);
                      analogWrite (pinENB, 0);
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);
                      digitalWrite (pinIN4, LOW);
                      break;
    default :
                      analogWrite (pinENA, 0);
                      analogWrite (pinENB, 0);
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);
                      digitalWrite (pinIN4, LOW);
                      break;
  }
}
