int infraPin1 = 8;
int infraPin2 = 7;
int valorInfra1 = 0;
int pinMotor1 = 12;
int pinMotor2 = 13;

void setup() {
  Serial.begin(9600); // Comenzamos comunicacion serial.
  pinMode(infraPin1, INPUT);
  pinMode(infraPin2, OUTPUT);
  pinMode(pinMotor1, OUTPUT);
  pinMode(pinMotor2, OUTPUT);
}

void loop() {
  valorInfra1 = analogRead(0);
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

  //delay(500);
  
}
