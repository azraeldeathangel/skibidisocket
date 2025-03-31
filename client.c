#include <stdio.h>
#include <winsock2.h>    // Include the Winsock library for networking
#include <ws2tcpip.h>    // Additional functions for Winsock
#pragma comment (lib, "Ws2_32.lib") // Link against the Winsock library

int main(void) {
    SOCKET client_socket;                   // Declare a socket
    WSADATA wsastructure;                   // Structure to store Winsock data
    int result;                             // Variable to store function results
    struct sockaddr_in client_address;      // Structure for socket address
    char senddata[1024] = "Hello World";    // Data to be sent
    char recvdata[1024];                    // Buffer to store received data

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2,2), &wsastructure);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 6); // Using TCP (SOCK_STREAM)

    if (client_socket == INVALID_SOCKET) {
        printf("socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Define server address
    client_address.sin_family = AF_INET;                                // IPv4
    client_address.sin_port = htons(1234);                     // Port number (convert to network byte order)
    client_address.sin_addr.s_addr = inet_addr("192.168.255.130"); // Server IP address

    // Attempt to connect to the server
    result = connect(client_socket, (SOCKADDR*)&client_address, sizeof(client_address));
    if (result == SOCKET_ERROR) {
        printf("connect() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Receive data from server
    recv(client_socket, recvdata, sizeof(recvdata), 0);
    printf("%s\n", recvdata);

    // Send data to the server
    send(client_socket, senddata, sizeof(senddata), 0);

    // Close the socket
    closesocket(client_socket);

    // Cleanup Winsock
    WSACleanup();
    return 0;
}
