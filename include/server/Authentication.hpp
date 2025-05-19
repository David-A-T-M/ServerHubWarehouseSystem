#pragma once

#include <string>
#include <map>

namespace AuthenticationConstants {
    constexpr int MAX_FAILED_ATTEMPTS = 3; /**< Maximum number of failed login attempts before blocking. */
}

/**
 * @class Authentication
 * @brief Handles client authentication and authorization.
 *
 * The Authentication class provides mechanisms for managing client credentials,
 * authenticating clients, and controlling their authorization status. It includes
 * features such as blocking clients after multiple failed login attempts, unblocking
 * them using a fingerprint-based method, and handling emergency blocking scenarios.
 *
 * Additionally, this class logs key events such as successful logins, failed login
 * attempts, client blocking, and credential modifications. These logs are essential
 * for auditing and debugging purposes.
 */
class Authentication {
    public:
        /**
         * @brief Authenticates a client using their ID and password.
         *
         * This method verifies the provided password against the stored hashed password.
         * If the password is incorrect, the number of failed attempts is incremented.
         * After 3 consecutive failed attempts, the client is blocked. Additionally,
         * authentication will fail if the client is already blocked or emergency blocked.
         *
         * Logs:
         * - Successful login.
         * - Failed login attempt.
         * - Client blocked after exceeding maximum failed attempts.
         *
         * @param client_id The unique identifier of the client.
         * @param password The plaintext password provided by the client.
         * @return `true` if authentication is successful, `false` otherwise.
         */
        bool authenticate(int client_id, const std::string& password);

        /**
         * @brief Checks if a client is authorized to perform actions.
         *
         * A client is considered authorized if they are logged in and not blocked.
         *
         * @param client_id The unique identifier of the client.
         * @return `true` if the client is authorized, `false` otherwise.
         */
        [[nodiscard]] bool isAuthorized(int client_id) const;

        /**
         * @brief Sets the logged-in status of a client to false.
         *
         * This method ensures that a client is marked as logged out. It does not allow
         * arbitrary changes to the logged-in status, improving security by preventing
         * unauthorized logins.
         *
         * Logs:
         * - Client logout event.
         *
         * @param client_id The unique identifier of the client.
         */
        void setLoggedOut(int client_id);

        /**
         * @brief Adds a new client with a hashed password.
         *
         * Registers a new client by storing their hashed password and initializing
         * their authentication data.
         *
         * Logs:
         * - New client added with their credentials.
         *
         * @param client_id The unique ID of the client.
         * @param newPassword The plaintext password to be hashed and stored.
         */
        void addCredentials(int client_id, const std::string& newPassword);

        /**
         * @brief Removes the credentials of a client.
         *
         * This method deletes the authentication data of a client, effectively removing
         * them from the system. If the client ID does not exist, the method does nothing.
         *
         * Logs:
         * - Client credentials removed.
         *
         * @param client_id The unique identifier of the client.
         */
        void removeCredentials(int client_id);

        /**
         * @brief Unblocks a client using a fingerprint-based method.
         *
         * Resets the block status and failed login attempts for a client.
         *
         * Logs:
         * - Client unblocked using fingerprint.
         *
         * @param client_id The unique ID of the client.
         * @return `true` if the client was successfully unblocked, `false` otherwise.
         */
        bool unblockWithFingerprint(int client_id);

        /**
         * @brief Blocks a client due to an emergency alert.
         *
         * Marks the client as blocked due to an emergency, preventing them from performing
         * any actions until manually unlocked.
         *
         * Logs:
         * - Client blocked due to an emergency alert.
         *
         * @param client_id The unique identifier of the client.
         */
        void blockDueToEmergency(int client_id);

        /**
         * @brief Unlocks a client using a predefined secret phrase.
         *
         * If the client is blocked due to an emergency, this method allows unlocking them
         * by providing the correct secret phrase.
         *
         * Logs:
         * - Client unlocked using the secret phrase.
         *
         * @param client_id The unique identifier of the client.
         * @param secretPhrase The secret phrase used to unlock the client.
         * @return `true` if the client was successfully unlocked, `false` otherwise.
         */
        bool unlockWithSecretPhrase(int client_id, const std::string& secretPhrase);    // TODO: Implement it to read from a config file

    private:
        /**
         * @struct AuthData
         * @brief Stores authentication data for a client.
         *
         * Contains the hashed password, login status, failed login attempts,
         * and block status for a client.
         */
        struct AuthData {
            std::string hashedPassword; /**< The hashed password of the client. */
            bool isLogged; /**< Indicates if the client is currently logged in. */
            int failedAttempts; /**< Number of consecutive failed login attempts. */
            bool isBlocked; /**< Indicates if the client is blocked. */
            bool emergencyBlocked; /**< Indicates if the client is blocked due to an emergency. */
        };

        std::map<int, AuthData> credentials; /**< Map of client IDs to their authentication data. */
        std::string emergencySecretPhrase = "defaultSecret"; /**< Predefined secret phrase for emergency unlock. */ // TODO: Implement in a config file

        /**
         * @brief Hashes a plaintext password.
         *
         * Converts a plaintext password into a hashed string for secure storage.
         *
         * @param password The plaintext password to hash.
         * @return The hashed password as a string.
         */
        [[nodiscard]] std::string hashPassword(const std::string& password) const;
};
