#include "tcp_thread.h"
#include "datastructure.h"
#include <QtNetwork>
#include <QDebug>

TCP_Thread::TCP_Thread()
{
    NetMessage.clear();
    //stopped = false;
    tcpSocket = new QTcpSocket();
    connect(tcpSocket,&QIODevice::readyRead,this,&TCP_Thread::ReadData);
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(SendBind()));
    //blocksize = 0;
}

TCP_Thread::~TCP_Thread()
{
    tcpSocket -> disconnectFromHost();
    delete tcpSocket;
}

void TCP_Thread::CreatConnect()
{
    tcpSocket -> abort();
    tcpSocket -> connectToHost(HostAddress,Port);
}


void TCP_Thread::ReadData()
{
    NetMessage.clear();
    QByteArray byteArray = this->tcpSocket->readAll();
    NetMessage = byteArray;
    AddtoTail(NetMessage);
}

void TCP_Thread::SendData(QByteArray *Message)
{
    tcpSocket -> write(*Message);
    tcpSocket -> waitForBytesWritten();
    qDebug() << "Send Message";
    delete Message;
}

void TCP_Thread::SendBind()
{
    QString m = BindInfo;
    tcpSocket -> write(m.toLatin1());
    tcpSocket -> waitForBytesWritten();
}

void TCP_Thread::DisplayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket -> errorString();
    tcpSocket -> disconnectFromHost();
    //stop();
    QTimer::singleShot(5000,this,&TCP_Thread::CreatConnect);
}

/*
void TCP_Thread::stop()
{
    stopped = true;
}
*/

