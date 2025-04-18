#pragma once

enum class MessageType {
    ALERT,
    NOTIFICATION,
    INVENTORY,
    CREDENTIALS,
};

enum class AlertSubType {
    WEATHER,
    ENEMY_THREAT,
    INFECTION
};

enum class NotificationSubType {
    ON_ROUTE,
    RECEIVED,
    NO_STOCK
};

enum class InventorySubType {
    REQUEST,
    CANCEL,
    UPDATE,
    INFO,
    HISTORY
};

enum class CredentialSubType {
    LOGIN,
    LOGOUT,
    SUBSCRIPTION
};
