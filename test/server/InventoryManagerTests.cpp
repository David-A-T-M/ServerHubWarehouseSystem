#include "gtest/gtest.h"
#include "server/InventoryManager.hpp"

class InventoryManagerTest : public ::testing::Test {
protected:
    InventoryManager inventory;
};

TEST_F(InventoryManagerTest, AddClientWithInventory) {
    cJSON* initialInventory = cJSON_CreateObject();
    cJSON_AddNumberToObject(initialInventory, "3", 10);
    inventory.addClient(1, initialInventory);
    EXPECT_EQ(inventory.getStockLevel(3), 10);
}

TEST_F(InventoryManagerTest, AddClientWithoutInventory) {
    inventory.addClient(2, nullptr);
    EXPECT_NE(inventory.getClientInventory(2), nullptr);
}

TEST_F(InventoryManagerTest, RemoveClient) {
    cJSON* initialInventory = cJSON_CreateObject();
    cJSON_AddNumberToObject(initialInventory, "3", 10);
    inventory.addClient(2, initialInventory);
    inventory.removeClient(2);
    EXPECT_EQ(inventory.getStockLevel(3), 0);
}

TEST_F(InventoryManagerTest, IncreaseStock_Valid) {
    EXPECT_TRUE(inventory.increaseStock(100, 10));
    EXPECT_EQ(inventory.getStockLevel(100), 10);
}

TEST_F(InventoryManagerTest, IncreaseStock_NegativeQuantity) {
    EXPECT_FALSE(inventory.increaseStock(200, -5));
    EXPECT_EQ(inventory.getStockLevel(200), 0);
}

TEST_F(InventoryManagerTest, IncreaseStock_ZeroQuantity) {
    EXPECT_FALSE(inventory.increaseStock(201, 0));
    EXPECT_EQ(inventory.getStockLevel(201), 0);
}

TEST_F(InventoryManagerTest, DecreaseStock_Valid) {
    inventory.increaseStock(300, 15);
    EXPECT_TRUE(inventory.decreaseStock(300, 10));
    EXPECT_EQ(inventory.getStockLevel(300), 5);
}

TEST_F(InventoryManagerTest, DecreaseStock_TooMuch) {
    inventory.increaseStock(301, 5);
    EXPECT_FALSE(inventory.decreaseStock(301, 10));
    EXPECT_EQ(inventory.getStockLevel(301), 5);
}

TEST_F(InventoryManagerTest, DecreaseStock_NegativeQuantity) {
    inventory.increaseStock(302, 10);
    EXPECT_FALSE(inventory.decreaseStock(302, -3));
    EXPECT_EQ(inventory.getStockLevel(302), 10);
}

TEST_F(InventoryManagerTest, DecreaseStock_ZeroQuantity) {
    inventory.increaseStock(303, 7);
    EXPECT_FALSE(inventory.decreaseStock(303, 0));
    EXPECT_EQ(inventory.getStockLevel(303), 7);
}

TEST_F(InventoryManagerTest, DecreaseStock_ItemDoesNotExist) {
    EXPECT_FALSE(inventory.decreaseStock(404, 5));
}

TEST_F(InventoryManagerTest, GetStockLevel_ItemNotPresent) {
    EXPECT_EQ(inventory.getStockLevel(999), 0);
}
