#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>

/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(9, 10);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

// Truck node ID (change this for each truck)
#define truck_no 1

// Structure to hold the truck data
struct TruckData {
  int truck_id;
  float lat;
  float lon;
};

uint32_t displayTimer = 0;

void setup() {
  Serial.begin(115200);

  // Set the nodeID manually for each truck
  mesh.setNodeID(truck_no);

  // Set the PA Level to MAX and disable LNA for power supply related issues
  radio.begin();
  radio.setPALevel(RF24_PA_MAX, 0);

  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  if (!mesh.begin()) {
    if (radio.isChipConnected()) {
      do {
        // mesh.renewAddress() will return MESH_DEFAULT_ADDRESS on failure to connect
        Serial.println(F("Could not connect to network.\nConnecting to the mesh..."));
      } while (mesh.renewAddress() == MESH_DEFAULT_ADDRESS);
    } else {
      Serial.println(F("Radio hardware not responding."));
      while (1) {
        // hold in an infinite loop
      }
    }
  }
  Serial.println(F("Truck node started"));
}

void loop() {
  mesh.update();

  // Prepare truck data
  TruckData truck;
  truck.truck_id = truck_no;  // Unique truck ID
  truck.lat = 37.7749;        // Example latitude (replace with actual GPS data)
  truck.lon = -122.4194;      // Example longitude (replace with actual GPS data)

  // Send truck data to the master node every second
  if (millis() - displayTimer >= 1000) {
    displayTimer = millis();

    // Send an 'T' type message containing the truck data
    if (!mesh.write(&truck, 'T', sizeof(truck))) {

      // If a write fails, check connectivity to the mesh network
      if (!mesh.checkConnection()) {
        Serial.println("Renewing Address");
        if (mesh.renewAddress() == MESH_DEFAULT_ADDRESS) {
          mesh.begin();  // If address renewal fails, reconfigure the radio and restart the mesh
        }
      } else {
        Serial.println("Send fail, Test OK");
      }
    } else {
      Serial.print("Send OK: Truck ID: ");
      Serial.println(truck.truck_id);
    }
  }
}
