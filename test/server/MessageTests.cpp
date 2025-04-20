#include <gtest/gtest.h>
#include "server/Message.hpp"
#include "cjson/cJSON.h"

class MessageTest : public ::testing::Test {
protected:
    cJSON* testContent{};

    void SetUp() override {
        testContent = cJSON_CreateObject();
        cJSON_AddStringToObject(testContent, "key", "value");
    }

    void TearDown() override {
    }
};

TEST_F(MessageTest, ConstructorWithoutClientID) {
    const Message msg(MessageType::NOTIFICATION, NotificationSubType::RECEIVED, testContent);
    EXPECT_EQ(msg.getClientID(), -1);
    EXPECT_EQ(msg.getType(), MessageType::NOTIFICATION);
    EXPECT_EQ(msg.getSubType(), 1);
    ASSERT_NE(msg.getContent(), nullptr);
}

TEST_F(MessageTest, ConstructorWithAllFields) {
    Message msg(10, MessageType::ALERT, AlertSubType::WEATHER, testContent);
    EXPECT_EQ(msg.getClientID(), 10);
    EXPECT_EQ(msg.getType(), MessageType::ALERT);
    EXPECT_EQ(msg.getSubType(), 0);
    ASSERT_NE(msg.getContent(), nullptr);
    const cJSON* key = cJSON_GetObjectItemCaseSensitive(msg.getContent(), "key");
    ASSERT_TRUE(cJSON_IsString(key));
    EXPECT_STREQ(key->valuestring, "value");
}

TEST_F(MessageTest, CopyConstructorCreatesDeepCopy) {
    const Message original(1, MessageType::INVENTORY, InventorySubType::REQUEST, testContent);
    Message copy = original;

    EXPECT_EQ(copy.getClientID(), original.getClientID());
    EXPECT_EQ(copy.getType(), original.getType());
    EXPECT_EQ(copy.getSubType(), original.getSubType());
    EXPECT_NE(copy.getContent(), original.getContent());
}

TEST_F(MessageTest, AssignmentOperatorCreatesDeepCopy) {
    const Message original(2, MessageType::CREDENTIALS, CredentialSubType::LOGIN, testContent);
    Message copy(99, MessageType::NOTIFICATION, -1, cJSON_CreateObject());

    copy = original;
    EXPECT_EQ(copy.getClientID(), original.getClientID());
    EXPECT_EQ(copy.getType(), original.getType());
    EXPECT_EQ(copy.getSubType(), original.getSubType());
    EXPECT_NE(copy.getContent(), original.getContent());

    const cJSON* originalKey = cJSON_GetObjectItemCaseSensitive(original.getContent(), "key");
    const cJSON* copyKey = cJSON_GetObjectItemCaseSensitive(copy.getContent(), "key");
    ASSERT_TRUE(cJSON_IsString(originalKey));
    ASSERT_TRUE(cJSON_IsString(copyKey));
    EXPECT_STREQ(originalKey->valuestring, copyKey->valuestring);
}

TEST_F(MessageTest, clientIDSetter) {
    Message msg(3, MessageType::ALERT, AlertSubType::ENEMY_THREAT, testContent);
    msg.setClientID(42);
    EXPECT_EQ(msg.getClientID(), 42);
}

TEST_F(MessageTest, ToJSONStringIncludesAllFields) {
    const Message msg(7, MessageType::ALERT, AlertSubType::ENEMY_THREAT, testContent);
    std::string json = msg.toJSONString();

    EXPECT_NE(json.find("\"clientID\":7"), std::string::npos);
    EXPECT_NE(json.find("\"type\":0"), std::string::npos);
    EXPECT_NE(json.find("\"subType\":1"), std::string::npos);
    EXPECT_NE(json.find("\"key\":\"value\""), std::string::npos);
}

TEST_F(MessageTest, FromJSONStringParsesCorrectly) {
    const std::string json = R"({
        "clientID":5,
        "type":1,
        "subType":2,
        "content":{"foo":"bar"}
    })";
    const Message msg = Message::fromJSONString(json);

    EXPECT_EQ(msg.getClientID(), 5);
    EXPECT_EQ(msg.getType(), MessageType::NOTIFICATION);
    EXPECT_EQ(msg.getSubType(), 2);

    const cJSON* foo = cJSON_GetObjectItemCaseSensitive(msg.getContent(), "foo");
    ASSERT_TRUE(cJSON_IsString(foo));
    EXPECT_STREQ(foo->valuestring, "bar");
}

TEST_F(MessageTest, FromJSONStringThrowsOnMalformedJson) {
    std::string malformedJson = R"({"clientID":5,"type":"invalid","subType":0,"content":{}})";
    EXPECT_THROW(Message::fromJSONString(malformedJson), std::runtime_error);
}

TEST_F(MessageTest, FromJSONStringThrowsOnInvalidJson) {
    std::string notJson = "not a json!";
    EXPECT_THROW(Message::fromJSONString(notJson), std::runtime_error);
}
