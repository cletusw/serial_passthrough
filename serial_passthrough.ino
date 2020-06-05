uint32_t currentBaud = 9600;

void setup() {
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  currentBaud = Serial.baud();
  Serial1.begin(currentBaud);
}

void loop() {
  // Check for baud changes on the USB CDC side
  // TODO: Instead of polling, upstream something like LUFA's EVENT_CDC_Device_LineEncodingChanged
  // (@see https://github.com/abcminiuser/lufa/blob/597fbf47cd2551423a231ac747e2f1405cf9306a/LUFA/Drivers/USB/Class/Device/CDCClassDevice.c#L86)
  // in Arduino's CDC library:
  // (@see https://github.com/arduino/ArduinoCore-avr/blob/58081c05e548560d3a60050bbf260ebec5d1e867/cores/arduino/CDC.cpp#L94)
  if (currentBaud != Serial.baud()) {
    currentBaud = Serial.baud();
    Serial1.flush();
    Serial1.begin(currentBaud);
  }

  if (Serial.available()) {
    Serial1.write(Serial.read()); // USB -> D1/pin21/TX
  }

  if (Serial1.available()) {
    Serial.write(Serial1.read()); // D0/pin20/RX -> USB
  }
}
