'''
import asyncio
from bleak import BleakClient

ADDRESS = "D0D00B06-F7B7-4C5E-E532-102652C8461A"  # your Nano BLE address
CHAR_UUID = "2A56"

def callback(sender, data):
    import struct
    pressure = struct.unpack('f', data)[0]
    print(pressure)

async def main():
    async with BleakClient(ADDRESS) as client:
        await client.start_notify(CHAR_UUID, callback)
        await asyncio.sleep(100)

asyncio.run(main())
'''
import asyncio
import struct
import csv
import os
import time
from bleak import BleakClient

ADDRESS = "D0D00B06-F7B7-4C5E-E532-102652C8461A"
CHAR_UUID = "2A56"
FILE_NAME = "pressure_data.csv"

# Clear old data on start
if os.path.exists(FILE_NAME):
    os.remove(FILE_NAME)

def callback(sender, data):
    # Get current high-precision timestamp
    current_time = time.time()
    pressure = struct.unpack('f', data)[0]
    
    # Log: [timestamp, pressure_value]
    with open(FILE_NAME, 'a', newline='') as f:
        writer = csv.writer(f)
        writer.writerow([current_time, pressure])
    
    print(f"Time: {current_time:.3f} | Pressure: {pressure:.2f}")

async def main():
    print(f"Connecting to {ADDRESS}...")
    try:
        async with BleakClient(ADDRESS) as client:
            print("Connected! Logging to CSV. Press Ctrl+C to stop.")
            await client.start_notify(CHAR_UUID, callback)
            
            while True:
                await asyncio.sleep(1)
    except Exception as e:
        print(f"Connection failed: {e}")

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nLogging stopped.")