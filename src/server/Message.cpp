#include "server/Message.hpp"
#include <stdexcept>

Message::Message(const Message& other)
    : clientID(other.clientID), type(other.type), subType(other.subType), content(cJSON_Duplicate(other.content, 1)) {
}

Message& Message::operator=(const Message& other) {
    if (this != &other) {
        clientID = other.clientID;
        type = other.type;
        subType = other.subType;
        if (content) cJSON_Delete(content);
        content = cJSON_Duplicate(other.content, 1);
        if (!content) throw std::runtime_error("Failed to duplicate cJSON content");
    }
    return *this;
}

Message::~Message() {
    if (content) cJSON_Delete(content);
}

int Message::getClientID() const { return clientID; }
MessageType Message::getType() const { return type; }
int Message::getSubType() const { return subType; }
cJSON* Message::getContentRO() const { return content; }
cJSON* Message::getContentRW() { return content; }

void Message::setClientID(int id) { clientID = id; }
void Message::setType(MessageType t) { type = t; }

std::string Message::toJSONString() const {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "clientID", clientID);
    cJSON_AddNumberToObject(root, "type", static_cast<int>(type));
    cJSON_AddNumberToObject(root, "subType", subType);
    cJSON_AddItemToObject(root, "content", cJSON_Duplicate(content, 1));
    char* jsonStr = cJSON_PrintUnformatted(root);
    std::string result(jsonStr);
    cJSON_free(jsonStr);
    cJSON_Delete(root);
    return result;
}

Message Message::fromJSONString(const std::string& jsonString) {
    cJSON* root = cJSON_Parse(jsonString.c_str());
    if (!root) throw std::runtime_error("Invalid JSON string");

    cJSON* idNode = cJSON_GetObjectItemCaseSensitive(root, "clientID");
    cJSON* typeNode = cJSON_GetObjectItemCaseSensitive(root, "type");
    cJSON* subTypeNode = cJSON_GetObjectItemCaseSensitive(root, "subType");
    cJSON* contentNode = cJSON_GetObjectItemCaseSensitive(root, "content");

    if (!cJSON_IsNumber(idNode) || !cJSON_IsNumber(typeNode) ||
        !cJSON_IsNumber(subTypeNode) || !cJSON_IsObject(contentNode)) {
        cJSON_Delete(root);
        throw std::runtime_error("Malformed JSON message");
    }

    int id = idNode->valueint;
    auto type = static_cast<MessageType>(typeNode->valueint);
    int subType = subTypeNode->valueint;
    cJSON* contentCopy = cJSON_Duplicate(contentNode, 1);
    cJSON_Delete(root);

    return {id, type, subType, contentCopy};
}
