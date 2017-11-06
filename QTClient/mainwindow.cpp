#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitClient();
}

MainWindow::~MainWindow()
{
    //delete m_Receive;
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    SendMessage();
}

void MainWindow::on_chatInput_returnPressed()
{
    SendMessage();
}

void MainWindow::InitClient()
{
    m_Socket.bind();

    //m_Receive = new std::thread(ReceiveMessage, &m_Socket);
}

void MainWindow::SendMessage()
{
    std::string buffer = ui->chatInput->text().toStdString();
    // Send message to server. Doesn't send any information about username atm.
    //m_Socket.writeDatagram(buffer.c_str(), sizeof(buffer) + 1, QHostAddress(SERVER_ADRESS), 55002);
}

void MainWindow::AppendChat(QString message)
{
    ui->chatOutput->append(message);
}

//void MainWindow::ReceiveMessage(QUdpSocket &socket, Ui::MainWindow& ui)
void MainWindow::ReceiveMessage(QUdpSocket* socket)
{
    while (true)
    {
        char buffer[1024];
        QHostAddress* sender;
        quint16* port;

        // Wait for a message
        //socket->waitForReadyRead();
        //socket->readDatagram(buffer, sizeof(buffer), sender, port);

        // Add the message to chat once received.
       // ui.chatOutput->append(buffer);
    }
}
