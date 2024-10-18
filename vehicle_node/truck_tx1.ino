#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define node_id 011
#define truck_id 1
RF24 radio(9, 10);           // CE, CSN pins
RF24Network network(radio);  // Network using the radio
const uint16_t this_node = node_id;
const int broadcast_nodes_qty = 1;
const uint16_t broadcast_node[broadcast_nodes_qty] = { 02 };  // Broadcast to all nearby nodes

struct Payload {
  int node;
  int truckID;
  float latitude;
  float longitude;
};

void setup() {
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  network.begin(89, this_node);  // Channel 90
}

bool isChannelClear() {
  // Check if the channel is busy using testRPD (Clear Channel Assessment)
  radio.startListening();
  delayMicroseconds(128);  // Give it some time to detect any signal
  bool busy = radio.testRPD();  // true if RF signal detected (channel is busy)
  radio.stopListening();
  return !busy;  // Returns true if the channel is clear
  // return true;
}

void sendData(Payload data, int x) {
  RF24NetworkHeader header(broadcast_node[x]);  // Broadcast header
  int retries = 3;                              // Number of retries
  bool success = false;

  while (retries > 0 && !success) {
    // CSMA/CA-like check: wait if the channel is busy
    if (isChannelClear()) {
      success = network.write(header, &data, sizeof(data));

      if (success) {

        Serial.println("Truck data broadcasted successfully: " + String(broadcast_node[x]));
      }
    } else {
      Serial.println("Channel busy. Waiting...");
    }

    delay(random(50, 150));  // Random backoff delay
    retries--;
  }

  if (!success) {
    Serial.println("Failed to send data after retries.");
  }
}

void loop() {
  network.update();  // Keep the network updated
  for (int x = 0; x < broadcast_nodes_qty; x++) {

    // Randomize data for simulation
    Payload data;
    data.node = int(broadcast_node[x]);
    data.truckID = truck_id;                         // Unique truck ID (101-140)
    data.latitude = random(123456, 1234567) / 10000.0;   // Simulated latitude
    data.longitude = random(789012, 7890123) / 10000.0;  // Simulated longitude

    // Try sending data
    sendData(data, x);

    delay(random(1500, 2500));  // Random delay between 1.5 and 2.5 seconds to avoid collision
  }
}
