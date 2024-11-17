//Incluimos librerias necesarias

  #include <Servo.h>
  #include <SPI.h>
  #include <Wire.h>
  #include <dht.h>

//Definimos variables para el sistema de irrigacion automatico

  int humsuelo = 0;
  int sensuelo = A4;
  const int rele2 = 6;

//Definimos el sistema de segumiento solar

  const int ldr1Pin = A2;
  const int ldr2Pin = A3;

  const int umbral = 25;
  const int ganancia = 10;
  int anguloServo = 90;

//Definimos varibles para el sistema de apagado automatico por ausencia 

  const int pinSensor = 4; 
  const int rele = 5; 
  const long intervaloSinMovimiento = 5000; 

//Definimos variables para el sistema sensor de gas

  int valor = 0;
  int pinsensor = A1;
  int buzzer = 11; 

//Definimos el sensor de temperatura y humedad

  dht DHT;
  #define DHT11_PIN 2

//Definimos servos

  Servo ser1;
  Servo ser2;


void setup() {

//iniciamos el monitor serial  

  Serial.begin(9600);

//Definimos los pines

  pinMode(buzzer,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(pinSensor,INPUT);
  pinMode(sensuelo,INPUT);
  pinMode(pinsensor,INPUT);
  pinMode(10,OUTPUT);


//Inicializamos los servos

  ser1.attach(10);
  ser2.attach(9);

  ser1.write(90);
  ser2.write(0);

}


void loop()
 {

//Sistema de irrigacion automatico

  int humsuelo = analogRead(sensuelo);
  Serial.print("Humedad del Suelo ");
  Serial.println(humsuelo);

  if (humsuelo < 800 )   {
  
    digitalWrite(rele2,HIGH);
    delay(10000);
    digitalWrite(rele2,LOW);
    delay(50);

  }

  else{

    digitalWrite(rele2,LOW);
    delay(50);

  }

//Sistema de segumiento solar

  int lecturaLDR1 = analogRead(ldr1Pin);
  int lecturaLDR2 = analogRead(ldr2Pin);

  int diferencia = lecturaLDR1 - lecturaLDR2;

  if ( abs(diferencia) > umbral )   {
    anguloServo += diferencia * ganancia / 100;
    anguloServo = constrain(anguloServo, 0, 180);
    ser1.write(anguloServo);
  }


//Sistema de apagado automatico por ausencia

  int sensorValor = digitalRead(pinSensor);

  static unsigned long tiempoUltimoMovimiento = 0;

  if ( sensorValor == HIGH )    {
    tiempoUltimoMovimiento = millis();
  }

 unsigned long tiempoRestante = intervaloSinMovimiento - (millis() - tiempoUltimoMovimiento);

  if ( millis() - tiempoUltimoMovimiento >= intervaloSinMovimiento )    {
    digitalWrite(rele, LOW);
  }

  else {
    Serial.print("Tiempo restante: ");
    Serial.print(tiempoRestante / 1000); 
    Serial.println(" segundos");
    digitalWrite(rele,HIGH);
  }

//Sistema de sensor de gas

  valor = analogRead(pinsensor);

  Serial.print("Gas = ");
  Serial.println(valor);

  if ( valor > 265 )   {
  ser2.write(90);
  tone(buzzer,500,1000);
  tone(buzzer,0,1000);
  tone(buzzer,500,1000);
  delay(50);
  
  }

  else if ( valor < 265 )   {
  ser2.write(0);
  tone(buzzer,0,1000);
  }

//Sensor de temperatura y humedad

  DHT.read11(DHT11_PIN);

  Serial.print("Temperatura = ");
  Serial.print(DHT.temperature);
  Serial.println(" C");

  Serial.print("Humedad = ");
  Serial.print(DHT.humidity);
  Serial.println(" %");
  delay(1000);

}