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
		CreateUser(sender, port, sendingUser);

		time = clock.getElapsedTime();
		sendingUser->SetTime(time);
		CheckUsersConnected(time);

		//w Name MEssage bla bla bla

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
			mstream << sendingUser->GetName() << " said: " << buffer;
			std::string message = "<b>" + sendingUser->GetName() + " said:</b> " + buffer;

			//Prints timestamp and message to server console
			std::cout << "[" << sm_historyLog.GetTimeStamp() << "]" << message << std::endl;

			SendToAll(message);
		}
	}

	for (auto it : m_connectedUsers) {
		delete it;
		it = nullptr;
	}

	m_connectedUsers.clear();
}

void Server::CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser)
{
	//if users exist
	if (m_connectedUsers.size() > 0)
	{
		for (int i = 0; i < m_connectedUsers.size(); i++)
		{
			//if it is the user sending the message. sendingUser is the user
			if (m_connectedUsers[i]->GetAdress() == sender && m_connectedUsers[i]->GetPort() == port)
			{
				sendingUser = m_connectedUsers[i];
				break;
			}
			//if no user was the sender. Create new user
			else if (i == m_connectedUsers.size() - 1)
			{
				User* newUser = new User();
				newUser->SetAdress(sender);
				newUser->SetPort(port);
				newUser->SetName("Anon" + std::to_string(m_connectedUsers.size() + 1));
				m_connectedUsers.push_back(newUser);

				std::string tempUserInfo = std::string(newUser->UserInfo());
				sm_historyLog.AddTextLog("Users", tempUserInfo);

				sendingUser = newUser;
			}
		}
	}
	//if no user exists
	else
	{
		User* newUser = new User();
		newUser->SetAdress(sender);
		newUser->SetPort(port);
		newUser->SetName("Anon1");
		m_connectedUsers.push_back(newUser);

		std::string tempUserInfo = std::string(newUser->UserInfo());
		sm_historyLog.AddTextLog("Users", tempUserInfo);

		sendingUser = newUser;
	}
}

void Server::DisconnectUser(User * user)
{
	for (auto it = m_connectedUsers.begin(); it != m_connectedUsers.end(); ) 
	{
		if (*it == user)
		{
			delete *it;
			it = m_connectedUsers.erase(it);
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

	std::string message = "<font color='#800000ff'><b>Whisper from " + sender->GetName() + ":</b> " + buffer.substr(spacePosition + 1) + "</color>";

	for (int i = 0; i < m_connectedUsers.size(); i++)
	{
		std::string currentUsername = m_connectedUsers[i]->GetName();
		std::transform(currentUsername.begin(), currentUsername.end(), currentUsername.begin(), ::tolower);
		if (targetUsername == currentUsername)
		{
			m_socket.send(message.c_str(), message.size() + 1, m_connectedUsers[i]->GetAdress(), m_connectedUsers[i]->GetPort());
			sm_historyLog.AddTextLog("ServerSent", message);
		}
	}

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
	std::string message = sender->GetName() + " changed name to " + buffer;
	SendToAll(message);

	sender->SetName(buffer);
}

void Server::SendToAll(std::string message)
{
	if (message.empty())
	{
		return;
	}

	for (int i = 0; i < m_connectedUsers.size(); i++)
	{
		m_socket.send(message.c_str(), message.size() + 1, m_connectedUsers[i]->GetAdress(), m_connectedUsers[i]->GetPort());
		sm_historyLog.AddTextLog("ServerSent", message);
	}
}

void Server::CheckUsersConnected(sf::Time time)
{
	for (int i = 0; i < m_connectedUsers.size(); i++)
	{
		float timediff = time.asSeconds() - m_connectedUsers[i]->GetTime().asSeconds();
		if (timediff > 10)
		{
			SendToAll("User timed out: " + m_connectedUsers[i]->GetName());
			std::cout << "User timed out: " << m_connectedUsers[i]->GetName() << std::endl;
			DisconnectUser(m_connectedUsers[i]);
			i--;
		}
	}
}
