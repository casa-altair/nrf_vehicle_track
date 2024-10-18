#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(9, 10);              // CE, CSN pins
RF24Network network(radio);     // Network using the radio
const uint16_t this_node = 02;  // Intermediate node address

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
  network.begin(89, this_node);    // Ensure same channel
  radio.setPALevel(RF24_PA_HIGH);  // Increase power level if necessary

  Serial.println("Intermediate node initialized.");
}

void loop() {
  network.update();  // Regularly update the network

  if (network.available()) {
    Serial.println("Data available!");

    RF24NetworkHeader header;                   // Create a header to read data
    Payload data;                               // Create a structure to hold the received data
    network.read(header, &data, sizeof(data));  // Read the received data into 'data'

    // Log the received data
    Serial.print("V: ");
    Serial.print(data.truckID);
    Serial.print("|");
    Serial.print(data.node);
    Serial.print("|");
    Serial.print(data.latitude, 6);  // Print float with 6 decimal places
    Serial.print("|");
    Serial.println(data.longitude, 6);

    // Determine if the data should be relayed to another node (e.g., master node)
    // This part depends on your system design and routing strategy
  }

  delay(500);  // Delay to avoid spamming the serial monitor
}
