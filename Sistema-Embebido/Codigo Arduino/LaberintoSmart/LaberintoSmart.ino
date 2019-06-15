int primero = 0;

//Infrarrojo
int entroAUnIf = 0;
int estaGirandoEnU = 0;
int estaGirandoEnUSuave = 0;
int estaGirandoIzquierda = 0;
int estaGirandoIzquierdaSuave = 0;
int estaGirandoDerecha = 0;
int estaGirandoDerechaSuave = 0;

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

/*
int infraPin1 = 7;
*/
int valorCI = 0;
int valorCD = 0;
int valorEI = 0;
int valorED = 0;
int valorEC = 0;

//Motores
//Motor A
int pinENA = 10;
int pinIN1 = 9;
int pinIN2 = 8;
//Motor B
int pinIN3 = 7;
int pinIN4 = 6;
int pinENB = 5;


//Ultrasonido
long t; //timepo que demora en llegar el eco
long d; //distancia en centimetros
int paradoPorObjeto = 0;

int pinUltraSonidoTrig = 3;
int pinUltraSonidoEcho = 4;


//Bluethoot
#include <SoftwareSerial.h>
int txBT = 12;
int rxBT = 11;
char mensajeBT;
SoftwareSerial BT1(txBT,rxBT);

void setup() {
  Serial.begin(9600); // Comenzamos comunicacion serial.
  
  //Infrarrojo
  /*pinMode (infraPin1, INPUT);
  */
  
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
  Serial.println("Enter AT commands:");
  BT1.begin(9600);
}

void loop() {
  //Infrarrojo
  /*valorInfra1 = digitalRead(infraPin1);*/
  valorCI = analogRead(1); //Centro Izquierda
  valorCD = analogRead(2); //Centro Derecha
  valorEI = analogRead(3); //Exterior Izquierda
  valorED = analogRead(4); //Exterior Derecha
  valorEC = analogRead(5); //Extremo Centro
  /*Serial.print(valorEI);
  Serial.print("\t");
  Serial.print(valorCI);
  Serial.print("\t");
  Serial.print(valorCD);
  Serial.print("\t");
  Serial.print(valorED);
  Serial.print("\t\t");
  Serial.print(valorEC);
  Serial.println("");
  delay(500);*/
  
  if(primero == 0){
    delay(5000);
    primero = 1;
  }
  
  //Medidor de distancia
  /*if(estaGirandoEnU == 0 && girandoIzquierda == 0 && girandoDerecha == 0){
    digitalWrite(pinUltraSonidoTrig, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(pinUltraSonidoTrig, LOW);
    
    t = pulseIn(pinUltraSonidoEcho, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm
  
    if(d <= 20){
      moverRobot(STOP);
      paradoPorObjeto = 1;
    }else{
      paradoPorObjeto = 0;
    }
  }*/

  if(paradoPorObjeto == 0){
    //Seguir adelante
    if(valorCI <= 80 && valorCD <= 80 && entroAUnIf == 0 && valorEI > 80 && valorED > 80){
      moverRobot(GO_AHEAD);
      entroAUnIf = 1;
    }
    //Acomodar a la izquierda
    if(valorCI <= 80 && valorCD > 80 && entroAUnIf == 0 && valorEI > 80 && valorED > 80){
      entroAUnIf = 1;
      /*if(estaGirandoEnU == 1 || estaGirandoIzquierda == 1){
        estaGirandoEnU = 0;
        estaGirandoEnUSuave = 0;
        estaGirandoIzquierda = 0;
        estaGirandoIzquierdaSuave = 0;
        moverRobot(STOP);
        delay(200);
      }else{
        moverRobot(ADAPT_LEFT_SOFT);
        delay(2);
      }*/
      if(estaGirandoDerecha == 1){
        estaGirandoDerecha = 0;
        estaGirandoDerechaSuave = 0;
        moverRobot(STOP);
        delay(200);
      }else{
        if(estaGirandoIzquierda == 1){
          estaGirandoIzquierda = 0;
          estaGirandoIzquierdaSuave = 0;
          moverRobot(STOP);
          delay(200);
        }else{
          if(estaGirandoEnU == 1){
            estaGirandoEnU = 0;
            estaGirandoEnUSuave = 0;
            moverRobot(STOP);
            delay(200);
          }else{
            moverRobot(ADAPT_LEFT_SOFT);
            delay(2);
          }
        }
      }
    }
    //Acomodar a la derecha
    if(valorCI > 80 && valorCD <= 80 && entroAUnIf == 0 && valorEI > 80 && valorED > 80){
      entroAUnIf = 1;
      if(estaGirandoDerecha == 1){
        estaGirandoDerecha = 0;
        estaGirandoDerechaSuave = 0;
        moverRobot(STOP);
        delay(200);
      }else{
        if(estaGirandoIzquierda == 1){
          estaGirandoIzquierda = 0;
          estaGirandoIzquierdaSuave = 0;
          moverRobot(STOP);
          delay(200);
        }else{
          if(estaGirandoEnU == 1){
            estaGirandoEnU = 0;
            estaGirandoEnUSuave = 0;
            moverRobot(STOP);
            delay(200);
          }else{
            moverRobot(ADAPT_RIGHT_SOFT);
            delay(2);
          }
        }
      }
    }
    //Girar en U
    if(valorCI > 80 && valorCD > 80 && entroAUnIf == 0 && estaGirandoIzquierda == 0 && estaGirandoDerecha == 0){
      entroAUnIf = 1;
      estaGirandoEnU = 1;
      if(estaGirandoEnUSuave == 0){
        moverRobot(TURN_U);
        delay(1000);
      }
      moverRobot(TURN_U_SOFT);
      estaGirandoEnUSuave = 1;
    }
    //Girar a la izquierda
    if(valorCI <= 80 && valorCD <= 80 && valorEI <= 80 && estaGirandoDerecha == 0){
      //entroAUnIf = 1;
      estaGirandoIzquierda = 1;
      if(estaGirandoIzquierdaSuave == 0){
        moverRobot(TURN_LEFT);
        delay(500);
      }
      moverRobot(TURN_LEFT_SOFT);
      estaGirandoIzquierdaSuave = 1;
    }
    //Girar a la derecha (Si el de adelante detecta negro, debe seguir adelante por la regla de la mano izquierda)
    if(valorCI <= 80 && valorCD <= 80 && valorED <= 80 && valorEI > 80 && valorEC > 80 && estaGirandoIzquierda == 0){
      //entroAUnIf = 1;
      estaGirandoDerecha = 1;
      if(estaGirandoDerechaSuave == 0){
        moverRobot(TURN_RIGHT);
        delay(500);
      }
      moverRobot(TURN_RIGHT_SOFT);
      estaGirandoDerechaSuave = 1;
    }
  }

  //moverRobot(GO_AHEAD);
  
  entroAUnIf = 0;
  
  //Bluethoot
  /*if (BT1.available()){
    mensajeBT = BT1.read();
    //Serial.write(BT1.read());
  }
  if(mensajeBT == 'F'){
    Serial.println("Finalizar recorrido");
    mensajeBT = ' ';
  }*/
  
  /*if (Serial.available())
    {  String S = GetLine();
       BT1.print(S);
       Serial.println("---> " + S);
    }*/
}

