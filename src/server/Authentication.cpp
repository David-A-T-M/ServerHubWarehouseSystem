#include "server/Authentication.hpp"
#include <stdexcept>

bool Authentication::authenticate(int client_id, const std::string& password) {
    auto it = credentials.find(client_id);
    if (it == credentials.end()) return false;

    AuthData& auth = it->second;

    if (auth.isBlocked || auth.emergencyBlocked) return false;

    if (auth.hashedPassword == hashPassword(password)) {
        auth.failedAttempts = 0;
        auth.isLogged = true;
        return true;
    }

    auth.failedAttempts++;
    if (auth.failedAttempts >= 3) {
        auth.isBlocked = true;
    }
    return false;
}

bool Authentication::isAuthorized(int client_id) const {
    auto it = credentials.find(client_id);
    return it != credentials.end() && it->second.isLogged && !it->second.isBlocked;
}

void Authentication::setLoggedOut(int client_id) {
    auto it = credentials.find(client_id);
    if (it != credentials.end()) {
        it->second.isLogged = false;
    }
}

void Authentication::addCredentials(int client_id, const std::string& newPassword) {
    if (credentials.contains(client_id)) return;

    credentials[client_id] = {
        hashPassword(newPassword),
        false,
        0,
        false,
        false
    };
}

void Authentication::removeCredentials(int client_id) {
    credentials.erase(client_id);
}

bool Authentication::unblockWithFingerprint(int client_id) {
    auto it = credentials.find(client_id);
    if (it == credentials.end()) return false;

    AuthData& auth = it->second;
    if (!auth.isBlocked) return false;

    auth.isBlocked = false;
    auth.failedAttempts = 0;
    auth.isLogged = true;
    return true;
}

void Authentication::blockDueToEmergency(int client_id) {
    auto it = credentials.find(client_id);
    if (it != credentials.end()) {
        it->second.emergencyBlocked = true;
    }
}

bool Authentication::unlockWithSecretPhrase(int client_id, const std::string& secretPhrase) {
    auto it = credentials.find(client_id);
    if (it == credentials.end()) return false;

    AuthData& auth = it->second;
    if (auth.emergencyBlocked && secretPhrase == "emergencyUnlock") {
        auth.emergencyBlocked = false;
        return true;
    }
    return false;
}

std::string Authentication::hashPassword(const std::string& password) const {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}