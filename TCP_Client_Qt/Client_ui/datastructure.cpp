#include "datastructure.h"

QQueue<QString *> MessageQueue;

void AddtoTail(QString Netmessage)
{
    QString *Buffer = new QString;
    *Buffer = Netmessage;
    MessageQueue.enqueue(Buffer);
}

QString *ProcessMessage()
{
    if(MessageQueue.isEmpty())
        return NULL;
    return MessageQueue.dequeue();
}