void moverRobot(int direccion){
  switch(direccion){
    case GO_AHEAD :  
                      analogWrite (pinENA, 170);//170
                      analogWrite (pinENB, 170);//170
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_RIGHT : 
                      analogWrite (pinENA, 85);
                      analogWrite (pinENB, 170);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);
                      digitalWrite (pinIN4, HIGH);
                      break;
    case TURN_LEFT : 
                      analogWrite (pinENA, 170);
                      analogWrite (pinENB, 85);
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_RIGHT_SOFT : 
                      analogWrite (pinENA, 70);
                      analogWrite (pinENB, 70);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);
                      digitalWrite (pinIN4, HIGH);
                      break;
    case TURN_LEFT_SOFT : 
                      analogWrite (pinENA, 70);
                      analogWrite (pinENB, 70);
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_RIGHT : 
                      analogWrite (pinENA, 170);//170
                      analogWrite (pinENB, 0);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);//HIGH
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_LEFT :   
                      analogWrite (pinENA, 0);
                      analogWrite (pinENB, 170);//170
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);//HIGH
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_RIGHT_SOFT : 
                      analogWrite (pinENA, 170);//170
                      analogWrite (pinENB, 0);
                      //Direccion motor A
                      digitalWrite (pinIN1, HIGH);
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, LOW);//HIGH
                      digitalWrite (pinIN4, LOW);
                      break;
    case ADAPT_LEFT_SOFT :
                      analogWrite (pinENA, 0);
                      analogWrite (pinENB, 170);//170
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);//HIGH
                      digitalWrite (pinIN2, LOW);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_U :
                      analogWrite (pinENA, 170);
                      analogWrite (pinENB, 85);
                      //Direccion motor A
                      digitalWrite (pinIN1, LOW);
                      digitalWrite (pinIN2, HIGH);
                      //Direccion motor B
                      digitalWrite (pinIN3, HIGH);
                      digitalWrite (pinIN4, LOW);
                      break;
    case TURN_U_SOFT :
                      analogWrite (pinENA, 70);
                      analogWrite (pinENB, 70);
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
