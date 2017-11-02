#include "ServerStuff.h"


HistoryLog ServerStuff::sm_historyLog;

ServerStuff::ServerStuff()
{
}


ServerStuff::~ServerStuff()
{
}

void ServerStuff::AllServerStuff()
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

		CreateUser(sender, port, sendingUser);

		std::string* logMessage	= new std::string(buffer);
		std::string* logFull	= new std::string(sendingUser->UserInfo() + "[Message:" + *logMessage + "]");

		sm_historyLog.AddTextLog("ServerReceived", *logFull);

		std::string bufferStr = buffer;

		// Send an answer
		std::stringstream mstream;
		mstream.clear();
		if (!bufferStr.empty() && sendingUser != nullptr) {
			mstream << sendingUser->GetName() << " said: " << buffer << std::endl;
			std::string message = mstream.str();

			std::cout << message;
			for (int i = 0; i < m_connectedUsers.size(); i++)
			{
				//Edit Note: Removed ->GetAdress and replaced it with ->GetName (replacing sender with sendingUser)		<-REMOVE THIS COMMENT<-
				if (!message.empty() && (m_connectedUsers[i]->GetName() != sendingUser->GetName() && m_connectedUsers[i]->GetPort() != port))
				{
					m_socket.send(message.c_str(), message.size() + 1, m_connectedUsers[i]->GetAdress(), m_connectedUsers[i]->GetPort());
					sm_historyLog.AddTextLog("ServerSent", message);
				}
			}
		}
	}

	for (auto it : m_connectedUsers) {
		delete it;
		it = nullptr;
	}

	m_connectedUsers.clear();
}

void ServerStuff::CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser)
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

				std::string* tempUserInfo = new std::string(newUser->UserInfo());
				sm_historyLog.AddTextLog("Users", *tempUserInfo);

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

		std::string* tempUserInfo = new std::string(newUser->UserInfo());
		sm_historyLog.AddTextLog("Users", *tempUserInfo);

		sendingUser = newUser;
	}
}
