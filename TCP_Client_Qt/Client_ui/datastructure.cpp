#include "datastructure.h"

QQueue<QByteArray *> MessageQueue;

void AddtoTail(QByteArray Netmessage)
{
    QByteArray *Buffer = new QByteArray;
    *Buffer = Netmessage;
    MessageQueue.enqueue(Buffer);
}

QByteArray *ProcessMessage()
{
    if(MessageQueue.isEmpty())
        return NULL;
    return MessageQueue.dequeue();
}
