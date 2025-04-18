#include "server/NetworkManager.hpp"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

NetworkManager::NetworkManager() : serverSocketTCPv4(-1), serverSocketUDPv4(-1),
                                   serverSocketTCPv6(-1), serverSocketUDPv6(-1), nextClientID(1) {}

void NetworkManager::initialize(int port) {
    setupSocket(serverSocketTCPv4, AF_INET, SOCK_STREAM, port);
    setupSocket(serverSocketUDPv4, AF_INET, SOCK_DGRAM, port);
    setupSocket(serverSocketTCPv6, AF_INET6, SOCK_STREAM, port);
    setupSocket(serverSocketUDPv6, AF_INET6, SOCK_DGRAM, port);

    std::cout << "Sockets initialized on port " << port << " (IPv4/IPv6, TCP/UDP)" << std::endl;
}

void NetworkManager::listenForConnections() {
    sockaddr_storage clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);
    int clientSock = accept(serverSocketTCPv4, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
    if (clientSock >= 0) {
        int id = nextClientID++;
        ClientConnection conn(id, clientSock, clientAddr, addrLen, ClientConnection::Protocol::TCP);
        activeClients.emplace(id, conn);
        std::cout << "TCP Client connected: " << id << std::endl;
    }
}

std::vector<Message> NetworkManager::receiveUDPMessage() {
    char buffer[1024];
    std::vector<Message> receivedMessages;

    auto handleSocket = [&](int socketFd, ClientConnection::Protocol protocol) {
        sockaddr_storage senderAddr{};
        socklen_t addrLen = sizeof(senderAddr);

        ssize_t bytes = recvfrom(socketFd, buffer, sizeof(buffer), MSG_DONTWAIT,
                                 reinterpret_cast<sockaddr*>(&senderAddr), &addrLen);

        if (bytes <= 0) return;

        std::string data(buffer, bytes);
        Message msg = Message::fromJSONString(data);
        std::cout << "Received UDP message: " << msg.toJSONString() << std::endl;

        int clientId = -1;
        for (const auto& [id, client] : activeClients) {
            sockaddr_storage clientAddr = client.getAddress();
            if (client.getProtocol() == protocol &&
                client.getAddressLength() == addrLen &&
                memcmp(&clientAddr, &senderAddr, addrLen) == 0) {
                clientId = id;
                break;
                }
        }

        if (clientId == -1) {
            clientId = nextClientID++;
            ClientConnection newClient(clientId, socketFd, senderAddr, addrLen, protocol);
            activeClients.emplace(clientId, newClient);
            std::cout << "Registered new UDP client: " << clientId << std::endl;
        }

        msg.setClientID(clientId);
        receivedMessages.push_back(msg);

        std::string response = "Acknowledged";
        sendto(socketFd, response.c_str(), response.size(), 0,
               reinterpret_cast<sockaddr*>(&senderAddr), addrLen);
    };

    handleSocket(serverSocketUDPv4, ClientConnection::Protocol::UDP);
    handleSocket(serverSocketUDPv6, ClientConnection::Protocol::UDP);

    return receivedMessages;
}


void NetworkManager::sendMessage(int clientID, const Message& msg) {
    auto it = activeClients.find(clientID);
    if (it == activeClients.end()) return;

    const ClientConnection& client = it->second;
    std::string raw = msg.toJSONString();

    if (client.getProtocol() == ClientConnection::Protocol::TCP) {
        send(client.getSocket(), raw.c_str(), raw.size(), 0);
    } else {
        sockaddr_storage clientAddr = client.getAddress();
        sendto(client.getSocket(), raw.c_str(), raw.size(), 0,
               reinterpret_cast<sockaddr*>(&clientAddr), client.getAddressLength());
    }
}

std::optional<Message> NetworkManager::receiveTCPMessage(int clientID) {
    auto it = activeClients.find(clientID);
    if (it == activeClients.end()) return std::nullopt;

    ClientConnection& client = it->second;
    char buffer[1024] = {0};

    ssize_t bytes;
    if (client.getProtocol() == ClientConnection::Protocol::TCP) {
        bytes = recv(client.getSocket(), buffer, sizeof(buffer), 0);
    } else {
        sockaddr_storage clientAddr = client.getAddress();
        socklen_t addrLen = client.getAddressLength();
        bytes = recvfrom(client.getSocket(), buffer, sizeof(buffer), 0,
                         reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
    }

    if (bytes > 0) {
        return Message::fromJSONString(std::string(buffer, bytes));
    }
    return std::nullopt;
}

void NetworkManager::closeConnection(int clientID) {
    auto it = activeClients.find(clientID);
    if (it != activeClients.end()) {
        close(it->second.getSocket());
        activeClients.erase(it);
        std::cout << "Connection closed: " << clientID << std::endl;
    }
}

void NetworkManager::setupSocket(int& socketFd, int family, int type, int port) {
    socketFd = socket(family, type, 0);
    if (socketFd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configure IPV6_V6ONLY for IPv6 sockets
    if (family == AF_INET6) {
        setsockopt(socketFd, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
    }

    if (family == AF_INET) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(socketFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            perror("Bind failed (IPv4)");
            exit(EXIT_FAILURE);
        }
    } else {
        sockaddr_in6 addr6{};
        addr6.sin6_family = AF_INET6;
        addr6.sin6_addr = in6addr_any;
        addr6.sin6_port = htons(port);

        if (bind(socketFd, reinterpret_cast<sockaddr*>(&addr6), sizeof(addr6)) < 0) {
            perror("Bind failed (IPv6)");
            exit(EXIT_FAILURE);
        }
    }

    if (type == SOCK_STREAM) {
        if (listen(socketFd, 10) < 0) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }
    }
}
