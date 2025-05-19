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
    NO_STOCK,
    DISCARDED
};

enum class InventorySubType {
    REQUEST,
    INFO,
    HISTORY
};

enum class CredentialSubType {
    LOGIN,
    LOGOUT,
    SUBSCRIPTION
};
