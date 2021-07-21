#ifndef TCP_THREAD_H
#define TCP_THREAD_H

#include <QThread>
#include <QAbstractSocket>
#include <QString>

#define HostAddress "192.168.112.1"
#define Port 17480

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

    QString NetMessage;

protected:
    void run();
    void stop();

private:
    char *ip_addr, port_num;
    quint16 blockSize;
    QTcpSocket *tcpSocket;
    bool stopped;
private slots:
    void DisplayError(QAbstractSocket::SocketError);

};

#endif // TCP_THREAD_H
