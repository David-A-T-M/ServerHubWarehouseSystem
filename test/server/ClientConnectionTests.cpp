#include <gtest/gtest.h>
#include "server/ClientConnection.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>

class ClientConnectionTest : public ::testing::Test {
protected:
    sockaddr_storage ipv4Addr{};
    sockaddr_storage ipv6Addr{};

    void SetUp() override {
        ipv4Addr = createIPv4LocalhostAddr();
        ipv6Addr = createIPv6LocalhostAddr();
    }

    static sockaddr_storage createIPv4LocalhostAddr() {
        sockaddr_storage addr{};
        auto* addr_in = reinterpret_cast<sockaddr_in*>(&addr);
        addr_in->sin_family = AF_INET;
        addr_in->sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &addr_in->sin_addr);
        return addr;
    }

    static sockaddr_storage createIPv6LocalhostAddr() {
        sockaddr_storage addr{};
        auto* addr_in6 = reinterpret_cast<sockaddr_in6*>(&addr);
        addr_in6->sin6_family = AF_INET6;
        addr_in6->sin6_port = htons(9090);
        inet_pton(AF_INET6, "::1", &addr_in6->sin6_addr);
        return addr;
    }
};

TEST_F(ClientConnectionTest, ConstructorInitialValuesIPv4) {
    ClientConnection connection(1, 42, ipv4Addr, sizeof(ipv4Addr), ClientConnection::Protocol::TCP);

    EXPECT_EQ(connection.getClientID(), 1);
    EXPECT_EQ(connection.getSocket(), 42);
    EXPECT_EQ(connection.getAddressLength(), sizeof(ipv4Addr));
    EXPECT_EQ(connection.getProtocol(), ClientConnection::Protocol::TCP);
    EXPECT_TRUE(connection.isConnected());

    sockaddr_storage address = connection.getAddress();
    auto addr = reinterpret_cast<const sockaddr_in*>(&address);
    EXPECT_EQ(addr->sin_family, AF_INET);
    EXPECT_EQ(ntohs(addr->sin_port), 8080);
}

TEST_F(ClientConnectionTest, ConstructorInitialValuesIPv6) {
    ClientConnection connection(2, 43, ipv6Addr, sizeof(ipv6Addr), ClientConnection::Protocol::UDP);

    EXPECT_EQ(connection.getClientID(), 2);
    EXPECT_EQ(connection.getSocket(), 43);
    EXPECT_EQ(connection.getAddressLength(), sizeof(ipv6Addr));
    EXPECT_EQ(connection.getProtocol(), ClientConnection::Protocol::UDP);
    EXPECT_TRUE(connection.isConnected());

    sockaddr_storage address = connection.getAddress();
    auto addr = reinterpret_cast<const sockaddr_in6*>(&address);
    EXPECT_EQ(addr->sin6_family, AF_INET6);
    EXPECT_EQ(ntohs(addr->sin6_port), 9090);
}

TEST_F(ClientConnectionTest, GetAddressLength) {
    ClientConnection connection(6, 47, ipv4Addr, sizeof(ipv4Addr), ClientConnection::Protocol::TCP);
    EXPECT_EQ(connection.getAddressLength(), sizeof(ipv4Addr));
}

TEST_F(ClientConnectionTest, GetAddress) {
    ClientConnection connection(7, 48, ipv4Addr, sizeof(ipv4Addr), ClientConnection::Protocol::TCP);
    sockaddr_storage address = connection.getAddress();
    auto addr = reinterpret_cast<const sockaddr_in*>(&address);
    EXPECT_EQ(addr->sin_family, AF_INET);
    EXPECT_EQ(ntohs(addr->sin_port), 8080);
}

TEST_F(ClientConnectionTest, ConnectionState) {
    ClientConnection connection(3, 44, ipv4Addr, sizeof(ipv4Addr), ClientConnection::Protocol::TCP);
    EXPECT_TRUE(connection.isConnected());
    connection.disconnect();
    EXPECT_FALSE(connection.isConnected());

    connection.disconnect();
    EXPECT_FALSE(connection.isConnected());
}

TEST_F(ClientConnectionTest, ProtocolHandling) {
    ClientConnection tcpConnection(4, 45, ipv4Addr, sizeof(ipv4Addr), ClientConnection::Protocol::TCP);
    ClientConnection udpConnection(5, 46, ipv4Addr, sizeof(ipv4Addr), ClientConnection::Protocol::UDP);

    EXPECT_EQ(tcpConnection.getProtocol(), ClientConnection::Protocol::TCP);
    EXPECT_EQ(udpConnection.getProtocol(), ClientConnection::Protocol::UDP);
}
