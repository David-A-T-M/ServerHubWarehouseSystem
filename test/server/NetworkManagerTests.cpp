#include <gtest/gtest.h>
#include "server/NetworkManager.hpp"
#include "server/Message.hpp"

#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class NetworkManagerTest : public ::testing::Test {
protected:
    NetworkManager manager;
    int port = 30000 + (std::chrono::steady_clock::now().time_since_epoch().count() % 10000);
    Message* testMessage1{};
    Message* testMessage2{};

    void SetUp() override {
        manager.initialize(port);

        cJSON* content1 = cJSON_CreateObject();
        cJSON_AddStringToObject(content1, "type", "TEST");
        cJSON_AddStringToObject(content1, "content", "Message 1 content");
        auto* tmp = new Message(1, MessageType::ALERT, AlertSubType::ENEMY_THREAT, content1);
        testMessage1 = tmp;

        cJSON* content2 = cJSON_CreateObject();
        cJSON_AddStringToObject(content2, "type", "TEST");
        cJSON_AddStringToObject(content2, "content", "Message 2 content");
        tmp = new Message(2, MessageType::INVENTORY, InventorySubType::HISTORY, content2);
        testMessage2 = tmp;
    }

    void TearDown() override {
        delete testMessage1;
        delete testMessage2;
    }
};

TEST_F(NetworkManagerTest, SendMessageTest) {
    std::thread serverThread([&]() {
        manager.listenForConnections();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_GE(sockfd, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    ASSERT_EQ(connect(sockfd, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)), 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    manager.sendMessage(1, *testMessage1);

    char buffer[1024];
    ssize_t bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    ASSERT_GT(bytes, 0);

    std::string received(buffer, bytes);
    auto parsed = Message::fromJSONString(received);
    EXPECT_STREQ(cJSON_GetObjectItem(parsed.getContent(), "type")->valuestring, "TEST");
    EXPECT_STREQ(cJSON_GetObjectItem(parsed.getContent(), "content")->valuestring, "Message 1 content");

    close(sockfd);
    serverThread.join();
}

TEST_F(NetworkManagerTest, ReceiveTCPMessageTest) {
    std::thread serverThread([&]() {
        manager.listenForConnections();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_GE(sockfd, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    ASSERT_EQ(connect(sockfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)), 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string json = testMessage2->toJSONString();
    ssize_t sent = send(sockfd, json.c_str(), json.size(), 0);
    ASSERT_GT(sent, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto received = manager.receiveTCPMessage(1);
    ASSERT_TRUE(received.has_value());
    EXPECT_STREQ(cJSON_GetObjectItem(received->getContent(), "type")->valuestring, "TEST");
    EXPECT_STREQ(cJSON_GetObjectItem(received->getContent(), "content")->valuestring, "Message 2 content");

    close(sockfd);
    serverThread.join();
}

TEST_F(NetworkManagerTest, CloseConnectionTest) {
    std::thread serverThread([&]() {
        manager.listenForConnections();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_GE(sockfd, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    ASSERT_EQ(connect(sockfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)), 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Enviar un mensaje de prueba desde el cliente
    std::string json = testMessage1->toJSONString();
    ssize_t sent = send(sockfd, json.c_str(), json.size(), 0);
    ASSERT_GT(sent, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Asegurar que el servidor puede recibirlo
    auto receivedBeforeClose = manager.receiveTCPMessage(1);
    ASSERT_TRUE(receivedBeforeClose.has_value());

    // Cerrar la conexión desde el lado del servidor
    manager.closeConnection(1);

    Message serverMessage(1, MessageType::NOTIFICATION, NotificationSubType::NO_STOCK,
                          cJSON_CreateString("Server test message"));
    manager.sendMessage(1, serverMessage);

    char buffer[1024];
    ssize_t bytes = recv(sockfd, buffer, sizeof(buffer), 0);
    EXPECT_LE(bytes, 0); // Should return 0 (EOF) or -1 (error)

    close(sockfd);
    serverThread.join();
}

TEST_F(NetworkManagerTest, ReceiveUDPMessageTest) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_GE(sock_fd, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    std::string json = testMessage2->toJSONString();
    ssize_t sent = sendto(sock_fd, json.c_str(), json.size(), 0,
                          reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
    ASSERT_GT(sent, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto messages = manager.receiveUDPMessage();
    ASSERT_FALSE(messages.empty());
    EXPECT_STREQ(cJSON_GetObjectItem(messages[0].getContent(), "content")->valuestring, "Message 2 content");

    close(sock_fd);
}
