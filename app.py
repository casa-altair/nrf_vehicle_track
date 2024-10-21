import struct
import time
import csv
from pyrf24 import RF24, RF24Network, RF24Mesh, RF24_PA_MIN
radio = RF24(22, 0)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)
mesh.setNodeID(0)
radio.begin()
radio.setPALevel(RF24_PA_MIN, 0)

CSV_FILE = "/home/casaaltair/nrf24l01_com/gps_data.csv"

current_millis = 0;

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
        if time.time() - current_millis > 1.5:
            current_millis = time.time()
            for node in mesh.addrList:
                print(f"NodeID: {node.nodeID}, RF24Network Address: 0{node.address:#o}")

        if network.available():
            header, payload = network.read(32)
            # channel_no = header.channel
            current_channel = radio.getChannel()
            truck_id, lat, lon = parse_truck_data(payload)
            this_data = f"Truck ID: {truck_id}, Latitude: {lat}, Longitude: {lon}"
            print("====", this_data)
            
            with open(CSV_FILE, mode='a', newline='', encoding="utf-8") as file:
                    try:
                        writer = csv.writer(file)
                        writer.writerow([truck_id, lat, lon])
                    except Exception as error:
                        print(error)
except KeyboardInterrupt:
    print("Powering down radio and exiting.")
    radio.powerDown()