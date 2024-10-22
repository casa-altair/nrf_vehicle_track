#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
long current_timer = 0;
#define RX 4
#define TX 3
#define GPS_BAUD 9600
SoftwareSerial gps_serial(RX, TX);
TinyGPSPlus gps;
void displayInfo();
void setup() {
  Serial.begin(9600);
  gps_serial.begin(GPS_BAUD);
}

void loop() {
  while (gps_serial.available() > 0) {
    char gpsData = gps_serial.read();
    if (gps.encode(gpsData)) {
      displayInfo();
    }
  }
}

void displayInfo() {
  if (gps.location.isValid()) {
    if (millis() - current_timer > 1000) {
      current_timer = millis();
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.print(gps.date.year());
      Serial.print(",");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.print(gps.time.second());
      Serial.print(",");
      Serial.print(gps.location.lat(), 6);
      Serial.print(",");
      Serial.println(gps.location.lng(), 6);
    }
  }
}

// 21/10/2024,17:0:14,20.237613,85.813781
// 21/10/2024,17:0:15,20.237615,85.813789
// 21/10/2024,17:0:17,20.237615,85.813789
