#pragma once

#include <map>
#include <set>
#include <string>
#include "server/NetworkManager.hpp"

/**
 * @brief Manages client subscriptions and sends notifications or alerts.
 *
 * The `NotificationSystem` class allows clients to subscribe to specific types of notifications
 * and provides methods to send targeted notifications or broadcast alerts to all clients.
 */
class NotificationSystem {
    public:
        /**
         * @brief Constructs a `NotificationSystem` instance.
         *
         * Initializes the notification system with a pointer to a `NetworkManager` instance
         * used for sending messages.
         *
         * @param networkManager Pointer to the `NetworkManager` responsible for message delivery.
         */
        explicit NotificationSystem(NetworkManager* networkManager);

        /**
         * @brief Registers a client with default subscriptions.
         *
         * Adds the client to the system and subscribes them to the default notification types:
         * `ON_ROUTE`, `RECEIVED`, and `NO_STOCK`.
         *
         * @param clientID Unique identifier of the client.
         */
        void registerClient(int clientID);

        /**
         * @brief Removes a client and clears their subscriptions.
         *
         * Deletes the client from the system and removes all their subscriptions.
         *
         * @param clientID Unique identifier of the client to be removed.
         */
        void removeClient(int clientID);

        /**
         * @brief Checks if a client is subscribed to a specific notification type.
         *
         * Determines whether the client is subscribed to the given notification type.
         *
         * @param clientID Unique identifier of the client.
         * @param type The notification type to check.
         * @return `true` if the client is subscribed, `false` otherwise.
         */
        bool isSubscribed(int clientID, NotificationSubType type) const;

        /**
         * @brief Broadcasts an alert to all clients.
         *
         * Sends a high-priority alert message to all registered clients.
         *
         * @param subType The type of alert to broadcast.
         * @param message The alert message to be sent.
         */
        void broadcastAlert(AlertSubType subType, const std::string& message);

        /**
         * @brief Subscribes a client to a specific notification type.
         *
         * Adds the specified notification type to the client's subscription list.
         * If the client is already subscribed, no changes are made.
         *
         * @param clientID Unique identifier of the client.
         * @param type The notification type to subscribe to.
         */
        void subscribe(int clientID, NotificationSubType type);

        /**
         * @brief Unsubscribes a client from a specific notification type.
         *
         * Removes the specified notification type from the client's subscription list.
         * If the client is not subscribed, no changes are made.
         *
         * @param clientID Unique identifier of the client.
         * @param type The notification type to unsubscribe from.
         */
        void unsubscribe(int clientID, NotificationSubType type);

        /**
         * @brief Sends a notification to a specific client if subscribed.
         *
         * Sends a notification message to the client if they are subscribed to the specified
         * notification type.
         *
         * @param clientID Unique identifier of the client.
         * @param type The type of notification to send.
         * @param message The notification message to be sent.
         * @return `true` if the notification was sent successfully, `false` otherwise.
         */
        bool notify(int clientID, NotificationSubType type, const std::string& message);
    private:
        std::map<int, std::set<NotificationSubType>> subscriptions; ///< Maps client IDs to their subscribed notification types.
        NetworkManager* networkManager; ///< Pointer to the `NetworkManager` for sending messages.
};
