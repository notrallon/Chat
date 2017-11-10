#include "Server.h"

void Server::ChangeUsername(User* sender, std::string buffer)
{
	// Make sure usernames can't have spaces
	size_t spacepos = buffer.find(' ');
	buffer = buffer.substr(0, spacepos);

	// Get a lowercase version of the name so we can search for it
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

	// Send a command to the client to confirm the namechange
	message = "/setname " + buffer;
	sender->SetName(buffer);
	m_socket.send(message.c_str(), message.length() + 1, sender->GetAdress(), sender->GetPort());

	// Make old and new name lowercase so we can find and emplace
	std::transform(oldName.begin(), oldName.end(), oldName.begin(), ::tolower);

	// Emplace the a new element in the map with our new name
	// and erase the old element.
	m_Users.emplace(newNameLower, sender);
	m_Users.erase(oldName);
}

void Server::DisconnectUser(std::string username)
{
	// Make sure the string is lowercase
	std::transform(username.begin(), username.end(), username.begin(), ::tolower);
	UserMap::iterator it = m_Users.find(username);

	// Delete the user from memory and remove the element from the map
	delete it->second;
	it->second = nullptr;
	m_Users.erase(it);
}

void Server::WhisperUser(User* sender, std::string buffer)
{
	// Get the name of the user the sender want to whisper
	size_t spacePosition = buffer.find(' ');
	std::string targetUsername = buffer.substr(0, spacePosition);
	std::transform(targetUsername.begin(), targetUsername.end(), targetUsername.begin(), ::tolower); // Make sure it's lowercase

	// If we can't find a user with that name, let the sender know
	// and return.
	if (m_Users.find(targetUsername) == m_Users.end())
	{
		std::string message = "Could not find user with name " + targetUsername;
		m_socket.send(message.c_str(), message.size() + 1, sender->GetAdress(), sender->GetPort());
		return;
	}

	// Format the whisper text.
	std::string message = "<font color='#800000ff'><b>Whisper from " + sender->GetName() + ":</b> " + buffer.substr(spacePosition + 1) + "</color>";

	// Get the user we want to whisper
	User* targetUser = m_Users.find(targetUsername)->second;

	// Send the message to the user
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

	for (CommandValType it : m_Commands)
	{
		//Using \n does not give a new line in QT text edit, use "<br> </br>" instead
		//message += "<b>/" + it.first + "</b>" + " -> " + it.second.second + "\n";

		message += "<b>/" + it.first + "</b>" + " -> " + it.second.second + "<br>";
		//Skriver ut alla kommandon i (,jag tror,) bokstavsordning
	}
	m_socket.send(message.c_str(), message.size() + 1, sender->GetAdress(), sender->GetPort());
	
}
