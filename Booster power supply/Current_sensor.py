import requests
import time

ESP32_IP = "10.34.233.39"  # <-- replace with your ESP32 IP
URL = f"http://{ESP32_IP}/"

POLL_INTERVAL = 0.05  # 50 ms → 20 Hz

while True:
    start = time.time()

    try:
        response = requests.get(URL, timeout=0.1)
        current = float(response.text.strip())
        print(current)
    except Exception as e:
        #print("Error:", e)
        continue

    elapsed = time.time() - start
    sleep_time = max(0, POLL_INTERVAL - elapsed)
    time.sleep(sleep_time)
    #print(f"{1/elapsed:.1f} Hz")
'''
import requests
import time
import matplotlib.pyplot as plt
from collections import deque

ESP32_IP = "10.34.233.39"
URL = f"http://{ESP32_IP}/"
POLL_INTERVAL = 0.05  # 20 Hz

# Keep last N samples (e.g. 10 seconds)
MAX_POINTS = 200  

times = deque(maxlen=MAX_POINTS)
currents = deque(maxlen=MAX_POINTS)

start_time = time.time()

plt.ion()
fig, ax = plt.subplots()
line, = ax.plot([], [])
ax.set_xlabel("Time (s)")
ax.set_ylabel("Current (A)")
ax.set_title("ESP32 Current Measurement")
ax.grid(True)

while True:
    loop_start = time.time()

    try:
        r = requests.get(URL, timeout=0.1)
        current = float(r.text.strip())
        t = time.time() - start_time

        times.append(t)
        currents.append(current)

        line.set_xdata(times)
        line.set_ydata(currents)

        ax.relim()
        ax.autoscale_view()

        plt.pause(0.001)

        print(current)

    except Exception:
        pass  # ignore dropped packets

    elapsed = time.time() - loop_start
    time.sleep(max(0, POLL_INTERVAL - elapsed))
'''