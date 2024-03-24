#define PIN 19

void setup() {
    Serial.begin(115200);
    pinMode(PIN, INPUT_PULLUP);
}

void loop() {
  int button = digitalRead(PIN);
  Serial.printf("Tlaciko = %d\n", button);
  delay(200);
}
