
// Pin Configuration for NRF24L01
#define truck_no 5
#define NRF_CE_PIN 9
#define NRF_CSN_PIN 10


// Pin Configuration for GPS Module
#define RX 4
#define TX 3
#define GPS_BAUD 9600
#define truck_send_data_time 1000
long gps_print_timeout = 0;
double latitude_data = 0;
double longitude_data = 0;

// Importing all Libraries
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);  // Configure NR24L01 Pins
RF24Network network(radio);           // Initiate communication with module
RF24Mesh mesh(radio, network);        // Start mesh communication

SoftwareSerial gps_serial(RX, TX);  // SoftSerial with GPS module
TinyGPSPlus gps;                    // Initiate GPS Class

// Structure to maintain GPS data and transmit to hub node
struct TruckData {
  int truck_id;
  double lat;
  double lon;
};

void setup() {
  // Serial Defines
  Serial.begin(9600);          // Serial debugging BAUD
  gps_serial.begin(GPS_BAUD);  // GPS Data read BAUD

  // NRF Defines
  mesh.setNodeID(truck_no);          // Start mesh network with truck ID
  radio.begin();                     // Initiate RADIO COmmunication
  radio.setPALevel(RF24_PA_MAX, 0);  // Set Power COnsumption to max

  // Connect to hub mesh
  Serial.println(F("Connecting to the mesh..."));
  if (!mesh.begin()) {
    if (radio.isChipConnected()) {
      do {
      } while (mesh.renewAddress() == MESH_DEFAULT_ADDRESS);
    } else {
      while (1) {}
    }
  }
}

void loop() {
  mesh.update();  // Update current mesh network

  while (gps_serial.available() > 0) {
    char gpsData = gps_serial.read();  // Read each data
    if (gps.encode(gpsData)) {
      if (gps.location.isValid()) {
        if (millis() - gps_print_timeout > truck_send_data_time) {
          gps_print_timeout = millis();

          // Prepare truck dats
          TruckData truck;
          truck.truck_id = truck_no;
          truck.lat = gps.location.lat();
          truck.lon = gps.location.lng();

          // Start Sending truck data with 'T' type message
          bool msg_sts = mesh.write(&truck, 'T', sizeof(truck));

          // If message sending failed
          if (!msg_sts) {
            // Restart MESH Communication
            if (!mesh.checkConnection()) {
              if (mesh.renewAddress() == MESH_DEFAULT_ADDRESS) {
                mesh.begin();
              }
            }
          }

          else {
            Serial.print("Send OK: Truck ID: ");
            Serial.println(truck.truck_id);
          }
        }
      }
    }
  }
}
