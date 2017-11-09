#include "Server.h"
#include "ComFuncs.h"
#include <utility>
#include <algorithm>

HistoryLog Server::sm_historyLog;

Server::Server()
{
	m_Commands.emplace("setname", &ComFuncs::SetName);
	m_Commands.emplace("sn", &ComFuncs::SetName);
	m_Commands.emplace("disconnect", &ComFuncs::Disconnect);
	m_Commands.emplace("w", &ComFuncs::Whisper);
	m_Commands.emplace("whisper", &ComFuncs::Whisper);
}

Server::~Server()
{
}

void Server::Run()
{
	// ----- The server -----
	// Create a socket and bind it to the port 55002
	m_socket.bind(55002);

	sf::Clock clock;
	sf::Time time;

	clock.restart();

	while (true) {
		// Receive a message from anyone
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;

		m_socket.receive(buffer, sizeof(buffer), received, sender, port);

		User* sendingUser = nullptr;
		std::string bufferStr = buffer;

		size_t spacePos = bufferStr.find(' ');
		std::string username = bufferStr.substr(0, spacePos);
		bufferStr = bufferStr.substr(spacePos + 1);
		
		CreateUser(sender, port, sendingUser, username);

		time = clock.getElapsedTime();
		sendingUser->SetTime(time);
		CheckUsersConnected(time);

		// TODO: Make commands work
		if (bufferStr[0] == '/') 
		{
			size_t spacePosition = bufferStr.find(' ');
			std::string command = bufferStr.substr(1, spacePosition - 1);

			if (command.size() + 1 == bufferStr.size())
			{
				continue;
			}
			std::string commandValue = bufferStr.substr(spacePosition + 1);

			// Example: /setname Richard <--- setname = command, Richard = value
			// so result is command->second("Richard") -- Calls ComFuncs::SetName("Richard")
			// TODO : Loop through the buffer until blankspace and save it into a string called "command"
			if (m_Commands.find(command) == m_Commands.end())
			{
				std::string message = "Error! Could not find command: /" + command;
				std::cout << message << std::endl;
				m_socket.send(message.c_str(), message.size() + 1, sendingUser->GetAdress(), sendingUser->GetPort());
				continue;
			}
			else
			{
				std::string message = "Executing user command: /" + command + " " + commandValue;
				std::cout << message << std::endl;
				auto commandFunc = m_Commands.find(command);
				//m_Commands.find(command)->second(commandValue, sendingUser, this);
				// Loop through next word in the buffer until a new blankspace and save it into a string called "value"
				commandFunc->second(commandValue, sendingUser, this);
				continue;
			}
		}

		std::string logMessage = std::string(buffer);
		std::string logFull	= std::string(sendingUser->UserInfo() + "[Message:" + logMessage + "]");

		sm_historyLog.AddTextLog("ServerReceived", logFull);


		// Send an answer
		std::stringstream mstream;
		mstream.clear();
		if (!bufferStr.empty() && sendingUser != nullptr) {
			mstream << sendingUser->GetName() << " said: " << bufferStr;
			std::string message = "<b>" + sendingUser->GetName() + " said:</b> " + bufferStr;

			//Prints timestamp and message to server console
			std::cout << "[" << sm_historyLog.GetTimeStamp() << "]" << message << std::endl;

			SendToAll(message);
		}
	}

	for (auto it : m_Users) {
		delete it.second;
		it.second = nullptr;
	}

	m_Users.clear();
}

void Server::CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser)
{

	if (m_Users.find("username") != m_Users.end())
	{
		// user = m_Users.find(username)->second;
		// return;
	}

	// else create a new user


	//if users exist
	if (m_Users.size() > 0)
	{
		for (auto it = m_Users.begin(); it != m_Users.end(); it++)
		{
			User* user = it->second;
			//if it is the user sending the message. sendingUser is the user
			if (user->GetAdress() == sender && user->GetPort() == port)
			{
				sendingUser = user;
				return;
			}
		}

		User* newUser = new User();
		newUser->SetAdress(sender);
		newUser->SetPort(port);
		newUser->SetName("Anon" + std::to_string(m_Users.size() + 1));
		std::string username = newUser->GetName();
		std::transform(username.begin(), username.end(), username.begin(), ::tolower);

		m_Users.emplace(username, newUser);

		std::string tempUserInfo = std::string(newUser->UserInfo());
		sm_historyLog.AddTextLog("Users", tempUserInfo);

		sendingUser = newUser;
		return;
	}
	//if no user exists
	else
	{
		User* newUser = new User();
		newUser->SetAdress(sender);
		newUser->SetPort(port);
		newUser->SetName("Anon1");
		std::string username = newUser->GetName();
		std::transform(username.begin(), username.end(), username.begin(), ::tolower);

		m_Users.emplace(username, newUser);

		std::string tempUserInfo = std::string(newUser->UserInfo());
		sm_historyLog.AddTextLog("Users", tempUserInfo);

		sendingUser = newUser;
		return;
	}
}

void Server::CreateUser(const sf::IpAddress sender, const unsigned short port, User *& sendingUser, std::string username)
{
	std::string userLowercase = username;
	std::transform(userLowercase.begin(), userLowercase.end(), userLowercase.begin(), ::tolower);
	if (m_Users.find(userLowercase) != m_Users.end())
	{
		sendingUser = m_Users.find(userLowercase)->second;
		return;
	}

	User* newUser = new User();
	newUser->SetAdress(sender);
	newUser->SetPort(port);
	newUser->SetName(username);
	

	m_Users.emplace(userLowercase, newUser);

	std::string tempUserInfo = std::string(newUser->UserInfo());
	sm_historyLog.AddTextLog("Users", tempUserInfo);

	sendingUser = newUser;
	return;
}

void Server::DisconnectUser(User * user)
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

void Server::ChangeUsername(User * sender, std::string buffer)
{
	// Make sure usernames can't have spaces
	size_t spacepos = buffer.find(' ');
	buffer = buffer.substr(0, spacepos);


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
	std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
	
	m_Users.emplace(buffer, sender);
	m_Users.erase(oldName);
}

void Server::SendToAll(std::string message)
{
	if (message.empty())
	{
		return;
	}

	for (auto it : m_Users)
	{
		m_socket.send(message.c_str(), message.size() + 1, it.second->GetAdress(), it.second->GetPort());
		sm_historyLog.AddTextLog("ServerSent", message);
	}
}

void Server::CheckUsersConnected(sf::Time time)
{
	for (auto it : m_Users)
	{
		float timediff = time.asSeconds() - it.second->GetTime().asSeconds();
		if (timediff > 10)
		{
			SendToAll("User timed out: " + it.second->GetName());
			std::cout << "User timed out: " << it.second->GetName() << std::endl;
			DisconnectUser(it.second);
		}
	}
}
