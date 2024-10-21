""" GPS CODE """
# https://www.google.com/maps/d/u/0/edit?mid=1ZvAEGBiR0pnlL9rKMWukVqyKF_rgs-8&ll=20.339242228402902%2C85.80228273359401&z=16
from datetime import datetime
import csv
import serial

SERIAL_PORT = "COM30"
BAUD_RATE = 9600
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
CSV_FILE = "serial_data.csv"

# Function to read from serial and write to CSV

def read_and_store_data():
    """ Read and Store Data """
    while True:
        if ser.in_waiting > 0:
            try:
                serial_data = ser.readline().decode('utf-8').strip()
                lat, lon = serial_data.split(',')

                with open(CSV_FILE, mode='a', newline='', encoding="utf-8") as file:
                    writer = csv.writer(file)
                    try:    
                        current_data = datetime.now().strftime('%d-%m-%Y')
                        current_time = datetime.now().strftime('%H:%M:%S')
                        writer.writerow([current_data, current_time, lat, lon])
                        print(f"Data saved: {current_data}, {current_time}, {lat}, {lon}")
                    except Exception as error:
                        print(error)
            except Exception as error:
                print(error)

try:
    print("Reading serial data and saving to CSV. Press Ctrl+C to stop.")
    read_and_store_data()
except KeyboardInterrupt:
    print("\nStopped by user.")
finally:
    ser.close()



# $GNRMC,165129.00,V,,,,,,,211024,,,N*6F
# $GNVTG,,,,,,,,,N*2E
# $GNGGA,165129.00,,,,,0,00,99.99,,,,,,*70\
# $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E
# $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E
# $GPGSV,4,1,13,03,14,063,,06,77,217,,11,32,215,,13,11,234,*7F
# $GPGSV,4,2,13,14,54,083,,17,42,017,,19,49,334,,20,06,192,*7B
# $GPGSV,4,3,13,22,60,036,,24,16,310,,30,26,168,,40,48,239,*74
# $GPGSV,4,4,13,41,66,188,*4F
# $GLGSV,2,1,07,74,06,144,,75,56,124,,76,53,349,,77,10,332,*69
# $GLGSV,2,2,07,85,14,026,,86,60,345,,87,46,250,*5E
# $GNGLL,,,,,165129.00,V,N*5C


# $GNRMC,165504.00,A,2014.24927,N,08548.82405,E,0.526,,211024,,,A*6F

# $GNVTG,,T,,M,0.526,N,0.974,K,A*36

# $GNGGA,165504.00,2014.24927,N,08548.82405,E,1,05,1.65,50.1,M,-62.7,M,,*5B

# $GNGSA,A,3,06,14,19,30,11,,,,,,,,3.44,1.65,3.02*14

# $GNGSA,A,3,,,,,,,,,,,,,3.44,1.65,3.02*1C

# $GPGSV,4,1,13,03,13,061,,06,79,222,31,11,33,216,28,13,11,233,*7F

# $GPGSV,4,2,13,14,53,086,21,17,41,019,,19,49,336,17,20,07,192,*72

# $GPGSV,4,3,13,22,61,039,,24,17,309,18,30,24,168,28,40,48,239,*72

# $GPGSV,4,4,13,41,66,188,32*4E

# $GLGSV,2,1,07,74,04,144,,75,54,128,,76,55,349,,77,11,333,*63

# $GLGSV,2,2,07,85,13,027,,86,58,347,,87,47,253,*53

# $GNGLL,2014.24927,N,08548.82405,E,165504.00,A,A*73