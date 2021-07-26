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

struct Components
{
    uint8_t Node;
    uint8_t Endpoint;
    QLineSeries *Series;
    QChart *Chart;
    QChartView *ChartView;
    QLabel *Value;
    QPushButton *Command;
    bool operator == (const DataforUI &pkt) const
    {
        return((Node == pkt.Node)&&(Endpoint == pkt.Endpoint));
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTextEdit *Status;

    void Fresh_PageIR(DataPull *, DataforUI);
    void Fresh_PageTH(DataPull *, DataforUI);
    void Fresh_PageLI(DataPull *, DataforUI);
    void Fresh_PageEQ(DataPull *, DataforUI);
    void Fresh_Status(DataPull *, DataforUI);

private:
    static uint8_t CountforTH;
    static uint8_t CountforLI;
    bool wheelmove;//reserved

    QLabel *SubTitle1;
    QLabel *SubTitle2;
    QLabel *SubTitle3;
    QLabel *SubTitle4;

    QPushButton *Infrared;
    QPushButton *Temp_Humi;
    QPushButton *Light;
    QPushButton *Equipment;

    QStackedWidget *StackedWidget;
    QGridLayout *GridLayout;
    //QGridLayout *InfraredLayout;
    //QGridLayout *Temp_HumiLayout;
    //QGridLayout *LightLayout;
    //QGridLayout *EquipmentLayout;


    QWidget *MainWidget;
    QWidget *WidgetP1;
    QWidget *WidgetP2;
    QWidget *WidgetP3;
    QWidget *WidgetP4;
    QWidget *WidgetStatus;

    QList<Components> Axislist;
    struct Components Creat_axis(DataforUI *, uint8_t, QWidget *);

    void Load_UI();
    void Load_PageIR();
    void Load_PageTH();
    void Load_PageLI();
    void Load_PageEQ();
    void Load_Status();

private slots:
    void SwitchPage_IR();
    void SwitchPage_TH();
    void SwitchPage_LI();
    void SwitchPage_EQ();
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
