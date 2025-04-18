#pragma once

#include <netinet/in.h>
#include <string>

/**
 * @brief Represents a client's connection to the server.
 *
 * This class encapsulates the details of a client's connection, including
 * its unique identifier, socket information, address, and protocol type.
 * It also tracks the connection status of the client.
 */
class ClientConnection {
    public:
        /**
         * @brief Enum representing the protocol type of the connection.
         */
        enum class Protocol {
            TCP, ///< Transmission Control Protocol
            UDP  ///< User Datagram Protocol
        };

        /**
         * @brief Constructs a new ClientConnection.
         *
         * @param id Unique identifier for the client.
         * @param sock Socket descriptor for the client's connection.
         * @param addr Address information of the client.
         * @param len Length of the client's address.
         * @param proto Protocol type used by the client (TCP or UDP).
         */
        ClientConnection(int id, int sock, const sockaddr_storage& addr, socklen_t len, Protocol proto);

        /**
         * @brief Gets the unique identifier of the client.
         * @return The client's unique identifier.
         */
        int getClientID() const;

        /**
         * @brief Gets the socket descriptor of the client's connection.
         * @return The socket descriptor.
         */
        int getSocket() const;

        /**
         * @brief Gets the address information of the client.
         * @return The client's address as a `sockaddr_storage` object.
         */
        [[nodiscard]] sockaddr_storage getAddress() const;

        /**
         * @brief Gets the length of the client's address.
         * @return The length of the client's address.
         */
        [[nodiscard]] socklen_t getAddressLength() const;

        /**
         * @brief Gets the protocol type used by the client.
         * @return The protocol type (TCP or UDP).
         */
        [[nodiscard]] Protocol getProtocol() const;

        /**
         * @brief Checks if the client is currently connected.
         * @return `true` if the client is connected, `false` otherwise.
         */
        [[nodiscard]] bool isConnected() const;

        /**
         * @brief Disconnects the client.
         *
         * Marks the client as disconnected and performs any necessary cleanup.
         */
        void disconnect();

    private:
        int clientID; ///< Unique identifier for the client.
        int socket; ///< Socket descriptor for the client's connection.
        sockaddr_storage clientAddress; ///< Address information of the client.
        socklen_t addressLength; ///< Length of the client's address.
        Protocol protocol; ///< Protocol type used by the client (TCP or UDP).
        bool connected; ///< Indicates whether the client is currently connected.
};
