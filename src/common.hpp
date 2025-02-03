#include "../headers/net.hpp"
#include <cstdint>

enum class GameMsg : uint32_t
{
    Server_Accept,
    Server_Deny,
    Server_Ping,
    Message_All,
    Server_Message,
    Client_Accepted,
    Client_RegisterWithServer,
    Client_UnregisterWithServer,
    Client_Acknowledge,
    Client_UpdatePosition,
    Client_UpdateAnimation,
    Client_UpdateHealth,
    Client_UpdateAmmo,
    Client_UpdateScore,
    Client_UpdateDeath,
    Client_UpdateKill,
    Client_UpdateRadius,
    Client_UpdateAvatar,
    Client_UpdateName,
    Client_UpdateChat,
    Client_UpdateTeam,
    Client_UpdateGame,
    Client_UpdateGameEnd,
    Client_UpdateGameStart,
    Client_UpdateGameReset,
    Client_UpdateGamePause,
    Client_UpdateGameResume,
    Client_UpdateGameAbort,
    Client_UpdateGameRestart,
    Client_UpdateGameLobby,
    Client_UpdateGameLobbyStart,
    Client_UpdateGameLobbyEnd,
    Client_UpdateGameLobbyAbort,
    Client_UpdateGameLobbyRestart,
    Client_UpdateGameLobbyPause,
    Client_UpdateGameLobbyResume,
    Client_UpdateGameLobbyReset,
    Client_UpdateGameLobbyChat,
    Client_UpdateGameLobbyTeam,
    Client_UpdateGameLobbyName,
    Client_UpdateGameLobbyAvatar,
    Client_UpdateGameLobbyReady,
    Client_UpdateGameLobbyUnready,
    Client_UpdateGameLobbyStartGame,
    Client_UpdateGameLobbyEndGame,
    Client_UpdateGameLobbyAbortGame,
    Client_UpdateGameLobbyRestartGame,
    Client_UpdateGameLobbyPauseGame,
    Client_UpdateGameLobbyResumeGame,
    Client_UpdateGameLobbyResetGame,
    Client_UpdateGameLobbyChatGame,
    Client_UpdateGameLobbyTeamGame,
    Client_UpdateGameLobbyNameGame,
    Client_UpdateGameLobbyAvatarGame,
    Client_UpdateGameLobbyReadyGame,
    Client_UpdateGameLobbyUnreadyGame,
    Client_UpdateGameLobbyStartGameGame,
    Client_UpdateGameLobbyEndGameGame,
    Client_UpdateGameLobbyAbortGameGame,
    Client_UpdateGameLobbyRestartGameGame,
    Client_UpdateGameLobbyPauseGameGame,
    Client_UpdateGameLobbyResumeGameGame,
    Client_UpdateGameLobbyResetGameGame,
    Client_UpdateGameLobbyChatGameGame,
    Client_UpdateGameLobbyTeamGameGame,
    Client_UpdateGameLobbyNameGameGame,
    Client_UpdateGameLobbyAvatarGameGame,
    Client_UpdateGameLobbyReadyGameGame,
    Client_UpdateGameLobbyUnreadyGameGame,
    Client_UpdateGameLobbyStartGameGameGame,
    Client_UpdateGameLobbyEndGameGameGame,
    Client_UpdateGameLobbyAbortGameGameGame,
    Client_UpdateGameLobbyRestartGameGameGame,
    Client_UpdateGameLobbyPauseGameGameGame
};

enum class MessageType
{
    Text,
    Binary,
    Command,
};

struct NetworkMessage
{
    MessageType type; // Type of the message
    uint32_t size;    // Size of the message body
    std::vector<uint8_t> body; // Message body containing raw bytes

    // Constructor to initialize the message
    NetworkMessage(MessageType t = MessageType::Text, uint32_t s = 0)
        : type(t), size(s) {}

    // Function to set the body and update the size
    void setBody(const std::vector<uint8_t>& data)
    {
        body = data;
        size = static_cast<uint32_t>(body.size());
    }

    // Function to get the size of the entire message
    size_t getSize() const
    {
        return sizeof(type) + sizeof(size) + body.size();
    }
};

struct sMessageType
{
	char data[1024];
};
