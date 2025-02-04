#include "common.hpp"
#include <cstdint>
#include <string>

class GameServer : public wkd::net::server_interface<GameMsg>
{
public:
	GameServer(uint16_t nPort) : wkd::net::server_interface<GameMsg>(nPort)
	{
	}

	std::unordered_map<uint32_t, sMessageType> m_mapPlayerRoster;
	std::vector<uint32_t> m_vGarbageIDs;

protected:
	bool OnClientConnect(std::shared_ptr<wkd::net::connection<GameMsg>> client) override
	{
		// For now we will allow all
		return true;
	}

	void OnClientValidated(std::shared_ptr<wkd::net::connection<GameMsg>> client) override
	{
		// Client passed validation check, so send them a message informing
		// them they can continue to communicate
		wkd::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Client_Accepted;
		client->Send(msg);
	}

	void OnClientDisconnect(std::shared_ptr<wkd::net::connection<GameMsg>> client) override
	{

	}

	void OnMessage(std::shared_ptr<wkd::net::connection<GameMsg>> client, wkd::net::message<GameMsg>& msg) override
	{
		switch (msg.header.id)
		{
		case GameMsg::Server_Message:
        {
            std::cout << "[" << client->GetID() << "]: " << msg << std::endl;

            // Extract the NetworkMessage from the received message
            sMessageType netMsg;
            msg >> netMsg;

            std::string clientMessage{netMsg.data.begin(), netMsg.data.begin() + netMsg.nSize};
            std::cout << "Received message from client: " << clientMessage << std::endl;
		}
		break;
		}
	}
};

int main()
{
	GameServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}
