#include "server/InventoryManager.hpp"
#include <cstdio>

InventoryManager::InventoryManager() {
    globalInventory = cJSON_CreateObject();
}

InventoryManager::~InventoryManager() {
    if (globalInventory) {
        cJSON_Delete(globalInventory);
    }
    for (auto& pair : clientInventories) {
        if (pair.second) {
            cJSON_Delete(pair.second);
        }
    }
}

void InventoryManager::addClient(int clientID, cJSON* initialInventory) {
    if (clientInventories.find(clientID) == clientInventories.end()) {
        clientInventories[clientID] = initialInventory ? cJSON_Duplicate(initialInventory, true) : cJSON_CreateObject();
        if (initialInventory) {
            cJSON* item = nullptr;
            cJSON_ArrayForEach(item, initialInventory) {
                if (cJSON_IsNumber(item)) {
                    int itemID = std::stoi(item->string);
                    int quantity = static_cast<int>(item->valuedouble);
                    increaseStock(itemID, quantity);
                }
            }
        }
    }
}

void InventoryManager::removeClient(int clientID) {
    auto it = clientInventories.find(clientID);
    if (it != clientInventories.end()) {
        cJSON* clientInventory = it->second;

        cJSON* item = nullptr;
        cJSON_ArrayForEach(item, clientInventory) {
            if (cJSON_IsNumber(item)) {
                int itemID = std::stoi(item->string);
                int quantity = static_cast<int>(item->valuedouble);
                decreaseStock(itemID, quantity);
            }
        }

        cJSON_Delete(clientInventory);
        clientInventories.erase(it);
    }
}

cJSON* InventoryManager::getClientInventory(int clientID) {
    auto it = clientInventories.find(clientID);
    if (it != clientInventories.end()) {
        return it->second;
    }
    return nullptr;
}

bool InventoryManager::increaseStock(int itemID, int quantity) {
    if (quantity <= 0 || !globalInventory) return false;

    std::string key = std::to_string(itemID);
    cJSON* item = cJSON_GetObjectItem(globalInventory, key.c_str());

    if (!item) {
        item = cJSON_CreateNumber(quantity);
        cJSON_AddItemToObject(globalInventory, key.c_str(), item);
    } else {
        item->valuedouble += quantity;
    }

    return true;
}

bool InventoryManager::decreaseStock(int itemID, int quantity) {
    if (quantity <= 0 || !globalInventory) return false;

    std::string key = std::to_string(itemID);
    cJSON* item = cJSON_GetObjectItem(globalInventory, key.c_str());

    if (!item || item->valuedouble < quantity) return false;

    item->valuedouble -= quantity;
    return true;
}

int InventoryManager::getStockLevel(int itemID) const {
    if (!globalInventory) return 0;

    std::string key = std::to_string(itemID);
    cJSON* item = cJSON_GetObjectItem(globalInventory, key.c_str());

    return item ? static_cast<int>(item->valuedouble) : 0;
}

void InventoryManager::logTransaction(int itemID, int quantity, const std::string& clientID) {
    //TODO: implementar usando el EventLogger y agregar tests
}

void InventoryManager::updateClientInventory(int clientID, cJSON* clientInventory) {
    if (!clientInventory) return;

    if (clientInventories[clientID]) {
        cJSON_Delete(clientInventories[clientID]);
    }
    clientInventories[clientID] = cJSON_Duplicate(clientInventory, true);
}

cJSON* InventoryManager::detectInventoryAnomalies(int clientID) const {
    // TODO: Implementar la detección de anomalías y agregar tests
    return nullptr;
}
