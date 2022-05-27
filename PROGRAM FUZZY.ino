#include <Fuzzy.h>
#include <Arduino.h>
#include <analogWrite.h>

//ambil nilai sensor (kiri dan depan)
//alamat sensor kiri
const int LtrigPin = 12;
const int LechoPin = 14;

//alamat sensor depan
const int  FtrigPin = 25;
const int  FechoPin = 33;

//alamat driver
const int  IN1 = 18; // deklarasi pin IN1
const int  IN2 = 5;  // deklarasi pin IN2
const int  IN3 = 4;  // deklarasi pin IN3
const int  IN4 = 2;  // deklarasi pin IN4
const int  ENA = 19; // deklarasi pin ENA
const int  ENB = 15;  // deklarasi pin ENB

//rumus
long duration;
int L;
long Fwaktu;
float F;


//-----------PWM
const int freq = 1000;
const int pwmCh = 0;
const int resolution = 8;
int dutyCycle = 125;
int dutyCycle1;

//Fuzzyfication
Fuzzy* fuzzy = new Fuzzy();
  //input
FuzzySet* dekat     = new FuzzySet(2, 6, 8, 12);
FuzzySet* sedang    = new FuzzySet(10, 16, 18, 24);
FuzzySet* jauh      = new FuzzySet(18, 26, 28, 36);
  //output
FuzzySet* kiri_agak_lambat    = new FuzzySet(2, 6, 9, 13);
FuzzySet* kanan_agak_lambat   = new FuzzySet(2, 6, 9, 13);
FuzzySet* kiri_lambat         = new FuzzySet(9, 13, 16, 20);
FuzzySet* kanan_lambat        = new FuzzySet(9, 13, 16, 20);
FuzzySet* kiri_sedang         = new FuzzySet(16, 20, 23, 27);
FuzzySet* kanan_sedang        = new FuzzySet(16, 20, 23, 27);
FuzzySet* kiri_agak_cepat     = new FuzzySet(23, 27, 30, 34);
FuzzySet* kanan_agak_cepat    = new FuzzySet(23, 27, 30, 34);
FuzzySet* kiri_cepat          = new FuzzySet(30, 34, 37, 41);
FuzzySet* kanan_cepat         = new FuzzySet(30, 34, 37, 41);


