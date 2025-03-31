#include <stdio.h>          // Standard input/output functions
#include <sys/socket.h>     // Socket functions
#include <sys/types.h>      // Data types used in socket programming
#include <netinet/in.h>     // sockaddr_in structure for IP addresses
#include <arpa/inet.h>      // Functions for manipulating IP addresses
#include <unistd.h>         // For the `close()` function

int main(void) {
    int server_socket;
    int client_socket;

    struct sockaddr_in server_address;
    char buffer[1024] = "Hello world";
    char receivebuffer[1024];

    /* Create a server socket
     * https://man7.org/linux/man-pages/man2/socket.2.html
     * Paramater usage (int domain, int type, int protocol)
     * Domain = AF_INET which is IPv4 communication
     * Socket Type = SOCK_STREAM which is TCP
     * We don't need any bitwise
     */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Catch errors
    if (server_socket == -1) {
        perror("socket");
    }

    // Configure Server Address
    server_address.sin_family = AF_INET; // We're going to use IPv4
    server_address.sin_port = htons(1234); // Set server port
    server_address.sin_addr.s_addr = inet_addr("192.168.255.130");

    // Bind the socket to the specified address and port
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("bind");
        close(server_socket);
        return 1;
    }

    // Listen for incoming connections (up to 5 clients)
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        close(server_socket);
        return 1;
    }

    // Accept connection
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("accept");
        close(server_socket);
        return 1;
    }

    // Send buffer to the client
    printf("Client connected\n");
    send(client_socket, buffer, sizeof(buffer), 0);

    // Receive client's buffer
    receive(client_socket, receivebuffer, sizeof(receivebuffer));
    printf("%s\n", receivebuffer);

    // Cleanup
    close(client_socket);
    close(server_socket);
    return 0;
}
