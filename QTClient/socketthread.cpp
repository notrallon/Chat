#include "socketthread.h"
#include <QtNetwork/QHostAddress>
#include <QScrollBar>


#define SERVER_ADRESS "10.96.108.57"

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
    m_Time.start();
    int time = 0;
    m_Socket.bind();
    m_ServerAdress = SERVER_ADRESS;
    std::string message = "/setname UnnamedPlayer";
    m_Socket.writeDatagram(&message[0], message.length() + 1, QHostAddress(m_ServerAdress), SERVER_PORT);
    m_Running = true;
    while (m_Running)
    {
        char buffer[1024];
        QHostAddress sender;
        quint16 port;
        if (m_Socket.waitForReadyRead(5000))
        {
            m_Socket.readDatagram(buffer, sizeof(buffer), &sender, &port);
            PrintToChat(QString(buffer));
        }

        time += m_Time.restart();

        if (time > 5000)
        {
            SendMessage('\0');
            time -= 5000;
        }
    }
}

void SocketThread::SendMessage(QString message)
{
    m_Socket.writeDatagram(message.toStdString().c_str(), message.length() + 1, QHostAddress(m_ServerAdress), SERVER_PORT);
}

void SocketThread::PrintToChat(QString message)
{
    m_Chatbox->append(message);
    m_Chatbox->textCursor().movePosition(QTextCursor::End);
    m_Chatbox->ensureCursorVisible();
}
