""" GPS data collector code """

import struct
import csv
from pyrf24 import RF24, RF24Network, RF24Mesh, RF24_PA_MIN
radio = RF24(22, 0)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)
mesh.setNodeID(0)
radio.begin()
radio.setPALevel(RF24_PA_MIN, 0)

CSV_FILE = "/home/csmiot/gps_data.csv"

CURRENT_MILLIS = 0

if not mesh.begin():
    raise OSError("Radio hardware not responding.")

radio.printDetails()
def parse_truck_data(this_payload):
    """ After getting data from NRF, decode data"""
    this_truck_id = int.from_bytes(this_payload[0:2], byteorder='little')
    this_lat, this_lon = struct.unpack('<ff', this_payload[2:10])
    return this_truck_id, this_lat, this_lon

try:
    while True:
        mesh.update()
        mesh.DHCP()
        # if time.time() - CURRENT_MILLIS > 5:
        #     CURRENT_MILLIS = time.time()
        #     for node in mesh.addrList:
        #         print(f"NodeID: {node.nodeID}, RF24Network Address: 0{node.address:#o}")

        if network.available():
            header, payload = network.read(32)
            current_channel = radio.getChannel()
            truck_id, lat, lon = parse_truck_data(payload)
            this_data = f"Truck ID: {truck_id}, Latitude: {lat}, Longitude: {lon}"
            print(this_data)
            try:
                with open(CSV_FILE, mode='a', newline='', encoding="utf-8") as file:
                    writer = csv.writer(file)
                    writer.writerow([truck_id, lat, lon])
            except Exception as error:
                print(error)
except KeyboardInterrupt:
    print("Powering down radio and exiting.")
    radio.powerDown()
