#include "server/ClientConnection.hpp"
#include <unistd.h> // Para close()

ClientConnection::ClientConnection(const int id, const int sock, const sockaddr_storage& addr, socklen_t len, Protocol proto)
    : clientID(id), socket(sock), clientAddress(addr), addressLength(len), protocol(proto), connected(true) {}

int ClientConnection::getClientID() const {
    return clientID;
}

int ClientConnection::getSocket() const {
    return socket;
}

sockaddr_storage ClientConnection::getAddress() const {
    return clientAddress;
}

socklen_t ClientConnection::getAddressLength() const {
    return addressLength;
}

ClientConnection::Protocol ClientConnection::getProtocol() const {
    return protocol;
}

bool ClientConnection::isConnected() const {
    return connected;
}

void ClientConnection::disconnect() {
    connected = false;
}
