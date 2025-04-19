#include "server/NotificationSystem.hpp"
#include <ranges>

const std::set DEFAULT_NOTIFICATIONS = {
    NotificationSubType::ON_ROUTE,
    NotificationSubType::RECEIVED,
    NotificationSubType::NO_STOCK
};

NotificationSystem::NotificationSystem(NetworkManager* networkManager) {
    this->networkManager = networkManager;
}

void NotificationSystem::registerClient(int clientID) {
    subscriptions[clientID] = DEFAULT_NOTIFICATIONS;
}

void NotificationSystem::removeClient(int clientID) {
    subscriptions.erase(clientID);
}

bool NotificationSystem::isSubscribed(int clientID, NotificationSubType type) const {
    auto it = subscriptions.find(clientID);
    return it != subscriptions.end() && it->second.contains(type);
}

void NotificationSystem::broadcastAlert(AlertSubType subType, const std::string& message) {
    cJSON* content = cJSON_CreateObject();
    cJSON_AddStringToObject(content, "message", message.c_str());
    for (int clientID : subscriptions | std::views::keys) {
        Message msg(clientID, MessageType::ALERT, subType, content);
        networkManager->sendMessage(msg);
    }
}

void NotificationSystem::subscribe(int clientID, NotificationSubType type) {
    subscriptions[clientID].insert(type);
}

void NotificationSystem::unsubscribe(int clientID, NotificationSubType type) {
    if (auto it = subscriptions.find(clientID); it != subscriptions.end()) {
        it->second.erase(type);
    }
}

bool NotificationSystem::notify(int clientID, NotificationSubType type, const std::string& message) {
    if (auto it = subscriptions.find(clientID); it != subscriptions.end() && it->second.contains(type)) {
        cJSON* content = cJSON_CreateObject();
        cJSON_AddStringToObject(content, "message", message.c_str());
        const Message msg(clientID, MessageType::NOTIFICATION, type, content);
        networkManager->sendMessage(msg);
        return true;
    }
    return false;
}
