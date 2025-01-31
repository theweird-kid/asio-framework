#include <cstdint>
#include <iostream>
#include <string>

// Include necessary headers
#include "../../headers/net_message.hpp"
#include "../../headers/net_server.hpp"

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

// Create a custom server
class CustomServer : public wkd::net::server_interface<CustomMsgTypes>
{
public:
    CustomServer(uint16_t nPort) : wkd::net::server_interface<CustomMsgTypes>(nPort) {}

protected:
    virtual bool OnClientConnect(std::shared_ptr<wkd::net::connection<CustomMsgTypes>> client) override
    {
        wkd::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerAccept;
        client->Send(msg);
        return true;
    }

    virtual void OnClientDisconnect(std::shared_ptr<wkd::net::connection<CustomMsgTypes>> client) override
    {
        wkd::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerMessage;
        msg << "Server: Client Disconnected.";
        MessageAllClients(msg, client);
    }

    virtual void OnMessage(std::shared_ptr<wkd::net::connection<CustomMsgTypes>> client, wkd::net::message<CustomMsgTypes>& msg) override
    {
        switch (msg.header.id)
        {
        case CustomMsgTypes::ServerPing:
        {
            std::cout << "[" << client->GetID() << "]: Server Ping\n";
            client->Send(msg);
        }
        break;
        case CustomMsgTypes::MessageAll:
        {
            std::cout << "[" << client->GetID() << "]: Message All\n";
            wkd::net::message<CustomMsgTypes> msg;
            msg.header.id = CustomMsgTypes::ServerMessage;
            msg << "Server: Message All";
            MessageAllClients(msg, client);
        }
        case CustomMsgTypes::ServerMessage:
        {
            std::cout << "[" << client->GetID() << "]: Server Message\n";
            std::cout << "Message: " << msg.body.data() << "\n";
        }
        break;
        }
    }
};

int main(int argc, char* argv[]) {
    //std::string ip = "192.168.1.80";
    CustomServer server(60000);
    server.Start();

    while (1)
    {
        server.Update(-1, true);
    }

    return 0;
}
