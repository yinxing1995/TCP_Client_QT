#ifndef TCP_THREAD_H
#define TCP_THREAD_H

#include <QThread>
//#include <QTcpSocket>
#include <QAbstractSocket>
#include <QString>

#define HostAddress "130.61.250.117"
#define Port 17480
#define BindInfo "Monitor:001Bind:001"

class QTcpSocket;

class TCP_Thread : public QThread
{
    Q_OBJECT
public:
    TCP_Thread();
    ~TCP_Thread();
    void CreatConnect();
    void ReadData();
    void SendData();
    void SendBind();
    QTcpSocket *tcpSocket;
    QByteArray NetMessage;

protected:
    void run();
    //void stop();

private:
    char *ip_addr, port_num;
    quint16 blocksize;
    //bool stopped;
private slots:
    void DisplayError(QAbstractSocket::SocketError);

};

#endif // TCP_THREAD_H
