#include "common.hpp"
#include <iostream>
#include <thread>

class Client : public wkd::net::client_interface<GameMsg>
{
public:
    void SendMessageToServer(const std::string& message)
    {
        wkd::net::message<GameMsg> msg;
        msg.header.id = GameMsg::Server_Message;

        // Convert the string into a vector of bytes
        std::vector<uint8_t> bodyData(message.begin(), message.end());

        // Create a NetworkMessage and set its body
        NetworkMessage netMsg(MessageType::Text);
        netMsg.setBody(bodyData);

        // Push the NetworkMessage into the message body
        msg << netMsg;
        Send(msg);
    }

    void HandleIncomingMessages()
    {
        while (IsConnected())
        {
            if (!Incoming().empty())
            {
                auto msg = Incoming().pop_front().msg;

                switch (msg.header.id)
                {
                case GameMsg::Client_Accepted:
                {
                    std::cout << "Server accepted connection." << std::endl;

                }
                break;
                case GameMsg::Server_Message:
                {
                    std::string serverMessage;
                    msg >> serverMessage;
                    std::cout << "Server: " << serverMessage << std::endl;
                }
                break;
                default:
                    std::cout << "Unknown message received from server." << std::endl;
                    break;
                }
            }
        }
    }
};

int main()
{
    Client client;
    if (client.Connect("127.0.0.1", 60000))
    {
        std::cout << "Connected to server!" << std::endl;

        // Start a thread to handle incoming messages from the server
        std::thread incomingThread([&client]() {
            client.HandleIncomingMessages();
        });

        // Main loop to take input from the user and send it to the server
        std::string userInput;
        while (true)
        {
            std::cout << "Enter message: ";
            std::getline(std::cin, userInput);
            if (userInput == "exit")
            {
                break;
            }
            client.SendMessageToServer(userInput);
        }

        client.Disconnect();
        incomingThread.join();
    }
    else
    {
        std::cerr << "Failed to connect to server." << std::endl;
    }

    return 0;
}
