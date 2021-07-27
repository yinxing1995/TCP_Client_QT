#include "mainwindow.h"
#include "datastructure.h"
#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>

extern QVector<DataforUI> DataList;
QVector<Components> Axislist;

uint8_t MainWindow::CountforTH = 0;
uint8_t MainWindow::CountforLI = 0;

//void MainWindow::Creat_axis(DataforUI *info, uint8_t num, QWidget *subpage)
void MainWindow::New_Axis(DataforUI *info,QWidget *subpage,DataPull *data)
{

    uint8_t node = info -> Node;
    uint8_t endpoint = info -> Endpoint;
    Components init{node,endpoint,NULL,NULL,NULL,NULL,NULL,NULL};

    init.Chart = new QChart();
    init.ChartView = new QChartView();
    init.Series = new QLineSeries();
    init.DateAxisX = new QDateTimeAxis();//时间类型轴(用作X轴)

    QValueAxis * AxisY = new QValueAxis();


    init.Chart->setTitle("Linechart");
    init.Chart->addSeries(init.Series);

    init.DateAxisX->setFormat("mm:ss");
    init.DateAxisX->setTickCount(6);

    AxisY->setRange(-20,100);

    init.Chart->addAxis(init.DateAxisX, Qt::AlignBottom);
    init.Chart->addAxis(AxisY, Qt::AlignLeft);
    init.Series->attachAxis(init.DateAxisX);
    init.Series->attachAxis(AxisY);

    QPen green(Qt::red);
    green.setWidth(2);
    init.Series->setPen(green);
    init.Series->setName("Value");
    init.ChartView -> setChart(init.Chart);
    init.ChartView -> setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    dynamic_cast<QGridLayout*>(subpage->layout())->addWidget(init.ChartView,CountforTH,0,1,3);
    if(info -> Controllable == READONLY)
    {
        init.Value = new QLabel("Temperature or Humidity");
        dynamic_cast<QGridLayout*>(subpage->layout())->addWidget(init.Value,CountforTH,3,1,1,Qt::AlignCenter);
    }
    else
    {
        init.Command = new QPushButton("ON/OFF");
        dynamic_cast<QGridLayout*>(subpage->layout())->addWidget(init.Command,CountforTH,3,1,1,Qt::AlignCenter);
    }
    delete info;
    Axislist.append(init);
    CountforTH++;

    Update_Axis(data);
    return;

}

void MainWindow::Update_Axis(DataPull *data)
{

    uint8_t endpoint = data->Endpoint;
    uint8_t node = data->Node;
    Components temp{node,endpoint,NULL,NULL,NULL,NULL,NULL};
    if(!Axislist.contains(temp))
        return;
    temp = Axislist.value(Axislist.indexOf(temp));

    QDateTime past = QDateTime::currentDateTime().addSecs(-300);
    QDateTime now = QDateTime::currentDateTime();
    temp.DateAxisX->setRange(past,now);
    float Yvalue;
    memcpy(&Yvalue,data->Data.constData(),sizeof(float));
    temp.Series->append(now.toMSecsSinceEpoch(),(qreal)Yvalue);
    if(data->Cluster == Temperature)
    {
        char p[20];
        sprintf(p,"Temp = %.2f℃",Yvalue);
        temp.Series->setName("Temperature");
        temp.Value -> setText(p);
    }
    if(data->Cluster == Humidity)
    {
        char p[20];
        sprintf(p,"Humi = %.2f%%%",Yvalue);
        temp.Series->setName("Humidity");
        temp.Value -> setText(p);
    }
    delete data;
}

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

    //initial state
    //SubTitle2 = new QLabel(WidgetP1);
    //SubTitle2 -> setText("Infrared Sensor");
    //layout loaded
}

void MainWindow::Fresh_PageTH(DataPull *data, DataforUI *info)
{

    //check if this node/endpoint exist;if yes fresh,if no creat;
    if(!DataList.contains(*info))
    {
        DataList.append(*info);
        emit Creat_Axis(info,WidgetP2,data);
    }
    else
    {
        emit Fresh_Axis(data);
    }
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
    Status -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
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
    InfraredLayout = new QGridLayout(WidgetP1);
    Temp_HumiLayout = new QGridLayout(WidgetP2);
    LightLayout = new QGridLayout(WidgetP3);
    EquipmentLayout = new QGridLayout(WidgetP4);

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
    connect(this,SIGNAL(Creat_Axis(DataforUI *,QWidget *,DataPull *)),this,SLOT(New_Axis(DataforUI *,QWidget *,DataPull *)));
    connect(this,SIGNAL(Fresh_Axis(DataPull *)),this,SLOT(Update_Axis(DataPull *)));

    WidgetP1 -> setLayout(InfraredLayout);
    WidgetP2 -> setLayout(Temp_HumiLayout);
    WidgetP3 -> setLayout(LightLayout);
    WidgetP4 -> setLayout(EquipmentLayout);

    /*
    QChart* chart = new QChart();

    // 构建折线系列对象
    QLineSeries *series = new QLineSeries();
    for (quint32 i = 0; i < 100; i++)
    {
        // 参数 x 为循环自增变量 i，参数 y 为正弦函数Y值
        series->append(i, sin(static_cast<double>(0.6f*i)));
    }

    // 将系列添加到图表
    chart->addSeries(series);
    // 基于已添加到图表的 series 来创建默认的坐标轴
    chart->createDefaultAxes();

    QChartView *p = new QChartView(chart);
    p->setChart(chart);
    Temp_HumiLayout->addWidget(p,0,0,3,1,Qt::AlignCenter);
    */


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

void UI_Thread::UpdateUI(DataPull *Message)
{
    uint8_t node = Message->Node;
    uint8_t control = Message->Controllable;
    uint8_t cluster = Message->Cluster;
    uint8_t endpoint = Message->Endpoint;
    DataforUI *Info = new DataforUI{node,endpoint,control};
    switch(cluster)
    {
    case Temperature:
        UI.Fresh_PageTH(Message,Info);
        break;
    case Humidity:
        UI.Fresh_PageTH(Message,Info);
        break;
    default:
        break;
    }
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
    DataPull *Message;


    while(1)
    {
        /*
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
        */
        Message = ProcessMessage();
        if(!Message)
        {
            msleep(2);
        }
        else
        {
            UI.Status -> append(QDateTime::currentDateTime().toString("[yyyy-M-dd hh:mm:ss]\r\n") + "Recv:" + Message->Data.toHex(' '));
            UI.Status -> moveCursor(QTextCursor::End);
            msleep(2);
            UpdateUI(Message);
        }
    }
}
