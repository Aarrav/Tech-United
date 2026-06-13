#!/usr/bin/env python3
"""
UDP Receiver for ESP32 Voltage Data with Real-time Graph
Receives voltage readings and displays in real-time with moving 4-second window
"""

import socket
import sys
from datetime import datetime
from collections import deque
import threading
import matplotlib.pyplot as plt
import matplotlib.animation as animation

class VoltageReceiver:
    def __init__(self, port=5005):
        self.port = port
        self.data_buffer = deque(maxlen=500)  # Keep last 500 points (roughly 15 seconds at 30ms)
        self.running = True

    def receive_data(self):
        """Listen for UDP packets in a separate thread"""
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind(('0.0.0.0', self.port))
        print(f"Listening for voltage data on UDP port {self.port}...")
        print("Close the graph window to stop.\n")

        try:
            while self.running:
                data, _ = sock.recvfrom(1024)
                packet = data.decode('utf-8').strip()

                try:
                    # Parse format: "timestamp_ms,voltage"
                    parts = packet.split(',')
                    if len(parts) == 2:
                        esp_timestamp_ms = int(parts[0])
                        voltage = float(parts[1])

                        # Store data with local timestamp for graphing
                        local_timestamp = datetime.now()
                        self.data_buffer.append({
                            'local_time': local_timestamp,
                            'esp_ms': esp_timestamp_ms,
                            'voltage': voltage
                        })

                        # Print with ms precision
                        print(f"{local_timestamp.strftime('%H:%M:%S.%f')[:-3]} | ESP: {esp_timestamp_ms:>8}ms | Voltage: {voltage:>7.2f}V")
                except ValueError:
                    print(f"Invalid data: {packet}")
        except KeyboardInterrupt:
            pass
        finally:
            sock.close()
            self.running = False

    def update_graph(self, _):
        """Update graph with latest data"""
        ax.clear()

        if len(self.data_buffer) == 0:
            ax.set_title("Voltage Monitor - Waiting for data...")
            ax.set_ylabel("Voltage (V)")
            ax.set_xlabel("Time (s)")
            return

        # Get current time
        now = datetime.now()

        # Filter data for last 4 seconds
        times = []
        voltages = []

        for point in self.data_buffer:
            time_diff = (now - point['local_time']).total_seconds()
            if time_diff <= 4.0:  # Keep last 4 seconds
                times.append(time_diff)
                voltages.append(point['voltage'])

        if len(times) > 0:
            # Plot the data
            ax.plot(times, voltages, 'b-', linewidth=2, marker='o', markersize=3)
            ax.set_xlim(4, 0)  # Reverse x-axis so newest data is on right
            ax.set_ylim(min(voltages) - 1, max(voltages) + 1)
            ax.set_ylabel("Voltage (V)", fontsize=12)
            ax.set_xlabel("Time (seconds ago)", fontsize=12)
            ax.grid(True, alpha=0.3)
            ax.set_title(f"Voltage Monitor - Last 4 seconds | Latest: {voltages[-1]:.2f}V", fontsize=14, fontweight='bold')
        else:
            ax.set_title("Voltage Monitor - Waiting for data...")
            ax.set_ylabel("Voltage (V)")
            ax.set_xlabel("Time (s)")

def main():
    global ax

    port = int(sys.argv[1]) if len(sys.argv) > 1 else 5005
    receiver = VoltageReceiver(port)

    # Start receiving data in background thread
    receive_thread = threading.Thread(target=receiver.receive_data, daemon=True)
    receive_thread.start()

    # Setup matplotlib figure
    fig, ax = plt.subplots(figsize=(12, 6))
    fig.suptitle("ESP32 Voltage Logger - Real-time Monitor", fontsize=16, fontweight='bold')

    # Create animation
    _ = animation.FuncAnimation(
        fig, receiver.update_graph,
        interval=100,  # Update every 100ms
        blit=False,
        cache_frame_data=False
    )

    plt.tight_layout()
    plt.show()

    receiver.running = False

if __name__ == "__main__":
    main()
