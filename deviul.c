#include <SoftwareSerial.h>
#define button1 2
#define button2 3
#define button3 4
#define buzzerPin 5
#define motorPin 6
#define echoPin 7
#define trigPin 8
#define sosPin 9

long duration;
int distance, buzzer=1, motor=1, value=0, j=0;
float voltage, R1 = 10000.0, R2 = 100000.0;

SoftwareSerial sos(10, 12); // RX, TX

void setup() { 
  sos.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(sosPin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  delay(1000);
}

void loop() {
  // Envia um pulso para o modulo ultrassonico 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Velocidade do som dividida por 2 (ida e volta)

  // identifica se o botão de SOS foi pressionado
  if(digitalRead(sosPin)==0){
    sos.print("numero, Mensagem de SOS, SMS\r\n");
    sos.print("numero, , CALL\r\n");
  }
  // Define os modos de operação
  if(digitalRead(button1)==0){buzzer=1, motor=1;}  // buzzer e motor
  if(digitalRead(button2)==0){buzzer=1, motor=0;}  // buzzer somente 
  if(digitalRead(button3)==0){buzzer=0, motor=1;}  // motor somente

  // Se a distancia for menor que 70 cm, aciona o buzzer e o motor
  if(distance >0 && distance <=70){
    if(buzzer==1) tone(buzzerPin, 150, 50);
    if(motor==1) digitalWrite(motorPin, HIGH);
    delay(200);
    noTone(buzzerPin);
    digitalWrite(motorPin, LOW);
  }

  if(distance >70 && distance <110){
    if(buzzer==1) tone(buzzerPin, 150, 50);
    if(motor==1) digitalWrite(motorPin, HIGH);
    delay(850);
    noTone(buzzerPin);
    digitalWrite(motorPin, LOW);
  }

  if(distance >=110 && distance <=200){
    if(buzzer==1) tone(buzzerPin, 150, 50);
    if(motor==1) digitalWrite(motorPin, HIGH);
    delay(1200);
    noTone(buzzerPin);
    digitalWrite(motorPin, LOW);
  }

  // Identifica a tensão da bateria
  value = analogRead(A1);
  voltage = value * (5.0/1024)*((R1 + R2)/R2);

  // Se a tensão for menor que 3.5V, aciona o buzzer e o motor a cada 1:40 min
  if(voltage < 3.5 && j%200==0){
    digitalWrite(motorPin, HIGH);
    tone(buzzerPin, 1000, 1000);
    delay(500);
    noTone(buzzerPin);
    digitalWrite(motorPin, LOW);
  }
  j++;
  delay(100);
}