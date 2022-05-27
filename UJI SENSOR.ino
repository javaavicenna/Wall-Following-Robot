//ambil nilai sensor (kiri dan depan)
//alamat sensor kiri
const int LtrigPin = 12;
const int LechoPin = 14;

//alamat sensor depan
const int  FtrigPin = 25;
const int  FechoPin = 33;



//rumus
long duration;
int L;
long Fwaktu;
float F;

void setup() {

  pinMode(LtrigPin, OUTPUT);
  pinMode(LechoPin, INPUT);
  pinMode(FtrigPin, OUTPUT);
  pinMode(FechoPin, INPUT);
  
  Serial.begin(9600);

}

void loop() {
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
