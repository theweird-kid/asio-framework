#include "../headers/net.hpp"
#include <array>
#include <cstdint>
#include <vector>

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

struct sMessageType
{
    uint32_t nID;
    uint32_t nSize;
    uint32_t nType;
    std::array<uint8_t, 1024> data;

    void copyToServerBuffer(wkd::net::message<GameMsg>& msg, const std::string& message) {
        this->nSize = message.size();
        this->nType = 1;
        std::memcpy(data.data(), message.data(), message.size());
        msg << *this;
    }
};
