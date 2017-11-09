#include "Server.h"

void Server::ChangeUsername(User* sender, std::string buffer)
{
	// Make sure usernames can't have spaces
	size_t spacepos = buffer.find(' ');
	buffer = buffer.substr(0, spacepos);

	std::string newNameLower = buffer;
	std::transform(newNameLower.begin(), newNameLower.end(), newNameLower.begin(), ::tolower);

	// If there's already a user with that name
	if (m_Users.find(newNameLower) != m_Users.end())
	{
		std::string message = "Error! Found other user with name " + buffer;
		m_socket.send(message.c_str(), message.size() + 1, sender->GetAdress(), sender->GetPort());
		return;
	}

	// If the string isn't empty by now we set the new name of the user
	if (buffer.empty())
	{
		std::string message = "Error! Could not change username. Field is empty.";
		m_socket.send(message.c_str(), message.size() + 1, sender->GetAdress(), sender->GetPort());
		return;
	}
	// Notify all users that a user changed name.
	std::string oldName = sender->GetName();
	std::string message = oldName + " changed name to " + buffer;
	SendToAll(message);

	// Send a command to the client to confirm it can change username
	message = "/setname " + buffer;
	sender->SetName(buffer);
	m_socket.send(message.c_str(), message.length() + 1, sender->GetAdress(), sender->GetPort());

	// Make old and new name lowercase so we can find and emplace
	std::transform(oldName.begin(), oldName.end(), oldName.begin(), ::tolower);

	m_Users.emplace(newNameLower, sender);
	m_Users.erase(oldName);
}

void Server::DisconnectUser(User* user)
{
	for (auto it = m_Users.begin(); it != m_Users.end(); )
	{
		if (it->second == user)
		{
			delete it->second;
			it = m_Users.erase(it);
			break;
		}
		else
		{
			it++;
		}
	}
}

void Server::WhisperUser(User* sender, std::string buffer)
{
	size_t spacePosition = buffer.find(' ');
	std::string targetUsername = buffer.substr(0, spacePosition);
	std::transform(targetUsername.begin(), targetUsername.end(), targetUsername.begin(), ::tolower);

	if (m_Users.find(targetUsername) == m_Users.end())
	{
		std::string message = "Could not find user with name " + targetUsername;
		m_socket.send(message.c_str(), message.size() + 1, sender->GetAdress(), sender->GetPort());
		return;
	}

	std::string message = "<font color='#800000ff'><b>Whisper from " + sender->GetName() + ":</b> " + buffer.substr(spacePosition + 1) + "</color>";


	User* targetUser = m_Users.find(targetUsername)->second;
	m_socket.send(message.c_str(), message.size() + 1, targetUser->GetAdress(), targetUser->GetPort());
	sm_historyLog.AddTextLog("ServerSent", message);

	// Let sender know he whispered
	message = "<font color='#800000ff'><b>Whisper to " + targetUsername + ":</b> " + buffer.substr(spacePosition + 1) + "</color>";
	m_socket.send(message.c_str(), message.size() + 1, sender->GetAdress(), sender->GetPort());
}

void Server::PrintCommands(User* sender)
{
	//TODO: Fix bug: Probably needs to be fixed in QTClient; a few lines become invisible on print, gets revealed on marking text with cursor.

	std::string message = "";

	for (auto it : m_ValueCommands)
	{
		//Using \n does not give a new line in QT text edit, use "<br> </br>" instead
		//message += "<b>/" + it.first + "</b>" + " -> " + it.second.second + "\n";

		message += "<br><b>/" + it.first + "</b>" + " -> " + it.second.second + "</br>";
		//Skriver ut alla kommandon i (,jag tror,) bokstavsordning
	}
	for (auto it : m_Commands)
	{
		//Using \n does not give a new line in QT text edit, use "<br> </br>" instead
		//message += "<b>/" + it.first + "</b>" + " -> " + it.second.second + "\n";

		message += "<br><b>/" + it.first + "</b>" + " -> " + it.second.second + "</br>";
		//Skriver ut alla kommandon i (,jag tror,) bokstavsordning
	}
	m_socket.send(message.c_str(), message.size() + 1, sender->GetAdress(), sender->GetPort());
	
}
