#include <gtest/gtest.h>
#include "server/NotificationSystem.hpp"

class NotificationSystemTest : public ::testing::Test {
protected:
    NetworkManager networkManager;
    NotificationSystem* system{};

    void SetUp() override {
        system = new NotificationSystem(&networkManager);
    }

    void TearDown() override {
        delete system;
    }
};

TEST_F(NotificationSystemTest, RegisterClientInitializesDefaultSubscriptions) {
    system->registerClient(1);

    EXPECT_TRUE(system->isSubscribed(1, NotificationSubType::ON_ROUTE));
    EXPECT_TRUE(system->isSubscribed(1, NotificationSubType::RECEIVED));
    EXPECT_TRUE(system->isSubscribed(1, NotificationSubType::NO_STOCK));
}

TEST_F(NotificationSystemTest, RemoveClientClearsSubscriptions) {
    system->registerClient(2);
    system->removeClient(2);

    EXPECT_FALSE(system->isSubscribed(2, NotificationSubType::ON_ROUTE));
}

TEST_F(NotificationSystemTest, SubscribeAddsSubscription) {
    system->registerClient(3);
    system->unsubscribe(3, NotificationSubType::RECEIVED);

    EXPECT_FALSE(system->isSubscribed(3, NotificationSubType::RECEIVED));

    system->subscribe(3, NotificationSubType::RECEIVED);
    EXPECT_TRUE(system->isSubscribed(3, NotificationSubType::RECEIVED));
}

TEST_F(NotificationSystemTest, NotifySendsMessageIfSubscribed) {
    system->registerClient(4);

    EXPECT_TRUE(system->notify(4, NotificationSubType::NO_STOCK, "Test message"));
}

TEST_F(NotificationSystemTest, NotifyDoesNotSendIfNotSubscribed) {
    system->registerClient(5);
    system->unsubscribe(5, NotificationSubType::RECEIVED);

    EXPECT_FALSE(system->notify(5, NotificationSubType::RECEIVED, "No message"));
}
