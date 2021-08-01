#ifndef TCP_THREAD_H
#define TCP_THREAD_H

#include <QAbstractSocket>
#include <QString>
#include <QWidget>

#define HostAddress "130.61.250.117"
//#define HostAddress "192.168.112.1"
#define Port 17480
#define BindInfo "Monitor:001Bind:001"

class QTcpSocket;

class TCP_Thread : public QWidget
{
    Q_OBJECT
public:
    TCP_Thread();
    ~TCP_Thread();
    void CreatConnect();
    void ReadData();

    QTcpSocket *tcpSocket;
    QByteArray NetMessage;

protected:
    //void run();
    //void stop();

private:
    char *ip_addr, port_num;
    quint16 blocksize;
    //bool stopped;

private slots:
    void SendData(QByteArray *);
    void SendBind();
    void DisplayError(QAbstractSocket::SocketError);

};

#endif // TCP_THREAD_H
