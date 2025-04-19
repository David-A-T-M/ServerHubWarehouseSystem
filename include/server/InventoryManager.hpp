#pragma once

#include "cjson/cJSON.h"
#include <string>
#include <map>

/**
 * @brief Handles inventory modifications and queries.
 * 
 * Tracks stock levels, logs transactions, and provides methods for managing
 * inventory data. Supports serialization and anomaly detection.
 */
class InventoryManager {

    public:
        InventoryManager();
        ~InventoryManager();

        bool increaseStock(int itemID, int quantity);
        bool decreaseStock(int itemID, int quantity);
        [[nodiscard]] int getStockLevel(int itemID) const;

        void logTransaction(int itemID, int quantity, const std::string& clientID);
        void updateClientInventory(int clientID, cJSON* clientInventory);
        cJSON* detectInventoryAnomalies(int clientID) const;

    private:
        cJSON* globalInventory; ///< Stores global inventory data as JSON for potential serialization.
        std::map<int, cJSON*> clientInventories; ///< Maps client IDs to their individual inventory data.
};
