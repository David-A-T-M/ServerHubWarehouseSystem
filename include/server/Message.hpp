#pragma once

#include <string>
#include "MessageTypes.hpp"
#include "cjson/cJSON.h"

/**
 * @class Message
 * @brief Represents a communication message exchanged between the server and a client.
 *
 * This class encapsulates the details of a message, including its type, subtype, content,
 * and metadata such as the client ID. It provides functionality for serialization,
 * deserialization, and deep copying of messages.
 */
class Message {
    public:
        /**
         * @brief Constructs a Message with a specified type, subtype, and content.
         *
         * This constructor is used when the client ID is not required.
         *
         * @tparam EnumClass The enumeration type for the message subtype.
         * @param type The main type of the message (e.g., ALERT, NOTIFICATION).
         * @param subType The specific subtype of the message, derived from the main type.
         * @param msgContent A pointer to the cJSON object representing the message content.
         */
        template<typename EnumClass>
        Message(MessageType type, EnumClass subType, cJSON* msgContent)
        : clientID(-1), type(type), subType(static_cast<int>(subType)), content(msgContent) {}

        /**
         * @brief Constructs a Message with a client ID, type, subtype, and content.
         *
         * This constructor is used when the message is associated with a specific client.
         *
         * @tparam EnumClass The enumeration type for the message subtype.
         * @param id The unique identifier of the client.
         * @param type The main type of the message (e.g., ALERT, NOTIFICATION).
         * @param subType The specific subtype of the message, derived from the main type.
         * @param msgContent A pointer to the cJSON object representing the message content.
         */
        template <typename EnumClass>
        Message(int id, MessageType type, EnumClass subType, cJSON* msgContent)
        : clientID(id), type(type), subType(static_cast<int>(subType)), content(msgContent) {}

        /**
         * @brief Copy constructor for the Message class.
         *
         * Creates a deep copy of the given Message object, including its content.
         *
         * @param other The Message object to copy.
         */
        Message(const Message& other);

        /**
         * @brief Assignment operator for the Message class.
         *
         * Performs a deep copy of the given Message object, replacing the current object's data.
         *
         * @param other The Message object to assign to this instance.
         * @return A reference to the updated Message object.
         */
        Message& operator=(const Message& other);

        /**
         * @brief Destructor for the Message class.
         *
         * Cleans up the dynamically allocated cJSON content to prevent memory leaks.
         */
        ~Message();

        /**
         * @brief Retrieves the unique identifier of the client associated with the message.
         *
         * @return The client ID, or -1 if no client ID is associated.
         */
        [[nodiscard]] int getClientID() const;

        /**
         * @brief Retrieves the main type of the message.
         *
         * The type indicates the general category of the message (e.g., ALERT, NOTIFICATION).
         *
         * @return The MessageType enumeration value representing the message type.
         */
        [[nodiscard]] MessageType getType() const;

        /**
         * @brief Retrieves the subtype of the message.
         *
         * The subtype provides a more specific classification within the main type.
         *
         * @return An integer representing the subtype of the message.
         */
        [[nodiscard]] int getSubType() const;

        /**
         * @brief Retrieves the content of the message.
         *
         * The content is stored as a cJSON object, which can be used to access
         * the message's payload.
         *
         * @return A pointer to the cJSON object representing the message content.
         */
        [[nodiscard]] cJSON* getContent() const;

        /**
         * @brief Sets the unique identifier of the client associated with the message.
         *
         * This method allows assigning or updating the client ID for the message.
         *
         * @param clientID The unique identifier of the client.
         */
        void setClientID(int clientID);

        /**
         * @brief Serializes the message into a JSON string.
         *
         * Converts the message's data, including its client ID, type, subtype,
         * and content, into a JSON-formatted string for transmission or storage.
         *
         * @return A string containing the JSON representation of the message.
         */
        [[nodiscard]] std::string toJSONString() const;

        /**
         * @brief Deserializes a JSON string into a Message object.
         *
         * Parses a JSON-formatted string and creates a Message object based on
         * the provided data. Throws an exception if the JSON is invalid or malformed.
         *
         * @param jsonString The JSON string to parse.
         * @return A Message object created from the parsed JSON data.
         * @throws runtime_error If the JSON string is invalid or malformed.
         */
        static Message fromJSONString(const std::string& jsonString);

    private:
        int clientID; ///< The unique identifier of the client associated with the message.
        MessageType type; ///< The main type of the message (e.g., ALERT, NOTIFICATION).
        int subType; ///< The specific subtype of the message.
        cJSON* content; ///< The content of the message as a cJSON object.
};
