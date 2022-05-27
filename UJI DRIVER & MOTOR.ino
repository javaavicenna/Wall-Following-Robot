//alamat driver
const int  IN1 = 18; // deklarasi pin IN1
const int  IN2 = 5;  // deklarasi pin IN2
const int  IN3 = 4;  // deklarasi pin IN3
const int  IN4 = 2;  // deklarasi pin IN4
const int  ENA = 19; // deklarasi pin ENA
const int  ENB = 15;  // deklarasi pin ENB


//-----------PWM
const int freq = 1000;
const int resolution = 8;





void setup() {
  
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
    // konfigurasikan fungsionalitas PWM LED
  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);
  
    // controlling output with PWM
  ledcAttachPin(ENA, 0);
  ledcAttachPin(ENB, 1);

Serial.begin(9600);

}

void loop() {
 
    ledcWrite(0, 40);
    ledcWrite(1, 40);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(1000);
    ledcWrite(0, 100);
    ledcWrite(1, 100);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(1000);
    ledcWrite(0, 250);
    ledcWrite(1, 250);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(1000);
    ledcWrite(0, 0);
    ledcWrite(1, 250);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(1000);
    ledcWrite(0, 250);
    ledcWrite(1, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(1000);
  }
  
