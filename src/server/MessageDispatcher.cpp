#include "server/MessageDispatcher.hpp"

MessageDispatcher::MessageDispatcher(std::queue<Message> *queue/*, Server* srv*/) : pendingMessages(queue)/*, server(srv)*/ {}

void MessageDispatcher::ProcessReceivedMessage(Message* msg) {
    if (msg == nullptr) {
        return;
    }

    switch (msg->getType()) {
        case MessageType::ALERT:
            processReceivedAlert(msg);
            break;

        case MessageType::INVENTORY:
            switch (static_cast<InventorySubType>(msg->getSubType())) {
                case InventorySubType::REQUEST:
                    ProcessReceivedInventoryRequest(msg);
                    break;
                case InventorySubType::INFO:
                    ProcessInventoryInfoRequest(msg);
                    break;
                case InventorySubType::HISTORY:
                    ProcessTransactionHistoryRequest(msg);
                    break;
                default:
                    delete msg;
                    break;
            }
            break;

        case MessageType::CREDENTIALS:
            switch (static_cast<CredentialSubType>(msg->getSubType())) {
                case CredentialSubType::LOGIN:
                    processLogin(msg);
                    break;
                case CredentialSubType::LOGOUT:
                    processLogout(msg);
                    break;
                case CredentialSubType::SUBSCRIPTION:
                    ProcessSubscriptions(msg);
                    break;
                default:
                    delete msg;
                    break;
            }
            break;

        default:
            delete msg;
            break;
    }
}

void MessageDispatcher::processReceivedAlert(Message* msg) {
    if (msg->getSubType() >= static_cast<int>(AlertSubType::WEATHER) &&
        msg->getSubType() <= static_cast<int>(AlertSubType::INFECTION)) {
        // server->generateAlert(msg);
    } else {
        delete msg;
    }
}

void MessageDispatcher::processLogin(Message* msg) {
    cJSON* content = msg->getContent();
    if (content == nullptr) {
        delete msg;
        return;
    }

    cJSON* passwordField = cJSON_GetObjectItem(content, "password");
    if (passwordField == nullptr || !cJSON_IsString(passwordField)) {
        delete msg;
        return;
    }

    std::string password = passwordField->valuestring;
    int sender = msg->getClientID();
    delete msg;
    // server->login(password, sender);
}

void MessageDispatcher::processLogout(Message* msg) {
    int sender = msg->getClientID();
    delete msg;
    // server->logout(sender);
}

void MessageDispatcher::ProcessSubscriptions(Message* msg) {
    cJSON* content = msg->getContent();
    if (content == nullptr) {
        delete msg;
        return;
    }

    int sender = msg->getClientID();
    cJSON* subscribeField = cJSON_GetObjectItem(content, "subscribe");
    if (subscribeField != nullptr && cJSON_IsArray(subscribeField)) {
        cJSON* subscription = nullptr;
        cJSON_ArrayForEach(subscription, subscribeField) {
            if (!cJSON_IsString(subscription)) {
                continue;
            }

            std::string subscriptionType = subscription->valuestring;

            if (subscriptionType == "ON_ROUTE") {
                //server->handleSubscriptions(sender, NotificationSubType::ON_ROUTE, true);
            } else if (subscriptionType == "RECEIVED") {
                //server->handleSubscriptions(sender, NotificationSubType::RECEIVED, true);
            } else if (subscriptionType == "NO_STOCK") {
                //server->handleSubscriptions(sender, NotificationSubType::NO_STOCK, true);
            }
        }
    }

    cJSON* unsubscribeField = cJSON_GetObjectItem(content, "unsubscribe");
    if (unsubscribeField != nullptr && cJSON_IsArray(unsubscribeField)) {
        cJSON* unsubscription = nullptr;
        cJSON_ArrayForEach(unsubscription, unsubscribeField) {
            if (!cJSON_IsString(unsubscription)) {
                continue;
            }

            std::string unsubscriptionType = unsubscription->valuestring;

            if (unsubscriptionType == "ON_ROUTE") {
                //server->handleSubscriptions(sender, NotificationSubType::ON_ROUTE, false);
            } else if (unsubscriptionType == "RECEIVED") {
                //server->handleSubscriptions(sender, NotificationSubType::RECEIVED, false);
            } else if (unsubscriptionType == "NO_STOCK") {
                //server->handleSubscriptions(sender, NotificationSubType::NO_STOCK, false);
            }
        }
    }

    delete msg;
}

void MessageDispatcher::ProcessReceivedInventoryRequest(Message* msg) {
    cJSON* content = msg->getContent();
    if (content == nullptr) {
        delete msg;
        return;
    }

    cJSON* productsField = cJSON_GetObjectItem(content, "products");
    if (productsField == nullptr || !cJSON_IsArray(productsField)) {
        delete msg;
        return;
    }

    int sender = msg->getClientID();
    std::vector<std::pair<int, int>> productRequests;
    cJSON* product = nullptr;
    cJSON_ArrayForEach(product, productsField) {
        cJSON* idField = cJSON_GetObjectItem(product, "id");
        cJSON* quantityField = cJSON_GetObjectItem(product, "quantity");

        if (!cJSON_IsNumber(idField) || !cJSON_IsNumber(quantityField)) {
            continue;
        }

        productRequests.emplace_back(idField->valueint, quantityField->valueint);
    }

    // server->handleInventoryRequest(sender, productRequests);

    delete msg;
}

void MessageDispatcher::ProcessInventoryInfoRequest(Message* msg) {
    int sender = msg->getClientID();
    // server->checkCurrentInventory(sender);

    delete msg;
}

void MessageDispatcher::ProcessTransactionHistoryRequest(Message* msg) {
    int clientId = msg->getClientID();
    // server->checkTransactionHistory(clientId);

    delete msg;
}
