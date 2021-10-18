#include <Arduino.h>
#include "ArrayCircularBuffer.h"

ArrayCircularBuffer eventBuffer;

void eventCallback(int event) {
  eventBuffer.write(event);
}

void setup() {
  Serial.begin(9600);
  Serial.println("hello");
    eventBuffer.write(1);
    eventBuffer.write(2);
    eventBuffer.write(3);
}

void loop() { 
  if(!eventBuffer.isEmpty()) {
    Serial.print(eventBuffer.read());
    Serial.println(" was stored");
  }
}