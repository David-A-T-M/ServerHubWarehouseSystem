#pragma once

#include <queue>
#include "Message.hpp"
#include "NotificationSystem.hpp"

class MessageDispatcher {
    public:
        MessageDispatcher(std::queue<Message> *queue);


        void ProcessReceivedMessage(Message* msg);

    private:
        std::queue<Message> *pendingMessages;
        // Server *server;
        void processReceivedAlert(Message* msg);


        void processLogin(Message* msg);


        void processLogout(Message* msg);


        void ProcessSubscriptions(Message* msg);


        void ProcessReceivedInventoryRequest(Message* msg);


        void ProcessInventoryInfoRequest(Message* msg);


      void ProcessTransactionHistoryRequest(Message* msg);
};
