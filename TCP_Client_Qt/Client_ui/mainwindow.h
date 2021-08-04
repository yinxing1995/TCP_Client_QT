#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QPushButton>
#include <QStackedWidget>
#include <QLayout>
#include <QTextEdit>
#include <QtCharts>
#include "datastructure.h"

#define PAGE_IR 0
#define PAGE_TH 1
#define PAGE_LI 2
#define PAGE_EQ 3

class QLabel;

struct Components_Axis
{
    uint8_t Node;
    uint8_t Endpoint;
    QLineSeries *Series;//reserved
    QChart *Chart;
    QChartView *ChartView;
    QDateTimeAxis *DateAxisX;
    QLabel *Value;
    QPushButton *Command;
    bool operator == (const Components_Axis &pkt) const
    {
        return((Node == pkt.Node)&&(Endpoint == pkt.Endpoint));
    }
};

struct Components_Array
{
public:
    uint8_t Node;
    uint8_t Endpoint;

    QVector<QLabel *> Data;

    bool operator == (const Components_Array &pkt) const
    {
        return((Node == pkt.Node)&&(Endpoint == pkt.Endpoint));
    }
};

struct Components_Control
{
public:
    uint8_t Node;
    uint8_t Endpoint;
    QLabel *Value;
    QPushButton *Onoff;

    bool operator == (const Components_Control &pkt) const
    {
        return((Node == pkt.Node)&&(Endpoint == pkt.Endpoint));
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void Creat_Array(DataforUI *,QWidget *,DataPull *);
    void Fresh_Array(DataPull *);
    void Creat_Axis(DataforUI *,QWidget *,DataPull *);
    void Fresh_Axis(DataPull *);
    void Creat_Control(DataforUI *,QWidget *,DataPull *);
    void Fresh_Control(DataPull *);
    void Send_Message(QByteArray *);


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTextEdit *Status;


    void Fresh_PageIR(DataPull *, DataforUI *);
    void Fresh_PageTH(DataPull *,DataforUI *);
    void Fresh_PageLI(DataPull *,DataforUI *);
    void Fresh_PageEQ(DataPull *,DataforUI *);
    void Fresh_Status(DataPull *,DataforUI *);
    void Fresh_Alarm(DataPull *);


private:
    static uint8_t CountforTH;
    static uint8_t CountforLI;
    static uint8_t CountforIR;
    static uint8_t CountforEQ;
    bool wheelmove;//reserved
    /*
    QLabel *SubTitle1;
    QLabel *SubTitle2;
    QLabel *SubTitle3;
    QLabel *SubTitle4;
*/
    QPushButton *Infrared;
    QPushButton *Temp_Humi;
    QPushButton *Light;
    QPushButton *Equipment;

    QStackedWidget *StackedWidget;
    QGridLayout *GridLayout;
    QGridLayout *InfraredLayout;
    QGridLayout *Temp_HumiLayout;
    QGridLayout *LightLayout;
    QGridLayout *EquipmentLayout;


    QWidget *MainWidget;
    QWidget *WidgetP1;
    QWidget *WidgetP2;
    QWidget *WidgetP3;
    QWidget *WidgetP4;
    QWidget *WidgetStatus;
    QWidget *WidgetAlarm;

    //QList<Components_Axis> THlist;
    //struct Components Creat_axis(DataforUI *, uint8_t, QWidget *);

    void Load_UI();
    void Load_PageIR();
    void Load_PageTH();
    void Load_PageLI();
    void Load_PageEQ();
    void Load_Status();
    void GenerateMessage(uint8_t ,uint8_t ,uint8_t ,uint8_t ,float *);
    void Load_Alarm();

private slots:
    void SwitchPage_IR();
    void SwitchPage_TH();
    void SwitchPage_LI();
    void SwitchPage_EQ();

    void New_Array(DataforUI *,QWidget *,DataPull *);
    void Update_Array(DataPull *);
    void New_Axis(DataforUI *,QWidget *,DataPull *);
    void Update_Axis(DataPull *);
    void New_Control(DataforUI *,QWidget *,DataPull *);
    void Update_Control(DataPull *);
    void Creat_Command_Button();
};

class UI_Thread : public QThread
{
    Q_OBJECT
public:
    UI_Thread();
    ~UI_Thread();
    MainWindow UI;

private:
    void UpdateUI(DataPull *);
    void run();
};
#endif // MAINWINDOW_H
