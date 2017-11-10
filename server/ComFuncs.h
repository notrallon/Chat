#pragma once

#include "Server.h"

struct ComFuncs
{//	Datatype	FunctionName	Parameters											Initialize
	static void SetName			(User* user, Server* server, std::string buffer = std::string('\0'))	{ server->ChangeUsername(user, buffer); }
	static void Disconnect		(User* user, Server* server, std::string buffer = std::string('\0'))	{ server->DisconnectUser(user->GetName());			}
	static void Whisper			(User* user, Server* server, std::string buffer = std::string('\0'))	{ server->WhisperUser	(user, buffer);	}
	static void PrintCommands	(User* user, Server* server, std::string buffer = std::string('\0'))	{ server->PrintCommands	(user);			}
};