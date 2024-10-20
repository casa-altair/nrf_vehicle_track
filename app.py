import struct
from pyrf24 import RF24, RF24Network, RF24Mesh, RF24_PA_MIN
radio = RF24(22, 0)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)
mesh.setNodeID(0)
radio.begin()
radio.setPALevel(RF24_PA_MIN, 0)

if not mesh.begin():
    raise OSError("Radio hardware not responding.")

radio.printDetails()
def parse_truck_data(payload):
    truck_id = int.from_bytes(payload[0:2], byteorder='little')
    lat, lon = struct.unpack('<ff', payload[2:10])
    return truck_id, lat, lon

try:
    while True:
        mesh.update()
        mesh.DHCP()
        while network.available():
            header, payload = network.read(32)
            print(f"Received raw data (length {len(payload)}): {payload}")
            truck_id, lat, lon = parse_truck_data(payload)
            print(f"Truck ID: {truck_id}, Latitude: {lat}, Longitude: {lon}")

except KeyboardInterrupt:
    print("Powering down radio and exiting.")
    radio.powerDown()