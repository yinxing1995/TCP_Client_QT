#include "datastructure.h"
#include <cstdlib>


//UploadFrame:Length(2)|FrameType|NodeID|Cluster|Endpoint|Controllable|Datatype|DataLength|Data|CheckSum

/*
* Might not be needed
*
* CheckFrame:Length|FrameType|NodeID|Cluster|Endpoint|Controllable|Datatype|DataLength|CheckSum
* Construction:Length|FrameType|"Initialized"|CheckSum
*/

//processing with buffer
QByteArray Buffer;
QList<DataforUI> DataList;

DataPull::DataPull(uint8_t node,uint8_t cluster, uint8_t endpoint, uint8_t control, uint8_t datatype, uint8_t datalength, QByteArray data)
{
    Node = node;
    Cluster = cluster;
    Endpoint = endpoint;
    Controllable = control;
    Datatype = datatype;
    Datalength = datalength;
    Data = data;
}

DataPull::~DataPull()
{

}

void AddtoTail(QByteArray Netmessage)
{
    Buffer.append(Netmessage);
}

DataPull *ProcessMessage()
{
    if(!Buffer.size())
        return NULL;

    int i = Buffer.indexOf(FRAME_FLAG,0);
    if(i < 0)
    {
        Buffer.clear();
        return NULL;
    }
    QByteArray Temp;
    uint16_t Framelen;
    uint8_t Frametype;
    uint8_t NodeID;
    uint8_t Cluster;
    uint8_t Endpoint;
    uint8_t Controllable;
    uint8_t Datatype;
    uint8_t Datalength;
    QByteArray Data;
    //get len

    Buffer.remove(0, i + strlen(FRAME_FLAG));
    Temp = Buffer.left(sizeof(uint16_t));
    memcpy(&Framelen, Temp.constData(), sizeof(uint16_t));
    Temp.clear();

    //len got,then process data
    Temp = Buffer.left(Framelen);
    Buffer.remove(0,Framelen);
    Temp.remove(0,sizeof(Framelen));
    Frametype = *Temp.constData();
    Temp.remove(0,sizeof(Frametype));
    NodeID = *Temp.constData();
    Temp.remove(0,sizeof(NodeID));
    Cluster = *Temp.constData();
    Temp.remove(0,sizeof(Cluster));
    Endpoint = *Temp.constData();
    Temp.remove(0,sizeof(Endpoint));
    Controllable = *Temp.constData();
    Temp.remove(0,sizeof(Controllable));
    Datatype = *Temp.constData();
    Temp.remove(0,sizeof(Datatype));
    Datalength = *Temp.constData();
    Temp.remove(0,sizeof(Datalength));

    //cpydata
    Data = Temp.left(4*Datalength);
    DataPull *p = new DataPull(NodeID, Cluster, Endpoint, Controllable, Datatype, Datalength, Data);

    return p;
}

