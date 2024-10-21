import serial
import time
import struct


# CRC Calculation Function
def compute_crc(data):
    crc = 0xFFFF
    for pos in data:
        crc ^= pos
        for _ in range(8):
            if (crc & 1) != 0:
                crc >>= 1
                crc ^= 0xA001
            else:
                crc >>= 1
    return crc


# Initialize Serial Communication
ser = serial.Serial(
    port='COM4',  # Adjust COM port as needed
    baudrate=4800,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=1
)

# Modbus RTU Communication Parameters
address_code = 0x01  # Device address (default 0x01)
function_code = 0x03  # Function code (0x03 - Read register)
register_start_address = 0x0000  # Register start address 000H
register_length = 0x0001  # Reading 1 register (2 bytes)

# Query Frame Construction (Modbus RTU Format)
query_frame = struct.pack('>B B H H', address_code, function_code, register_start_address, register_length)

# Compute CRC for Query Frame
crc = compute_crc(query_frame)
query_frame += struct.pack('<H', crc)  # Append CRC low and high bytes

# Continuous Loop for Data Reading
while True:
    ser.write(query_frame)  # Send the query frame to the slave
    time.sleep(1)

    # Read and decode the response frame (7 bytes expected)
    response = ser.read(7)

    if len(response) == 7:
        # Check CRC
        received_crc = struct.unpack('<H', response[-2:])[0]
        calculated_crc = compute_crc(response[:-2])

        if received_crc == calculated_crc:
            # Extract the data from the response frame
            high_byte, low_byte = response[3], response[4]
            total_solar_radiation = (high_byte << 8) | low_byte  # Combine high and low byte

            # Print the read values to the command window
            print(f"Total Solar Irradiation: {total_solar_radiation} W/m^2")
        else:
            print('CRC check failed.')

    else:
        print('Failed to read response.')

    time.sleep(1)

# Close serial connection (this will only execute if the loop is stopped)
ser.close()
