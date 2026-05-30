#include <iostream>
#include <cstring>
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    const char* UDP_IP = "0.0.0.0";
    const int UDP_PORT = 5005;

    // 1. Create a UDP socket (AF_INET, SOCK_DGRAM)
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // 2. Bind the socket to the port and IP
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    inet_pton(AF_INET, UDP_IP, &server_addr.sin_addr);

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed\n";
        close(sock);
        return 1;
    }

    std::cout << "Listening for timed pressure data on port " << UDP_PORT << "...\n";

    // Struct format equivalent: uint32_t (4 bytes) + float (4 bytes) = 8 bytes
    // Note: Most modern architectures are little-endian by default, matching '<If'
    const size_t EXPECTED_SIZE = 8; 
    char buffer[1024];

    // Variables to track state
    bool has_last_timestamp = false;
    uint32_t last_timestamp_ms = 0;

    // 3. Main receive loop
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        // Receive data
        ssize_t bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, 
                                          (struct sockaddr*)&client_addr, &client_len);

        if (bytes_received < 0) {
            std::cerr << "Error receiving data\n";
            break;
        }

        // Check if we received exactly 8 bytes
        if (static_cast<size_t>(bytes_received) == EXPECTED_SIZE) {
            uint32_t timestamp_ms;
            float pressure;

            // Unpack data cleanly using memcpy to avoid alignment/aliasing issues
            std::memcpy(&timestamp_ms, buffer, sizeof(timestamp_ms));
            std::memcpy(&pressure, buffer + sizeof(timestamp_ms), sizeof(pressure));

            // Calculate time difference (dt)
            uint32_t dt_ms = 0;
            if (has_last_timestamp) {
                dt_ms = timestamp_ms - last_timestamp_ms;
            } else {
                has_last_timestamp = true;
            }

            // Print the data
            // printf handles float formatting smoothly like python's :.2f
            std::printf("Time: %u ms (+%u ms) | Pressure: %.2f Pa\n", 
                        timestamp_ms, dt_ms, pressure);

            // Save current timestamp for next iteration
            last_timestamp_ms = timestamp_ms;
        } else {
            std::cout << "Received malformed packet of size " << bytes_received << "\n";
        }
    }

    // 4. Clean up
    close(sock);
    return 0;
}