#include "Server.h"
#include "ComFuncs.h"
#include <utility>
#include <algorithm>

HistoryLog Server::sm_historyLog;
static std::vector<std::string> s_commandsStrings;

Server::Server()
{
	m_ValueCommands.emplace		("setname",			std::make_pair(&ComFuncs::SetName,		"Set your username."));
	m_ValueCommands.emplace		("sn",				std::make_pair(&ComFuncs::SetName,		"Set your username."));
	m_Commands.emplace			("disconnect",			std::make_pair(&ComFuncs::Disconnect,	"Disconnect from the server."));
	m_ValueCommands.emplace		("w",				std::make_pair(&ComFuncs::Whisper,		"Whisper another user."));
	m_ValueCommands.emplace		("whisper",			std::make_pair(&ComFuncs::Whisper,		"Whisper another user."));
	m_Commands.emplace			("help",					std::make_pair(&ComFuncs::PrintCommands,"Prints all commands to the user."));
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
			bool hasValue;
			size_t spacePosition = bufferStr.find(' ');
			std::string command = bufferStr.substr(1, spacePosition - 1);

			//Ignores commands without space after it. (example: '/help' is ignored, but not '/help ')
			//if (command.size() + 1 == bufferStr.size())
			//{
				//continue;
			//}

			//if spaceposition does not find its value. find returns string::npos
			if (spacePosition == std::string::npos)
				hasValue = false;
			else
				hasValue = true;
 
			std::string commandValue = bufferStr.substr(spacePosition + 1);


			// Example: /setname Richard <--- setname = command, Richard = value
			// so result is command->second("Richard") -- Calls ComFuncs::SetName("Richard")
			// TODO : Loop through the buffer until blankspace and save it into a string called "command"
			if (m_ValueCommands.find(command) == m_ValueCommands.end() && m_Commands.find(command) == m_Commands.end())
			{
				std::string message = "Error! Could not find command: /" + command;
				std::cout << message << std::endl;
				m_socket.send(message.c_str(), message.size() + 1, sendingUser->GetAdress(), sendingUser->GetPort());
				continue;
			}
			else
			{
				std::string message;
				if (hasValue)
				{
					message = "Executing user command: /" + command + " " + commandValue;
					std::cout << message << std::endl;
					auto commandFunc = m_ValueCommands.find(command);
					//m_Commands.find(command)->second(commandValue, sendingUser, this);
					// Loop through next word in the buffer until a new blankspace and save it into a string called "value"
					commandFunc->second.first(commandValue, sendingUser, this);
				}
				else
				{
					message = "Executing user command: /" + command;

					std::cout << message << std::endl;
					auto commandFunc = m_Commands.find(command);
					//m_Commands.find(command)->second(commandValue, sendingUser, this);
					// Loop through next word in the buffer until a new blankspace and save it into a string called "value"
					commandFunc->second.first(sendingUser, this);
				}

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
	auto ito = std::begin(m_Users);

	while (ito != std::end(m_Users))
	{

		float timediff = time.asSeconds() - ito->second->GetTime().asSeconds();
		if (timediff > 10)
		{
			SendToAll("User timed out: " + ito->second->GetName());
			std::cout << "User timed out: " << ito->second->GetName() << std::endl;
			DisconnectUser(ito->second);
			ito = std::begin(m_Users); //ito iteration starts from the beginning
			continue;
		}
		else
			ito++;
	}

	//old?
	/*for (auto it = m_Users.begin(); it != m_Users.end(); )
	{
		float timediff = time.asSeconds() - it->second->GetTime().asSeconds();
		if (timediff > 10)
		{
			SendToAll("User timed out: " + it->second->GetName());
			std::cout << "User timed out: " << it->second->GetName() << std::endl;
			DisconnectUser(it->second);
		}
		else 
		{
			it++;
		}
	}*/
}
