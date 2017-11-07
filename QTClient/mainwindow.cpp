#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_SocketThread(nullptr)
{
    ui->setupUi(this);
    InitClient();
}

MainWindow::~MainWindow()
{
    m_SocketThread->SetRunning(false);
    m_SocketThread->quit();
    while(!m_SocketThread->isFinished());
    delete m_SocketThread;
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
    //m_Socket.bind();
    m_SocketThread = new SocketThread();

    //m_SocketThread->SetSocket(&m_Socket);
    m_SocketThread->SetChatbox(ui->chatOutput);

    QObject::connect(m_SocketThread, SIGNAL(finished()), this, SLOT(quit()));
    m_SocketThread->start();
}

void MainWindow::SendMessage()
{
    /*
    std::string buffer = ui->chatInput->text().toStdString();
    // Send message to server. Doesn't send any information about username atm.
    m_Socket.writeDatagram(buffer.c_str(), sizeof(buffer) + 1, QHostAddress(SERVER_ADRESS), 55002);
    ui->chatInput->clear();
    */
    m_SocketThread->SendMessage(ui->chatInput->text());
    ui->chatInput->clear();
}

void MainWindow::AppendChat(QString message)
{
    ui->chatOutput->append(message);
}

//void MainWindow::ReceiveMessage(QUdpSocket &socket, Ui::MainWindow& ui)
/*void MainWindow::ReceiveMessage(QUdpSocket& socket, Ui::MainWindow* s_ui)
{
    while (true)
    {
        char buffer[1024];
        QHostAddress* sender;
        quint16* port;

        // Wait for a message
        socket.waitForReadyRead();
        socket.readDatagram(buffer, sizeof(buffer), sender, port);

        // Add the message to chat once received.
        s_ui->chatOutput->append(buffer);
    }
}*/
