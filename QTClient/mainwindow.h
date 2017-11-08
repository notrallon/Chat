#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>

#include "socketthread.h"

#define SERVER_ADRESS "10.96.108.41"

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

    void            on_actionSet_username_triggered();

    void            on_actionConnect_to_IP_triggered();

private:
    Ui::MainWindow* ui;
    //QUdpSocket      m_Socket;
    QString         m_Name;
    QString         m_ServerAdress;
    SocketThread*   m_SocketThread;

    void            InitClient();
    void            SendMessage();
    void            SendMessage(QString);
    void            AppendChat(QString message);


};



#endif // MAINWINDOW_H
