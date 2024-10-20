#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>

/***** Configure the chosen CE, CS pins *****/
RF24 radio(9, 10);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

// Structure to hold the received data (truck_id, latitude, longitude)
struct TruckData {
  int truck_id;
  float lat;
  float lon;
};

uint32_t displayTimer = 0;

void setup() {
  Serial.begin(115200);

  // Set the nodeID to 0 for the master node
  mesh.setNodeID(0);

  // Set the PA Level to MIN and disable LNA for testing & power supply related issues
  radio.begin();
  radio.setPALevel(RF24_PA_MIN, 0);

  // Connect to the mesh
  if (!mesh.begin()) {
    Serial.println(F("Radio hardware not responding."));
    while (1);
  }
  Serial.println(F("Master node started"));
}

void loop() {
  // Call mesh.update to keep the network updated
  mesh.update();

  // DHCP service to assign addresses to sensor nodes
  mesh.DHCP();

  // Check for incoming data from the nodes
  if (network.available()) {
    RF24NetworkHeader header;
    TruckData truck;

    // Check the type of the incoming message
    network.peek(header);
    if (header.type == 'T') { // 'T' means TruckData message
      network.read(header, &truck, sizeof(truck));
      
      // Print truck data
      Serial.print("Truck ID: ");
      Serial.println(truck.truck_id);
      Serial.print("Latitude: ");
      Serial.println(truck.lat, 6);
      Serial.print("Longitude: ");
      Serial.println(truck.lon, 6);
    }
  }

  // Display the mesh addresses every 5 seconds
  if (millis() - displayTimer > 5000) {
    displayTimer = millis();
    Serial.println(" ");
    Serial.println(F("********Assigned Addresses********"));
    for (int i = 0; i < mesh.addrListTop; i++) {
      Serial.print("NodeID: ");
      Serial.print(mesh.addrList[i].nodeID);
      Serial.print(" RF24Network Address: 0");
      Serial.println(mesh.addrList[i].address, OCT);
    }
    Serial.println(F("**********************************"));
  }
}
