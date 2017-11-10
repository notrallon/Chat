#include "Server.h"
#include "ComFuncs.h"
#include <utility>
#include <algorithm>

HistoryLog Server::sm_historyLog;
static std::vector<std::string> s_commandsStrings;

Server::Server()
{
	m_Commands.emplace		("setname",			std::make_pair(&ComFuncs::SetName,		"Set your username."));
	m_Commands.emplace		("sn",				std::make_pair(&ComFuncs::SetName,		"Set your username."));
	m_Commands.emplace		("disconnect",		std::make_pair(&ComFuncs::Disconnect,	"Disconnect from the server."));
	m_Commands.emplace		("w",				std::make_pair(&ComFuncs::Whisper,		"Whisper another user."));
	m_Commands.emplace		("whisper",			std::make_pair(&ComFuncs::Whisper,		"Whisper another user."));
	m_Commands.emplace		("help",			std::make_pair(&ComFuncs::PrintCommands,"Prints all commands to the user."));
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
		
		FindUser(sender, port, sendingUser, username);

		if (sendingUser == nullptr)
		{
			continue;
		}

		time = clock.getElapsedTime();
		
		sendingUser->SetTime(time);
		CheckUsersConnected(time);

		// TODO: Make commands work
		if (bufferStr[0] == '/') 
		{
			//bool hasValue;
			size_t spacePosition = bufferStr.find(' ');
			std::string command = bufferStr.substr(1, spacePosition - 1);
 
			std::string commandValue = bufferStr.substr(spacePosition + 1);

			if (spacePosition == std::string::npos)
			{
				commandValue = '\0';
			}


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
				std::string message;

				message = "Executing user command: /" + command + " " + commandValue;
				std::cout << message << std::endl;
				CommandIterator commandFunc = m_Commands.find(command);
				commandFunc->second.first(sendingUser, this, commandValue);
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

	for (UserMap::value_type it : m_Users) {
		delete it.second;
		it.second = nullptr;
	}

	m_Users.clear();
}

void Server::CreateNewUser(const sf::IpAddress sender, const uShort port, User*& sendingUser, std::string username, Server* server)
{
	sf::UdpSocket socket;

	socket.bind(sf::UdpSocket::AnyPort);
	socket.setBlocking(false);

	std::string confirm = "/confirm " + std::to_string(socket.getLocalPort());
	socket.send(confirm.c_str(), confirm.length() + 1, sender, port);

	sf::Time time;
	char buffer[256];
	std::size_t received = 0;
	sf::IpAddress keySender;
	unsigned short keyPort;

	sf::Socket::Status status;
	do 
	{
		static sf::Clock clock;
		time = clock.getElapsedTime();
		status = socket.receive(buffer, sizeof(buffer), received, keySender, keyPort);
	} while (time.asSeconds() < 5.0f && !socket.Done);

	if (buffer != SERVER_KEY)
	{
		return;
	}

	User* newUser = new User();
	newUser->SetAdress(sender);
	newUser->SetPort(port);
	newUser->SetName(username);

	std::string tempUserInfo = std::string(newUser->UserInfo());
	sm_historyLog.AddTextLog("Users", tempUserInfo);

	sendingUser = newUser;

	server->AddUser(newUser);

	return;
}

void Server::AddUser(User* user)
{
	std::string userLowercase = user->GetName();
	std::transform(userLowercase.begin(), userLowercase.end(), userLowercase.begin(), ::tolower);
	m_Users.emplace(userLowercase, user);
}

void Server::FindUser(const sf::IpAddress sender, const uShort port, User *& sendingUser, std::string username)
{
	std::string userLowercase = username;
	std::transform(userLowercase.begin(), userLowercase.end(), userLowercase.begin(), ::tolower);
	if (m_Users.find(userLowercase) != m_Users.end())
	{
		sendingUser = m_Users.find(userLowercase)->second;
		return;
	}

	// Start a new thread that creates a user and checks for the correct key.
	m_CreateUserThread = new std::thread(CreateNewUser, sender, port, std::ref(sendingUser), username, this);
	m_CreateUserThread->join();
}

void Server::SendToAll(std::string message)
{
	if (message.empty())
	{
		return;
	}

	for (UserMap::value_type it : m_Users)
	{
		m_socket.send(message.c_str(), message.size() + 1, it.second->GetAdress(), it.second->GetPort());
		sm_historyLog.AddTextLog("ServerSent", message);
	}
}

void Server::CheckUsersConnected(sf::Time time)
{
	UserMap::iterator ito = std::begin(m_Users);

	while (ito != std::end(m_Users))
	{
		float timediff = time.asSeconds() - ito->second->GetTime().asSeconds();
		if (timediff > 10) 
		{
			SendToAll("User timed out: " + ito->second->GetName());
			std::cout << "User timed out: " << ito->second->GetName() << std::endl;
			DisconnectUser(ito->first);
			ito = std::begin(m_Users); //ito iteration starts from the beginning
			continue;
		}
		else
		{
			ito++;
		}
	}
}
