#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTextEdit>
#include <QtNetwork/QUdpSocket>
#include <Qtime>
#include <QMap>
#include <map>
#include <functional>

class SocketThread : public QThread
{
    friend struct ComFuncs;
public:
    void SetChatbox(QTextEdit* box);
    void SetRunning(bool running);
    void SetIp(QString ip);

    const QString& GetIp() const;

    void SendMessage(QString message);
private:
    //using CommandMap = std::map<std::string, std::function<void(std::string, SocketThread*)>>;

    QUdpSocket  m_Socket;
    QTextEdit*  m_Chatbox;
    bool        m_Running;
    QString     m_ServerAdress;
    QTime       m_Time;
    QString     m_Name;
    std::map<std::string, std::function<void(std::string, SocketThread*)>>  m_Commands;


    void run();
    void InitCommands();

    void PrintToChat(QString message);
    void DecodeMessage(QString message);

    void SetName(std::string name);
};

#endif // SOCKETTHREAD_H
