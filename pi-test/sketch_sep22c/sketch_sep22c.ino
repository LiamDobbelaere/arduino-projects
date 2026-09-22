void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLDOWN);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);      // GP3 now a 3.3V source
}
void loop() {
  digitalWrite(LED_BUILTIN, digitalRead(2));
}