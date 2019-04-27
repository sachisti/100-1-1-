#define PIN 13

int read;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

void loop() {
  read = analogRead(A0);
  if (read >= 500) {
    digitalWrite(PIN, HIGH);
  }
  else {
    digitalWrite(PIN, LOW);
  }

}
