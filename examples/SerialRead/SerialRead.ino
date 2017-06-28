#include <Bounce2.h>
#include <NBHX711.h>

NBHX711 scale(A2, A3, 20);
Bounce tara;

void setup() {
  tara.attach(8, INPUT_PULLUP);
  Serial.begin(250000);
  Serial.println(F("NBHX711 Demo"));
  scale.begin();
  scale.setScale(6599.15);
}

void loop() {
  static float lastValue;
  static bool firstTime = true;
  static byte initialCount = 0;
  //static unsigned long lastReading;
  //unsigned long topLoop = millis();
  if (tara.update() && tara.fell()) {
    Serial.println(F("tare..."));
    scale.tare();
  }
  if (scale.update()) {
    if (firstTime) {
      if (++initialCount > 5) {
        scale.tare();
        firstTime = false;
      }
    } else {
      //	if (topLoop - lastReading >= 1000) {
      //		lastReading = topLoop;
      float thisValue = scale.getUnits();
      if (abs(lastValue - thisValue) >= 0.01) {
        Serial.print(scale.getUnits(), 2);
        Serial.write(' ');
        Serial.print(scale.getUnits(5), 2);
        Serial.write(' ');
        Serial.print(scale.getUnits(10), 2);
        Serial.write(' ');
        Serial.print(scale.getUnits(15), 2);
        Serial.write(' ');
        Serial.print(scale.getUnits(20), 2);
        Serial.println();
      }
      lastValue = thisValue;
    }
  }
}