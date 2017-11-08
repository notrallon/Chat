#pragma once

#include "Server.h"

struct ComFuncs
{
	static void SetName(std::string buffer, User* user, Server* server);
	static void Disconnect(std::string buffer, User* user, Server* server) { server->DisconnectUser(user); }
	static void Whisper(std::string buffer, User* user, Server* server) { server->WhisperUser(user, buffer); }
};