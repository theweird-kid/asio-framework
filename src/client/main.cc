#include "../../headers/net_message.hpp"
#include "../../headers/net_client.hpp"
#include <cstring>

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomClient : public wkd::net::client_interface<CustomMsgTypes>
{
public:
    void PingServer()
	{
		wkd::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		// Caution with this...
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		msg << timeNow;
		Send(msg);
	}

	void MessageAll()
	{
		wkd::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;
		Send(msg);
	}
};

int main()
{
    CustomClient c;
    c.Connect("127.0.0.1", 60000);

    while(c.IsConnected())
    {
        //c.PingServer();
        //c.MessageAll();
        while(!c.Incoming().empty()) {
            auto msg = c.Incoming().pop_front().msg;
            switch(msg.header.id)
            {
                case CustomMsgTypes::ServerAccept:
                {
                    std::cout << "Server Accepted Connection" << std::endl;
                }
                break;
                case CustomMsgTypes::ServerPing:
                {
                    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                    std::chrono::system_clock::time_point timeThen;
                    msg >> timeThen;
                    std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << std::endl;
                }
                break;
                default:
                {
                    std::cout << "Unrecognized message" << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
