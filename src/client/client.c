#include "client/client.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

bool initializeClient(Client* client, const char* ip, int port, int protocol) {
    client->protocol = protocol;
    client->socket_fd = socket(AF_INET, protocol == 0 ? SOCK_STREAM : SOCK_DGRAM, 0);
    if (client->socket_fd < 0) return false;

    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(port);
    client->server_addr.sin_addr.s_addr = inet_addr(ip);

    if (protocol == 0 && connect(client->socket_fd, (struct sockaddr*)&client->server_addr, sizeof(client->server_addr)) < 0) {
        return false;
    }

    client->is_connected = true;
    return true;
}

bool sendMessage(Client* client, const char* message) {
    if (client->protocol == 0) {
        if (!client->is_connected) return false;
        return send(client->socket_fd, message, strlen(message), 0) >= 0;
    }
    if (client->protocol == 1) {
        return sendto(client->socket_fd, message, strlen(message), 0,
                      (struct sockaddr*)&client->server_addr, sizeof(client->server_addr)) >= 0;
    }
    return false;
}

bool receiveMessage(Client* client, char* buffer, int buffer_size) {
    if (!client->is_connected) return false;
    int bytes = recv(client->socket_fd, buffer, buffer_size - 1, 0);
    if (bytes < 0) return false;
    buffer[bytes] = '\0';
    return true;
}

void closeClient(Client* client) {
    if (client->is_connected) {
        close(client->socket_fd);
        client->is_connected = false;
    }
}
