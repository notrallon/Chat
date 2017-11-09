#include "socketthread.h"
#include <QtNetwork/QHostAddress>
#include <QScrollBar>
#include <utility>
#include <time.h>
#include "comfuncs.h"


#define SERVER_ADRESS "10.96.108.54"

const quint16 SERVER_PORT = 55002;

void SocketThread::SetChatbox(QTextEdit *box)
{
    m_Chatbox = box;
}

void SocketThread::SetRunning(bool running)
{
    m_Running = running;
}

void SocketThread::SetIp(QString ip)
{
    m_ServerAdress = ip;
}

const QString& SocketThread::GetIp() const
{
    return m_ServerAdress;
}

void SocketThread::run()
{
    srand(time(NULL));
    InitCommands();
    m_Time.start();
    static int s_time = 0;
    m_Socket.bind();
    m_ServerAdress = SERVER_ADRESS;
    m_Name = QString::fromStdString("Anon" + std::to_string(rand() % 1000));
    //std::string message = "/setname UnnamedPlayer";
    //m_Socket.writeDatagram(&message[0], message.length() + 1, QHostAddress(m_ServerAdress), SERVER_PORT);
    m_Running = true;
    while (m_Running)
    {
        char buffer[1024];
        QHostAddress sender;
        quint16 port;
        if (m_Socket.waitForReadyRead(5000))
        {
            m_Socket.readDatagram(buffer, sizeof(buffer), &sender, &port);
            DecodeMessage(buffer);

        }

        s_time += m_Time.restart();

        if (s_time > 5000)
        {
            //SendMessage('\0');
            s_time -= 5000;
        }
    }
}

void SocketThread::InitCommands()
{
    m_Commands.emplace("setname", &ComFuncs::SetName);
}

void SocketThread::SendMessage(QString message)
{
    message = m_Name + " " + message;
    m_Socket.writeDatagram(message.toStdString().c_str(), message.length() + 1, QHostAddress(m_ServerAdress), SERVER_PORT);
}

void SocketThread::PrintToChat(QString message)
{
    m_Chatbox->append(message);
    m_Chatbox->textCursor().movePosition(QTextCursor::End);
    m_Chatbox->ensureCursorVisible();
}

void SocketThread::DecodeMessage(QString message)
{
    std::string stdMessage = message.toStdString();

    if (stdMessage[0] == '/')
    {
        uint spacePos = stdMessage.find(' ');
        std::string command = stdMessage.substr(1, spacePos - 1);

        std::string commandValue = stdMessage.substr(spacePos + 1);

        auto commandFunc = m_Commands.find(command);
        commandFunc->second(commandValue, this);
        return;
    }

    PrintToChat(message);
}

void SocketThread::SetName(std::string name)
{
    m_Name = QString::fromStdString(name);
}
