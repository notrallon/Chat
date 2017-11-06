#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <thread>
#include <QThread>

#define SERVER_ADRESS "10.96.108.42"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void            on_sendButton_clicked();

    void            on_chatInput_returnPressed();

private:
    Ui::MainWindow* ui;
    QUdpSocket      m_Socket;
    QString         m_Name;
    QString         m_ServerAdress;
    //std::thread*    m_Receive;

    void            InitClient();
    void            SendMessage();
    void            AppendChat(QString message);

    static void     ReceiveMessage(QUdpSocket* socket);
};



#endif // MAINWINDOW_H