void setup() {

  pinMode(LtrigPin, OUTPUT);
  pinMode(LechoPin, INPUT);
  pinMode(FtrigPin, OUTPUT);
  pinMode(FechoPin, INPUT);
  
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

  //input 
  FuzzyInput *fuzz_jarak_L = new FuzzyInput(1);

  fuzz_jarak_L->addFuzzySet(dekat);
  fuzz_jarak_L->addFuzzySet(sedang);
  fuzz_jarak_L->addFuzzySet(jauh);
  fuzzy->addFuzzyInput(fuzz_jarak_L);

  //output
  FuzzyOutput *fuzz_putaran = new FuzzyOutput(1);

  fuzz_putaran->addFuzzySet(kiri_agak_lambat);
  fuzz_putaran->addFuzzySet(kanan_agak_lambat);
  fuzz_putaran->addFuzzySet(kiri_lambat);
  fuzz_putaran->addFuzzySet(kanan_lambat);
  fuzz_putaran->addFuzzySet(kiri_sedang);
  fuzz_putaran->addFuzzySet(kanan_sedang);
  fuzz_putaran->addFuzzySet(kiri_agak_cepat);
  fuzz_putaran->addFuzzySet(kanan_agak_cepat);
  fuzz_putaran->addFuzzySet(kiri_cepat);
  fuzz_putaran->addFuzzySet(kanan_cepat);
  fuzzy->addFuzzyOutput(fuzz_putaran);

  //----rule base----
    //Rule01 , Jika jarak dekat maka motor kiri cepat dan kanan lambat
  FuzzyRuleAntecedent* if_dekat = new FuzzyRuleAntecedent();
  if_dekat->joinSingle(dekat);
  FuzzyRuleConsequent* then_kiri_agak_cepat = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* then_kanan_agak_lambat = new FuzzyRuleConsequent();
  then_kiri_agak_cepat->addOutput(kiri_agak_cepat);
  then_kanan_agak_lambat->addOutput(kanan_agak_lambat);

  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, if_dekat, then_kiri_agak_cepat);
  FuzzyRule* fuzzyRule011 = new FuzzyRule(1, if_dekat, then_kanan_agak_lambat);

    //Rule02 , Jika jarak dekat dan sedang maka motor kiri dan kanan cepat
  FuzzyRuleAntecedent* if_dekat_AND_sedang = new FuzzyRuleAntecedent();
  if_dekat_AND_sedang->joinWithAND(dekat, sedang);
  FuzzyRuleConsequent* then_kiri_cepat = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* then_kanan_cepat = new FuzzyRuleConsequent();
  then_kiri_cepat->addOutput(kiri_cepat);
  then_kanan_cepat->addOutput(kanan_cepat);

  FuzzyRule* fuzzyRule02 = new FuzzyRule(1, if_dekat_AND_sedang, then_kiri_cepat);
  FuzzyRule* fuzzyRule021 = new FuzzyRule(1, if_dekat_AND_sedang, then_kanan_cepat);
  
    //Rule03 , Jika jarak sedang maka motor kiri dan kanan sedang 
  FuzzyRuleAntecedent* if_sedang = new FuzzyRuleAntecedent();
  if_sedang->joinSingle(sedang);
  FuzzyRuleConsequent* then_kiri_sedang = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* then_kanan_sedang = new FuzzyRuleConsequent();
  then_kiri_sedang->addOutput(kiri_sedang);
  then_kanan_sedang->addOutput(kanan_sedang);

  FuzzyRule* fuzzyRule03 = new FuzzyRule(1, if_sedang, then_kiri_sedang);
  FuzzyRule* fuzzyRule031 = new FuzzyRule(1, if_sedang, then_kanan_sedang);

    //Rule04 , Jika jarak sedang dan jauh maka motor kiri lambat dan kanan agak cepat 
  FuzzyRuleAntecedent* if_sedang_AND_jauh = new FuzzyRuleAntecedent();
  if_sedang_AND_jauh->joinWithAND(sedang, jauh);
  FuzzyRuleConsequent* then_kiri_lambat = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* then_kanan_agak_cepat = new FuzzyRuleConsequent();
  then_kiri_lambat->addOutput(kiri_lambat);
  then_kanan_agak_cepat->addOutput(kanan_agak_cepat);

  FuzzyRule* fuzzyRule04 = new FuzzyRule(1, if_sedang_AND_jauh, then_kiri_lambat);
  FuzzyRule* fuzzyRule041 = new FuzzyRule(1, if_sedang_AND_jauh, then_kanan_agak_cepat);

    //Rule05 , Jika jarak jauh maka motor kiri dan kanan sedang 
  FuzzyRuleAntecedent* if_jauh = new FuzzyRuleAntecedent();
  if_jauh->joinSingle(jauh);
  FuzzyRuleConsequent* then_kiri_agak_lambat = new FuzzyRuleConsequent();
  FuzzyRuleConsequent* then_kanan_lambat = new FuzzyRuleConsequent();
  then_kiri_agak_lambat->addOutput(kiri_agak_lambat);
  then_kanan_lambat->addOutput(kanan_lambat);

  FuzzyRule* fuzzyRule05 = new FuzzyRule(1, if_jauh, then_kiri_agak_lambat);
  FuzzyRule* fuzzyRule051 = new FuzzyRule(1, if_jauh, then_kanan_lambat);
  
    //Menambahkan fuzzy Rule
  fuzzy->addFuzzyRule(fuzzyRule01);
  fuzzy->addFuzzyRule(fuzzyRule011);
  fuzzy->addFuzzyRule(fuzzyRule02);
  fuzzy->addFuzzyRule(fuzzyRule021);
  fuzzy->addFuzzyRule(fuzzyRule03);
  fuzzy->addFuzzyRule(fuzzyRule031);
  fuzzy->addFuzzyRule(fuzzyRule04);
  fuzzy->addFuzzyRule(fuzzyRule041);
  fuzzy->addFuzzyRule(fuzzyRule05);
  fuzzy->addFuzzyRule(fuzzyRule051);
  
Serial.begin(9600);

}

void loop() {
  distance();
  
  if (L >= 6 && L <= 12) {
    int input_L = L;
    fuzzy->setInput(1, input_L);
    fuzzy->fuzzify();
    float output = fuzzy->defuzzify(1);
    Serial.print("output: ");
    Serial.println(output);
    dutyCycle = output + 100;
    dutyCycle1 = output + 160;
    ledcWrite(0, dutyCycle);
    ledcWrite(1, dutyCycle1);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    
  }/*
  if (L >= 6 && L <= 12) {
    ledcWrite(0, 100);
    ledcWrite(1, 170);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    
  }*/
  
  if (L > 12 && L <= 24) {//sedang
    int input_L = L;
    fuzzy->setInput(1, input_L);
    fuzzy->fuzzify();
    float output = fuzzy->defuzzify(1);
    Serial.print("output: ");
    Serial.println(output);
    dutyCycle = output+120;
    ledcWrite(0, dutyCycle);
    ledcWrite(1, dutyCycle);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  if (L > 24 && L <= 36) {
    int input_L = L;
    fuzzy->setInput(1, input_L);
    fuzzy->fuzzify();
    float output = fuzzy->defuzzify(1);
    Serial.print("output: ");
    Serial.println(output);
    dutyCycle = output+140;
    dutyCycle1 = output+100;
    ledcWrite(0, dutyCycle);
    ledcWrite(1, dutyCycle1);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  if (F <= 35 ) {
    ledcWrite(0, 160);
    ledcWrite(1, 160);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  Serial.println(dutyCycle);
}
void distance(){
  digitalWrite(LtrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(LtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(LtrigPin, LOW);
  duration = pulseIn(LechoPin, HIGH);
  L = duration / 58.2;
  Serial.print("Jarak L: ");
  Serial.print(L);
  Serial.println(" cm");

  digitalWrite(FtrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(FtrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(FtrigPin, LOW);
  Fwaktu = pulseIn(FechoPin, HIGH);
  F = Fwaktu / 58.2;
  Serial.print("Jarak F : ");
  Serial.print(F);
  Serial.println(" cm");
}
