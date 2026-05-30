import socket
import struct

UDP_IP = "0.0.0.0"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for timed pressure data on port {UDP_PORT}...")

# The struct format: '<' (little-endian), 'I' (uint32), 'f' (float)
struct_format = '<If'
expected_size = struct.calcsize(struct_format) # Should be 8 bytes

# Keep track of the previous timestamp
last_timestamp_ms = None

try:
    while True:
        data, addr = sock.recvfrom(1024)
        
        # Check if we received exactly the 8 bytes we expect
        if len(data) == expected_size:
            # Unpack the two values
            timestamp_ms, pressure = struct.unpack(struct_format, data)
            
            # Calculate the time difference (dt)
            if last_timestamp_ms is not None:
                dt_ms = timestamp_ms - last_timestamp_ms
            else:
                dt_ms = 0 # First sample has no previous time
            
            # Print the data, including the difference (+XX ms)
            print(f"Time: {timestamp_ms} ms (+{dt_ms} ms) | Pressure: {pressure:.2f} Pa")
            
            # Save the current timestamp for the next loop
            last_timestamp_ms = timestamp_ms
            
        else:
            print(f"Received malformed packet of size {len(data)}")
            
except KeyboardInterrupt:
    print("\nClosing receiver.")
    sock.close()