# Vehicle Tracking system
Vehicle Tracking System using NRF24L01 RFs as mesh communication
- This project is about developing a mesh network with NRF24L01a and Arduino.
- Arduino and NRF24L01 will work as truck and range extender clients
- Raspberrypi with NRF24L01 will be the main HUB

# File Documentation

```http
  File: Master_Mesh.ino
```
- This file contains Mesh's Hub using Arduino UNO, for debugging purpose.
- This will print the Truck ID,Latitude and Longitude.
- This will also show us the channel number inwhich that clientnode is sending.

```http
  File: Vehicle_node.ino
```
- This file contains Mesh's Client using Arduino UNO.
- This will automatically connect to mesh.
- If there is some disconnection, this code, will auto-connect with different channel or node.
- This application will only send data via mesh to Hub Node.

| Version | Description                                              |
| :-----: | :------------------------------------------------------- |
| 1.0     | Developed application for both hub and client on arduino |
| 1.1     | Developed application for python based data receive      |
