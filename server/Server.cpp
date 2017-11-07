#include "Server.h"
#include <utility>


HistoryLog Server::sm_historyLog;

Server::Server()
{
	m_Commands.emplace("setname", &ComFuncs::SetName);
	m_Commands.emplace("sn", &ComFuncs::SetName);
	m_Commands.emplace("disconnect", &ComFuncs::Disconnect);
}


Server::~Server()
{
}

void Server::Run()
{
	// ----- The server -----
	// Create a socket and bind it to the port 55002
	m_socket.bind(55002);

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



		//	std::string command = "";
			/*for (int i = 1; i < sizeof(buffer); i++)
			{
				if (buffer[i] == ' ')
				{
					for (int j = i + 1; j < sizeof(buffer); j++) 
					{
						if (buffer[j] == ' ')
						{
							break; // j
						}
						commandValue += buffer[j];
					}
					break; // i
				}
				command += buffer[i];
			}*/

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
			mstream << sendingUser->GetName() << " said: " << buffer << std::endl;
			std::string message = mstream.str();

			//Prints timestamp and message to server console
			std::cout << "[" << sm_historyLog.GetTimeStamp() << "]" << message << std::endl;

			for (int i = 0; i < m_connectedUsers.size(); i++)
			{
				//Edit Note: Removed ->GetAdress and replaced it with ->GetName (replacing sender with sendingUser)		<-REMOVE THIS COMMENT<-
				m_socket.send(message.c_str(), message.size() + 1, m_connectedUsers[i]->GetAdress(), m_connectedUsers[i]->GetPort());
				sm_historyLog.AddTextLog("ServerSent", message);
			}
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
		}
		else
		{
			it++;
		}
	}
}
