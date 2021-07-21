#include "mainwindow.h"
#include "datastructure.h"
#include <QDebug>
#include <QLabel>

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

void MainWindow::Load_UI()
{
    MainWidget = new QWidget;
    WidgetP1 = new QWidget;
    WidgetP2 = new QWidget;
    WidgetP3 = new QWidget;
    WidgetP4 = new QWidget;
    Status = new QLabel;

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
    GridLayout -> addWidget(Status,4,0,1,4,Qt::AlignLeft|Qt::AlignVCenter);
    GridLayout -> addWidget(StackedWidget,0,0,4,3);

    SubTitle1 = new QLabel(WidgetP1);
    SubTitle2 = new QLabel(WidgetP2);
    SubTitle3 = new QLabel(WidgetP3);
    SubTitle4 = new QLabel(WidgetP4);

    SubTitle1 -> setText("Infrared Sensor");
    SubTitle2 -> setText("Temprature & Humidity");
    SubTitle3 -> setText("Light sensor");
    SubTitle4 -> setText("Equipment");


    //Status->setStyleSheet("background-color:green;");

    StackedWidget -> addWidget(WidgetP1);
    StackedWidget -> addWidget(WidgetP2);
    StackedWidget -> addWidget(WidgetP3);
    StackedWidget -> addWidget(WidgetP4);

    connect(Infrared,SIGNAL(clicked()),this,SLOT(SwitchPage_IR()));
    connect(Temp_Humi,SIGNAL(clicked()),this,SLOT(SwitchPage_TH()));
    connect(Light,SIGNAL(clicked()),this,SLOT(SwitchPage_LI()));
    connect(Equipment,SIGNAL(clicked()),this,SLOT(SwitchPage_EQ()));
}

void MainWindow::Load_PageIR()
{}

void MainWindow::Load_PageTH()
{}

void MainWindow::Load_PageLI()
{}

void MainWindow::Load_PageEQ()
{}

void MainWindow::Load_Status()
{
    //Status = new QLabel(WidgetStatus);
    Status -> setText(tr("Initialized"));
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
    QString *Message;
    while(1)
    {
        Message = ProcessMessage();
        if(!Message)
        {
            msleep(20);
        }
        else
        {
            UI.Status -> setText(*Message);
            delete Message;
            msleep(5);
        }
        Message = NULL;
    }
}
