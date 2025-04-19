#pragma once

#include "cjson/cJSON.h"
#include <string>
#include <map>

/**
 * @brief Manages inventory operations and client-specific inventories.
 *
 * The `InventoryManager` class provides methods to handle global inventory
 * and client-specific inventories. It supports adding/removing clients,
 * modifying stock levels, logging transactions, and detecting anomalies.
 *
 * The inventory data is stored as JSON objects using the cJSON library,
 * allowing for easy serialization and manipulation.
 */
class InventoryManager {
    public:
        /**
         * @brief Constructs an `InventoryManager` instance.
         *
         * Initializes the global inventory as an empty JSON object.
         */
        InventoryManager();

        /**
         * @brief Destroys the `InventoryManager` instance.
         *
         * Frees memory allocated for the global inventory and all client-specific inventories.
         */
        ~InventoryManager();

        /**
         * @brief Adds a new client with an optional initial inventory.
         *
         * If the client does not already exist, their inventory is added.
         * If an initial inventory is provided, its items are added to the global inventory.
         *
         * @param clientID The unique identifier for the client.
         * @param initialInventory A JSON object representing the client's initial inventory.
         */
        void addClient(int clientID, cJSON* initialInventory);

        /**
         * @brief Removes a client and updates the global inventory.
         *
         * The client's inventory is removed, and its items are subtracted from the global inventory.
         *
         * @param clientID The unique identifier for the client to be removed.
         */
        void removeClient(int clientID);

        /**
         * @brief Retrieves the inventory of a specific client.
         *
         * @param clientID The unique identifier for the client.
         * @return A JSON object representing the client's inventory, or `nullptr` if the client does not exist.
         */
        cJSON* getClientInventory(int clientID);

        /**
         * @brief Increases the stock level of a specific item in the global inventory.
         *
         * If the item does not exist, it is added to the global inventory.
         *
         * @param itemID The unique identifier for the item.
         * @param quantity The quantity to add. Must be greater than 0.
         * @return `true` if the stock was successfully increased, `false` otherwise.
         */
        bool increaseStock(int itemID, int quantity);

        /**
         * @brief Decreases the stock level of a specific item in the global inventory.
         *
         * If the item does not exist or the quantity to decrease exceeds the current stock, the operation fails.
         *
         * @param itemID The unique identifier for the item.
         * @param quantity The quantity to subtract. Must be greater than 0.
         * @return `true` if the stock was successfully decreased, `false` otherwise.
         */
        bool decreaseStock(int itemID, int quantity);

        /**
         * @brief Retrieves the stock level of a specific item in the global inventory.
         *
         * @param itemID The unique identifier for the item.
         * @return The current stock level of the item, or 0 if the item does not exist.
         */
        [[nodiscard]] int getStockLevel(int itemID) const;

        /**
         * @brief Logs a transaction involving a specific item and client.
         *
         * This method is currently unimplemented.
         *
         * @param itemID The unique identifier for the item.
         * @param quantity The quantity involved in the transaction.
         * @param clientID The unique identifier for the client.
         */
        void logTransaction(int itemID, int quantity, const std::string& clientID);

        /**
         * @brief Updates the inventory of a specific client.
         *
         * Replaces the client's current inventory with the provided inventory.
         *
         * @param clientID The unique identifier for the client.
         * @param clientInventory A JSON object representing the new inventory for the client.
         */
        void updateClientInventory(int clientID, cJSON* clientInventory);

        /**
         * @brief Detects anomalies in a client's inventory.
         *
         * This method is currently unimplemented.
         *
         * @param clientID The unique identifier for the client.
         * @return A JSON object representing detected anomalies, or `nullptr` if unimplemented.
         */
        [[nodiscard]] cJSON* detectInventoryAnomalies(int clientID) const;

    private:
        cJSON* globalInventory; ///< Stores global inventory data as a JSON object.
        std::map<int, cJSON*> clientInventories; ///< Maps client IDs to their individual inventory data.
};
