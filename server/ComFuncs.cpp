#include "ComFuncs.h"

void ComFuncs::SetName(std::string buffer, User * user, Server * server)
{
	server->ChangeUsername(user, buffer);
}
