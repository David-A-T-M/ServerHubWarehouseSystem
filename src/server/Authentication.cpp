#include "server/Authentication.hpp"
#include <stdexcept>

bool Authentication::authenticate(const int client_id, const std::string& password) {
    const auto it = credentials.find(client_id);
    if (it == credentials.end()) return false;

    AuthData& auth = it->second;

    if (auth.isBlocked || auth.emergencyBlocked) return false;

    if (auth.hashedPassword == hashPassword(password)) {
        auth.failedAttempts = 0;
        auth.isLogged = true;
        //TODO : Log successful login
        return true;
    }

    auth.failedAttempts++;
    //TODO : Log failed login
    if (auth.failedAttempts >= AuthenticationConstants::MAX_FAILED_ATTEMPTS) {
        auth.isBlocked = true;
        //TODO : Log blocked
    }
    return false;
}

bool Authentication::isAuthorized(const int client_id) const {
    const auto it = credentials.find(client_id);
    return it != credentials.end() && it->second.isLogged && !it->second.isBlocked;
}

void Authentication::setLoggedOut(const int client_id) {
    if (const auto it = credentials.find(client_id); it != credentials.end()) {
        it->second.isLogged = false;
    }
}

void Authentication::addCredentials(const int client_id, const std::string& newPassword) {
    if (credentials.contains(client_id)) return;

    credentials[client_id] = {
        hashPassword(newPassword),
        false,
        0,
        false,
        false
    };
    //TODO : Loggear nuevo cliente
}

void Authentication::removeCredentials(const int client_id) {
    credentials.erase(client_id);
    //TODO : Loggear borrado de cliente
}

bool Authentication::unblockWithFingerprint(const int client_id) {
    const auto it = credentials.find(client_id);
    if (it == credentials.end()) return false;

    AuthData& auth = it->second;
    if (!auth.isBlocked) return false;

    auth.isBlocked = false;
    auth.failedAttempts = 0;
    auth.isLogged = true;
    //TODO : Log successful unblock with fingerprint
    return true;
}

void Authentication::blockDueToEmergency(const int client_id) {
    if (const auto it = credentials.find(client_id); it != credentials.end()) {
        it->second.emergencyBlocked = true;
        //TODO : Log emergency block
        //TODO : Called by the server when an emergency alert is triggered
    }
}

bool Authentication::unlockWithSecretPhrase(const int client_id, const std::string& secretPhrase) {
    const auto it = credentials.find(client_id);
    if (it == credentials.end()) return false;

    if (AuthData& auth = it->second;
        auth.emergencyBlocked && secretPhrase == "emergencyUnlock") {
        auth.emergencyBlocked = false;
        return true;
        //TODO : Log successful unlock with secret phrase
        //TODO : fix secretPhrase location
    }
    return false;
}

std::string Authentication::hashPassword(const std::string& password) const {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}
