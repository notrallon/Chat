#include "socketthread.h"
#include <QtNetwork/QHostAddress>


#define SERVER_ADRESS "10.96.108.41"

void SocketThread::SetChatbox(QTextEdit *box)
{
    m_Chatbox = box;
}

void SocketThread::SetRunning(bool running)
{
    m_Running = running;
}

void SocketThread::run()
{
    m_Socket.bind();
    m_Running = true;
    while (m_Running)
    {
        char buffer[1024];
        QHostAddress sender;
        quint16 port;
        if (m_Socket.waitForReadyRead(500))
        {
            m_Socket.readDatagram(buffer, sizeof(buffer), &sender, &port);
            PrintToChat(QString(buffer));
        }
    }
}

void SocketThread::SendMessage(QString message)
{
    m_Socket.writeDatagram(message.toStdString().c_str(), message.length() + 1, QHostAddress(SERVER_ADRESS), 55002);
}

void SocketThread::PrintToChat(QString message)
{
    m_Chatbox->append(message);
}
