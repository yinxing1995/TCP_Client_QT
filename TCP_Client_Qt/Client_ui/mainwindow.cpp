#include "mainwindow.h"
#include "datastructure.h"
#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this -> setWindowTitle(tr("Fire Detecting System"));
    this -> resize(800,600);
    Load_UI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::Load_PageIR()
{
    SubTitle1 = new QLabel(WidgetP1);
    SubTitle1 -> setText("Infrared Sensor");
}

void MainWindow::Load_PageTH()
{
    SubTitle2 = new QLabel(WidgetP2);
    SubTitle2 -> setText("Temprature & Humidity");
}

void MainWindow::Load_PageLI()
{
    SubTitle3 = new QLabel(WidgetP3);
    SubTitle3 -> setText("Light Sensor");
}

void MainWindow::Load_PageEQ()
{
    SubTitle4 = new QLabel(WidgetP4);
    SubTitle4 -> setText("Equipment");
}

void MainWindow::Load_Status()
{
    Status = new QTextEdit(WidgetStatus);
    QFont ft;
    ft.setPointSize(10);
    Status -> setFont(ft);
    QGridLayout *LayoutStatus = new QGridLayout(WidgetStatus);
    LayoutStatus -> addWidget(Status,0,0,1,1);
    Status -> setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    Status -> document() -> setMaximumBlockCount(50);
    Status -> setReadOnly(true);
}

void MainWindow::Load_UI()
{
    MainWidget = new QWidget;
    WidgetP1 = new QWidget;
    WidgetP2 = new QWidget;
    WidgetP3 = new QWidget;
    WidgetP4 = new QWidget;
    WidgetStatus = new QWidget;

    StackedWidget = new QStackedWidget(MainWidget);
    GridLayout = new QGridLayout(MainWidget);

    setCentralWidget(MainWidget);

    this->centralWidget()->setLayout(GridLayout);

    Infrared = new QPushButton(MainWidget);
    Infrared -> setText(tr("IR"));

    Temp_Humi = new QPushButton(MainWidget);
    Temp_Humi -> setText(tr("TH"));

    Light = new QPushButton(MainWidget);
    Light -> setText(tr("LI"));

    Equipment = new QPushButton(MainWidget);
    Equipment -> setText(tr("EQ"));

    GridLayout -> addWidget(Infrared,0,3,1,1,Qt::AlignCenter);
    GridLayout -> addWidget(Temp_Humi,1,3,1,1,Qt::AlignCenter);
    GridLayout -> addWidget(Light,2,3,1,1,Qt::AlignCenter);
    GridLayout -> addWidget(Equipment,3,3,1,1,Qt::AlignCenter);
    GridLayout -> addWidget(WidgetStatus,4,0,1,3);
    GridLayout -> addWidget(StackedWidget,0,0,4,3);

    WidgetStatus -> setStyleSheet("background-color:green;");
    StackedWidget-> setStyleSheet("background-color:grey;");

    StackedWidget -> addWidget(WidgetP1);
    StackedWidget -> addWidget(WidgetP2);
    StackedWidget -> addWidget(WidgetP3);
    StackedWidget -> addWidget(WidgetP4);

    Load_PageIR();
    Load_PageTH();
    Load_PageLI();
    Load_PageEQ();
    Load_Status();

    connect(Infrared,SIGNAL(clicked()),this,SLOT(SwitchPage_IR()));
    connect(Temp_Humi,SIGNAL(clicked()),this,SLOT(SwitchPage_TH()));
    connect(Light,SIGNAL(clicked()),this,SLOT(SwitchPage_LI()));
    connect(Equipment,SIGNAL(clicked()),this,SLOT(SwitchPage_EQ()));
}


void MainWindow::SwitchPage_IR()
{
    StackedWidget -> setCurrentIndex(PAGE_IR);
}

void MainWindow::SwitchPage_TH()
{
    StackedWidget -> setCurrentIndex(PAGE_TH);
}

void MainWindow::SwitchPage_LI()
{
    StackedWidget -> setCurrentIndex(PAGE_LI);
}

void MainWindow::SwitchPage_EQ()
{
    StackedWidget -> setCurrentIndex(PAGE_EQ);
}


UI_Thread::UI_Thread()
{
    MainWindow ui;
    this->start();
}

UI_Thread::~UI_Thread()
{

}

void UI_Thread::run()
{
    QByteArray *Message;
    while(1)
    {
        Message = ProcessMessage();
        if(!Message)
        {

            msleep(20);
        }
        else
        {
            qDebug() << Message->toHex();
            UI.Status -> append(QDateTime::currentDateTime().toString("[yyyy-M-dd hh:mm:ss]\r\n") + "Recv:" + Message->toHex(' '));
            UI.Status -> moveCursor(QTextCursor::End);
            delete Message;
            msleep(5);
        }
        Message = NULL;
    }
}
