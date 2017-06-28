# Non Blocking HX711 Arduino Library

Copyright (c) 2017 Whandall (whandall@gmx.de), released under the MIT license.  
See the LICENSE file for licensing details.

A little more advanced Arduino driver for the HX711 ADC.

The HX711 is a low-cost strain gauge amplifier produced by Avia Semiconductor.

The HX711 communicates with a non-i2c compliant two wire protocol.

This library provides the code required to use an Arduino, the HX711 module and a strain gauge load cell to build a scale, force gauge or other pressure or force sensitive projects.

The library has a single class, **NBHX711** with some functions.

Readings will be gathered when available via the update function, making the whole process non-blocking

## Class
The **NBHX711** class takes four parameters on construction, the depth of the sample buffer, the pin to use for data (output), the pin to use to signal readiness (clock) and an optional gain/channel.

## Functions

Function  | Description
------------- | -------------
**begin** | Sets up the hardware
**read**  | Returns a long integer that is the current value of the HX711.
**update** | Checks for new sample, if available read it into the sample buffer.

## Example

Here is a simple example of using the HX711 on pins A2 and A3 to read a strain gauge and print it's current value:

```c++
#include <NBHX711.h>
NBHX711 hx711(A2, A3);
void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long lastRead;
  hx711.update();
  if (millis() - lastRead >= 500) {
    lastRead = millis();
    Serial.println(hx711.read());
  }
}
```
