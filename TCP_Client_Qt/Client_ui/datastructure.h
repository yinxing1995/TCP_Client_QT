#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QList>
#include <QDebug>

#define FRAME_FLAG "Frame"

enum Cluster
{
    Temperature = 0x00,
    TemperatureArray = 0x01,
    LightStrength = 0x02,
    InfraredStrength = 0x03,
    Humidity = 0x04,
    OnOff = 0x05,
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
    uint8_t Cluster;
    uint8_t Endpoint;
    uint8_t Controllable;
};


void AddtoTail(QByteArray NetMessage);
DataPull *ProcessMessage();

#endif // DATASTRUCTURE_H
