#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QList>

#define FRAME_FLAG "Frame"

enum FrameType
{
    REPORT = 0x00,
    CHECK = 0x01,
    NETSTATUS = 0x02,
    CONTROL = 0x03,
};

enum Cluster
{
    Temperature = 0x00,
    TemperatureArray = 0x01,
    LightStrength = 0x02,
    InfraredStrength = 0x03,
    Humidity = 0x04,
    OnOff = 0x05,
};

enum Control
{
    READWRITE = 0x00,
    READONLY =0x01,
};

enum DataType
{
    _INT32 = 0x00,
    _FLOAT = 0x01,
};

class DataPull
{
public:
    uint8_t Node;
    uint8_t Cluster;
    uint8_t Endpoint;
    uint8_t Controllable;
    uint8_t Datatype;
    uint8_t Datalength;
    QByteArray Data;
    DataPull(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, QByteArray);
    ~DataPull();
};


struct DataforUI
{
    uint8_t Node;
    uint8_t Endpoint;
    uint8_t Controllable;//reserved
    uint8_t Datatype;
    uint8_t Datalength;
    bool operator == (const DataforUI &pkt) const
    {
        return((Node == pkt.Node)&&(Endpoint == pkt.Endpoint));
    }
};


void AddtoTail(QByteArray NetMessage);
DataPull *ProcessMessage();

#endif // DATASTRUCTURE_H
