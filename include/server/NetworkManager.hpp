#pragma once

#include <map>
#include <optional>
#include "ClientConnection.hpp"
#include "Message.hpp"
#include <vector>

/**
 * @brief Handles network communication between the server and clients.
 *
 * The NetworkManager class is responsible for initializing sockets, managing client connections,
 * sending and receiving messages, and handling both TCP and UDP protocols for IPv4 and IPv6.
 */
class NetworkManager {
    public:
        /**
         * @brief Constructs a new NetworkManager object.
         *
         * Initializes internal variables and prepares the manager for socket operations.
         */
        NetworkManager();

        /**
         * @brief Initializes the server sockets for both IPv4 and IPv6, TCP and UDP.
         *
         * @param port The port number to bind the sockets to.
         */
        void initialize(int port);

        /**
         * @brief Listens for incoming TCP connections.
         *
         * Accepts new client connections on the TCP IPv4 socket and adds them to the active clients list.
         */
        void listenForConnections();

        /**
         * @brief Receives incoming UDP messages.
         *
         * Processes messages received on both IPv4 and IPv6 UDP sockets and sends acknowledgment responses.
         *
         * @return A vector of `Message` objects containing the received messages.
         */
        std::vector<Message> receiveUDPMessage();

        /**
         * @brief Sends a message to a specific client.
         *
         * This function sends a message to the client identified by the `clientID` field in the `Message` object.
         * It determines the appropriate protocol (TCP or UDP) and sends the message accordingly.
         * If the client is not found in the active clients list, the function does nothing.
         *
         * @param msg The message to be sent, represented as a `Message` object. The `clientID` field must be set.
         */
        void sendMessage(const Message& msg);

        /**
         * @brief Receives a message from a specific client.
         *
         * @param clientID The unique identifier of the client to receive the message from.
         * @return An optional `Message` object containing the received message, or `std::nullopt` if no message was received.
         */
        std::optional<Message> receiveTCPMessage(int clientID);


        /**
         * @brief Closes the connection with a specific client.
         *
         * @param clientID The unique identifier of the client whose connection should be closed.
         */
        void closeConnection(int clientID);
    private:
        /**
         * @brief Sets up a socket for communication.
         *
         * Creates, configures, and binds a socket for the specified address family and type.
         *
         * @param socketFd Reference to the socket file descriptor to be initialized.
         * @param family The address family (e.g., AF_INET for IPv4, AF_INET6 for IPv6).
         * @param type The socket type (e.g., SOCK_STREAM for TCP, SOCK_DGRAM for UDP).
         * @param port The port number to bind the socket to.
         */
        void setupSocket(int& socketFd, int family, int type, int port);

        int serverSocketTCPv4; ///< File descriptor for the IPv4 TCP socket.
        int serverSocketUDPv4; ///< File descriptor for the IPv4 UDP socket.
        int serverSocketTCPv6; ///< File descriptor for the IPv6 TCP socket.
        int serverSocketUDPv6; ///< File descriptor for the IPv6 UDP socket.

        std::map<int, ClientConnection> activeClients; ///< Map of active client connections.
        int nextClientID; ///< Counter for assigning unique client IDs.


};
