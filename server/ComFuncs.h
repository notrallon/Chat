#pragma once

#include "Server.h"

struct ComFuncs
{//	Datatype	FunctionName	Parameters											Initialize
	static void SetName			(std::string buffer, User* user, Server* server)	{ server->ChangeUsername(user, buffer); }
	static void Disconnect		(User* user, Server* server)	{ server->DisconnectUser(user);			}
	static void Whisper			(std::string buffer, User* user, Server* server)	{ server->WhisperUser	(user, buffer);	}
	static void PrintCommands	(User* user, Server* server)	{ server->PrintCommands	(user);			}
};