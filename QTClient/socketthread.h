#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTextEdit>
#include <QtNetwork/QUdpSocket>

class SocketThread : public QThread
{
public:
    void SetChatbox(QTextEdit* box);
    void SetRunning(bool running);

    void SendMessage(QString message);
private:
    QUdpSocket  m_Socket;
    QTextEdit*  m_Chatbox;
    bool        m_Running;

    void run();

    void PrintToChat(QString message);
};

#endif // SOCKETTHREAD_H
