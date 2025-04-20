#ifndef CLIENT_H
#define CLIENT_H

#include <stdbool.h>
#include <netinet/in.h>

/**
 * @struct Client
 * @brief Represents a client connection, supporting both TCP and UDP protocols.
 */
typedef struct {
    int socket_fd; ///< File descriptor for the socket.
    bool is_connected; ///< Indicates whether the client is currently connected.
    int protocol; ///< Protocol used by the client (0 for TCP, 1 for UDP).
    struct sockaddr_in server_addr; ///< Server address information.
} Client;

/**
 * @brief Initializes a client connection.
 *
 * @param client Pointer to the Client structure to initialize.
 * @param ip IP address of the server to connect to.
 * @param port Port number of the server.
 * @param protocol Protocol to use (0 for TCP, 1 for UDP).
 * @return true if the client was successfully initialized, false otherwise.
 */
bool initializeClient(Client* client, const char* ip, int port, int protocol);

/**
 * @brief Sends a message to the server.
 *
 * @param client Pointer to the Client structure.
 * @param message Message to send.
 * @return true if the message was successfully sent, false otherwise.
 */
bool sendMessage(Client* client, const char* message);

/**
 * @brief Receives a message from the server.
 *
 * @param client Pointer to the Client structure.
 * @param buffer Buffer to store the received message.
 * @param buffer_size Size of the buffer.
 * @return true if a message was successfully received, false otherwise.
 */
bool receiveMessage(Client* client, char* buffer, int buffer_size);

/**
 * @brief Closes the client connection.
 *
 * @param client Pointer to the Client structure to close.
 */
void closeClient(Client* client);

#endif //CLIENT_H
