#include "tcp_thread.h"
#include "datastructure.h"
#include <QtNetwork>
#include <QDebug>

TCP_Thread::TCP_Thread()
{
    NetMessage.clear();
    stopped = false;
    blockSize = 0;
    this->start();
}

TCP_Thread::~TCP_Thread()
{
    delete tcpSocket;
}

void TCP_Thread::CreatConnect()
{
    tcpSocket -> moveToThread(this);
    tcpSocket -> abort();
    tcpSocket -> connectToHost(HostAddress,Port);
}


void TCP_Thread::ReadData()
{
    NetMessage.clear();
    QByteArray byteArray = this->tcpSocket->readAll();
    qDebug() << tr("recv from server: ") + byteArray + QDateTime::currentDateTime().toString(" yyyy-M-dd hh:mm:ss") + tr("\n");
    NetMessage = byteArray;
    AddtoTail(NetMessage);
}

void TCP_Thread::SendData()
{
    QString m = "Client Qt!";
    tcpSocket -> write(m.toLatin1());
    tcpSocket -> waitForBytesWritten();
    if(!stopped)
        QTimer::singleShot(1000,this,&TCP_Thread::SendData);
}


void TCP_Thread::DisplayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket -> errorString();
    tcpSocket -> disconnectFromHost();
    stop();
}

void TCP_Thread::stop()
{
    stopped = true;
}

void TCP_Thread::run()
{
    tcpSocket = new QTcpSocket();
    connect(tcpSocket,&QIODevice::readyRead,this,&TCP_Thread::ReadData);
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
    CreatConnect();
    QTimer::singleShot(1000,this,&TCP_Thread::SendData);
    while(!stopped)
    {
        if(NetMessage.toLatin1() == "stop")
        {
            tcpSocket -> disconnectFromHost();
            stop();
        }
    }
    delete tcpSocket;
    delete this;
}
