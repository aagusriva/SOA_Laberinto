int infraPin1 = 8;
int infraPin2 = 7;
int valorInfra1 = 0;

//Motor A
int pinENA = 10;
int pinIN1 = 9;
int pinIN2 = 8;
//Motor B
int pinIN3 = 7;
int pinIN4 = 6;
int pinENB = 5;

//Ultrasonido
int pinUltraSonidoTrig = 3;
int pinUltraSonidoEcho = 4;

//Bluethoot
#include <SoftwareSerial.h>
SoftwareSerial BT1(4,2);

void setup() {
  Serial.begin(9600); // Comenzamos comunicacion serial.
  
  //Motor
  pinMode (pinENA, OUTPUT);
  pinMode (pinENB, OUTPUT);
  pinMode (pinIN1, OUTPUT);
  pinMode (pinIN2, OUTPUT);
  pinMode (pinIN3, OUTPUT);
  pinMode (pinIN4, OUTPUT);
  
  
  //Ultrasonido
  /*pinMode(pinUltraSonidoTrig, OUTPUT); //pin como salida
  pinMode(pinUltraSonidoEcho, INPUT);  //pin como entrada
  digitalWrite(pinUltraSonidoTrig, LOW);//Inicializamos el pin con 0
  */

  //Bluethoot
  /*Serial.println("Enter AT commands:");
  BT1.begin(9600);*/
}

void loop() {
  /*valorInfra1 = analogRead(0);
  //Serial.print("SENSOR1: ");
  //Serial.println(valorInfra1);

  if(valorInfra1 > 10){
    digitalWrite(infraPin2,HIGH);
    //Para adelante
    digitalWrite (pinMotor1, HIGH);
    digitalWrite (pinMotor2, LOW);
  }else{
    digitalWrite(infraPin2,LOW);
    //Para atras
    digitalWrite (pinMotor1, LOW);
    digitalWrite (pinMotor2, HIGH);
  }

  //delay(500);*/

  //Medidor de distancia
  /*long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(pinUltraSonidoTrig, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(pinUltraSonidoTrig, LOW);
  
  t = pulseIn(pinUltraSonidoEcho, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm
  
  Serial.print("Distancia: ");
  Serial.print(d);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  delay(1000);          //Hacemos una pausa de 100ms
  */
  //Motor
  Adelante ();
  delay (10000);
  /*Atras ();
  delay (10000);
  Derecha ();
  delay (10000);
  Izquierda ();
  delay (10000);*/
  Parar ();
  delay (10000);
  

  //Bluethoot
  /*if (BT1.available())
     Serial.write(BT1.read());
  
  if (Serial.available())
    {  String S = GetLine();
       BT1.print(S);
       Serial.println("---> " + S);
    }*/
}

void Adelante ()
{
 //Direccion motor A
 digitalWrite (pinIN1, HIGH);
 digitalWrite (pinIN2, LOW);
 analogWrite (pinENA, 255); //Velocidad motor A
 //Direccion motor B
 digitalWrite (pinIN3, HIGH);
 digitalWrite (pinIN4, LOW);
 analogWrite (pinENB, 255); //Velocidad motor B
}

void Atras ()
{
 //Direccion motor A
 digitalWrite (pinIN1, LOW);
 digitalWrite (pinIN2, HIGH);
 analogWrite (pinENA, 128); //Velocidad motor A
 //Direccion motor B
 digitalWrite (pinIN3, LOW);
 digitalWrite (pinIN4, HIGH);
 analogWrite (pinENB, 128); //Velocidad motor B
}

void Derecha ()
{
 //Direccion motor A
 digitalWrite (pinIN1, HIGH);
 digitalWrite (pinIN2, LOW);
 analogWrite (pinENA, 200); //Velocidad motor A
 //Direccion motor B
 digitalWrite (pinIN3, LOW);
 digitalWrite (pinIN4, HIGH);
 analogWrite (pinENB, 100); //Velocidad motor A
}

void Izquierda ()
{
 //Direccion motor A
 digitalWrite (pinIN1, LOW);
 digitalWrite (pinIN2, HIGH);
 analogWrite (pinENA, 100); //Velocidad motor A
 //Direccion motor B
 digitalWrite (pinIN3, HIGH);
 digitalWrite (pinIN4, LOW);
 analogWrite (pinENB, 200); //Velocidad motor A
}

void Parar ()
{
 //Direccion motor A
 digitalWrite (pinIN1, LOW);
 digitalWrite (pinIN2, LOW);
 analogWrite (pinENA, 0); //Velocidad motor A
 //Direccion motor B
 digitalWrite (pinIN3, LOW);
 digitalWrite (pinIN4, LOW);
 analogWrite (pinENB, 0); //Velocidad motor A
}

/*
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
}*/
