#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>

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
    SendMessage("/disconnect ");
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
    m_Name = "UnnamedPlayer";
    m_SocketThread = new SocketThread();

    m_SocketThread->SetChatbox(ui->chatOutput);

    QObject::connect(m_SocketThread, SIGNAL(finished()), this, SLOT(quit()));
    m_SocketThread->start();
}

void MainWindow::SendMessage()
{
    m_SocketThread->SendMessage(ui->chatInput->text());
    ui->chatInput->clear();
}

void MainWindow::SendMessage(QString message)
{
    m_SocketThread->SendMessage(message);
}

void MainWindow::AppendChat(QString message)
{
    ui->chatOutput->append(message);
}

void MainWindow::on_actionSet_username_triggered()
{
    QInputDialog dialog;
    dialog.setTextValue("Username");


    dialog.exec();

    m_Name = dialog.textValue();
    SendMessage("/setname " + dialog.textValue());

}

void MainWindow::on_actionConnect_to_IP_triggered()
{
    QInputDialog dialog;
    dialog.setTextValue(m_SocketThread->GetIp());


    dialog.exec();

    m_SocketThread->SetIp(dialog.textValue());
}
