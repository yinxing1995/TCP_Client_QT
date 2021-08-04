#include "mainwindow.h"
#include "datastructure.h"
#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>

extern QVector<DataforUI> DataList;
QVector<Components_Axis> Axislist;
QVector<Components_Array> Arraylist;
QVector<Components_Control> Controllist;

uint8_t MainWindow::CountforTH = 0;
uint8_t MainWindow::CountforLI = 0;
uint8_t MainWindow::CountforIR = 0;
uint8_t MainWindow::CountforEQ = 0;

void MainWindow::GenerateMessage(uint8_t node,uint8_t endpoint,uint8_t datatype,uint8_t datalen,float *data)
{
    QByteArray *Message = new QByteArray;
    Message->append(CONTROL);
    Message->append(node);
    Message->append(endpoint);
    Message->append(datatype);
    Message->append(datalen);
    float p1[datalen];
    uint32_t p2[datalen];
    uint8_t i = datalen;
    uint8_t checksum;
    if(datatype == _INT32)
    {
        for(i=0;i<datalen;i++)
        {
            p2[i] = data[i];
        }
        Message->append((char *)p2,datalen*sizeof(p2));
    }
    else if(datatype == _FLOAT)
    {
        for(i=0;i<datalen;i++)
        {
            p1[i] = data[i];
        }
        Message->append((char *)p1,datalen*sizeof(p1));
    }
    else return;
    for(i=0;i<Message->size();i++)
    {
        checksum += Message->at(i);
    }
    Message->append(checksum);
    uint16_t FrameLen = Message->size() + sizeof(FrameLen);
    Message->insert(0,(char *)&FrameLen,sizeof(FrameLen));
    Message->insert(0,FRAME_FLAG,strlen(FRAME_FLAG));
    qDebug() << Message->toHex();
    emit Send_Message(Message);
}

template <typename T>
static void CovertKtoRG (T *component, float temperature)
{
    uint8_t R = 0,G = 0,B = 0;
    char p[30];
    if(temperature <= 0)
        temperature = 0;
    else if(temperature >= 120)
        temperature = 120;

    if(temperature < 20)
    {
        R = 0;
        G = 0;
        B = 255;
    }
    else if(temperature < 40)
    {
        R = 0;
        G = 255 * (temperature - 20) / 20;
        B = 255;
    }
    else if(temperature < 60)
    {
        R = 0;
        G = 255;
        B = 255 - 255 * (temperature - 40) / 20;
    }
    else if(temperature < 80)
    {
        R = 255 * (temperature - 60) / 20;
        G = 255;
        B = 0;
    }
    else if(temperature < 100)
    {
        R = 255;
        G = 255 - 255 * (temperature - 80) / 20;
        B = 0;
    }
    else if(temperature < 120)
    {
        R = 255;
        G = 0;
        B = 255 * (temperature - 100) / 20;
    }

    sprintf(p,"QLabel{background-color:rgb(%d,%d,%d);}",R,G,B);
    component -> setStyleSheet(p);

}

void MainWindow::New_Array(DataforUI *info,QWidget *subpage,DataPull *data)
{
    Components_Array init;
    init.Node = info->Node;
    init.Endpoint = info->Endpoint;
    init.Data.clear();
    init.Data.resize(info->Datalength);
    QGridLayout * Window = dynamic_cast<QGridLayout*>(subpage->layout());
    //size could be variable
    for(int row=0; row<8; row++)
    {
        for(int col=0; col<8; col++)
        {
            QLabel *p = new QLabel();
            p->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
            p->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
            p->setAlignment(Qt::AlignCenter);
            Window->addWidget(p,row,col,1,1);
            init.Data.push_front(p);
        }
    }
    //
    Arraylist.append(init);
    Update_Array(data);
    delete info;
    return;
}

void MainWindow::Update_Array(DataPull *data)
{
    uint8_t endpoint = data->Endpoint;
    uint8_t node = data->Node;
    QVector<QLabel *> null;
    Components_Array temp{node,endpoint,null};
    if(!Arraylist.contains(temp))
        return;
    temp = Arraylist.value(Arraylist.indexOf(temp));
    float Value = 0;
    char p[20];
    for(int i=0;i<64;i++)
    {
        memcpy(&Value,data->Data.constData(),sizeof(Value));
        data->Data.remove(0,sizeof(Value));
        sprintf(p,"%.2f℃",Value);
        CovertKtoRG(temp.Data[i],Value);
        temp.Data[i]->setText(p);
    }
    delete data;
}


void MainWindow::New_Axis(DataforUI *info,QWidget *subpage,DataPull *data)
{
    uint8_t node = info -> Node;
    uint8_t endpoint = info -> Endpoint;
    Components_Axis init{node,endpoint,NULL,NULL,NULL,NULL,NULL,NULL};

    init.Chart = new QChart();
    init.ChartView = new QChartView();
    init.Series = new QLineSeries();
    init.DateAxisX = new QDateTimeAxis();//时间类型轴(用作X轴)

    QValueAxis * AxisY = new QValueAxis();


    //init.Chart->setTitle("Linechart");
    init.Chart->addSeries(init.Series);

    init.DateAxisX->setFormat("hh:mm");
    init.DateAxisX->setTickCount(6);

    if((data->Cluster == Temperature)||(data->Cluster == Humidity))
        AxisY->setRange(0,100);
    else if(data->Cluster == LightStrength)
        AxisY->setRange(0,10);

    init.Chart->addAxis(init.DateAxisX, Qt::AlignBottom);
    init.Chart->addAxis(AxisY, Qt::AlignLeft);
    init.Series->attachAxis(init.DateAxisX);
    init.Series->attachAxis(AxisY);

    QPen green(Qt::red);
    green.setWidth(1);
    init.Series->setPen(green);
    init.Series->setName("Value");
    init.ChartView -> setChart(init.Chart);
    init.ChartView -> setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    uint8_t Lines = 0;

    QString Text;
    if(subpage == WidgetP2)
    {
        Lines = CountforTH;
        CountforTH++;
        Text = "Temperature or Humidity";
    }
    else if(subpage == WidgetP3)
    {
        Lines = CountforLI;
        CountforLI++;
        Text = "LightStrength";
    }
    else
    {
        delete info;
        delete data;
        return;
    }

    dynamic_cast<QGridLayout*>(subpage->layout())->addWidget(init.ChartView,Lines,0,1,3);

    init.Value = new QLabel(Text);
    dynamic_cast<QGridLayout*>(subpage->layout())->addWidget(init.Value,Lines,3,1,1,Qt::AlignCenter);
    delete info;

    Axislist.append(init);
    Update_Axis(data);
    return;

}

void MainWindow::Update_Axis(DataPull *data)
{

    uint8_t endpoint = data->Endpoint;
    uint8_t node = data->Node;
    Components_Axis temp{node,endpoint,NULL,NULL,NULL,NULL,NULL,NULL};
    if(!Axislist.contains(temp))
        return;
    temp = Axislist.value(Axislist.indexOf(temp));

    QDateTime past = QDateTime::currentDateTime().addSecs(-60*60*24);
    QDateTime now = QDateTime::currentDateTime();
    temp.DateAxisX->setRange(past,now);
    float Yvalue;
    memcpy(&Yvalue,data->Data.constData(),sizeof(Yvalue));
    //temp.Series->append(now.toMSecsSinceEpoch(),(qreal)Yvalue);
    if(data->Cluster == Temperature)
    {
        temp.Series->append(now.toMSecsSinceEpoch(),(qreal)Yvalue);
        char p[20];
        sprintf(p,"Node = %d\r\nTemp = %.2f℃",data->Node,Yvalue);
        temp.Series->setName("Temperature");
        temp.Value -> setText(p);
    }
    if(data->Cluster == Humidity)
    {
        temp.Series->append(now.toMSecsSinceEpoch(),(qreal)Yvalue);
        char p[20];
        sprintf(p,"Node = %d\r\nHumi = %.2f%%%",data->Node,Yvalue);
        temp.Series->setName("Humidity");
        temp.Value -> setText(p);
    }
    if(data->Cluster == LightStrength)
    {
        temp.Series->append(now.toMSecsSinceEpoch(),*(uint32_t *)&Yvalue);
        char p[20];
        sprintf(p,"Node = %d\r\nLight = %d",data->Node,*(uint32_t *)&Yvalue);
        temp.Series->setName("Light");
        temp.Value -> setText(p);
    }
    delete data;
}

void MainWindow::New_Control(DataforUI *info,QWidget *subpage,DataPull *data)
{
    uint8_t node = info -> Node;
    uint8_t endpoint = info -> Endpoint;

    if(data->Cluster != OnOff)//Now only support onoff
    {
        delete info;
        delete data;
        return;
    }

    Components_Control init{node,endpoint,NULL,NULL};

    init.Value = new QLabel(subpage);
    init.Onoff = new QPushButton(subpage);
    //init.Value->setStyleSheet("QLabel{background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,stop:0 yellow,stop:1 pink);}");
    init.Value->setStyleSheet("QLabel{background-color:pink;}");
    init.Value->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    init.Value->setAlignment(Qt::AlignCenter);

    uint8_t Lines = CountforEQ;
    dynamic_cast<QGridLayout*>(subpage->layout())->addWidget(init.Value,Lines,0,1,3,Qt::AlignCenter);
    dynamic_cast<QGridLayout*>(subpage->layout())->addWidget(init.Onoff,Lines,3,1,1,Qt::AlignCenter);


    CountforEQ++;
    Controllist.append(init);
    connect(init.Onoff,SIGNAL(clicked()),this,SLOT(Creat_Command_Button()));
    Update_Control(data);
    delete info;
    return;
}

void MainWindow::Update_Control(DataPull * data)
{
    uint8_t endpoint = data->Endpoint;
    uint8_t node = data->Node;
    Components_Control temp{node,endpoint,NULL,NULL};
    if(!Controllist.contains(temp))
        return;
    temp = Controllist.value(Controllist.indexOf(temp));
    char p[30];
    sprintf(p,"Node = %d Endpoint = %d State = ",data->Node,data->Endpoint);
    if(data->Data.constData() == 0)
    {
        strcat(p,"OFF");
        temp.Onoff->setText("Open");
    }
    else
    {
        strcat(p,"ON");
        temp.Onoff->setText("Close");
    }
    temp.Value->setText(p);
    delete data;
}

void MainWindow::Creat_Command_Button()
{
    QPushButton *Btn = dynamic_cast<QPushButton*>(this->sender());
    uint8_t max = Controllist.size();
    uint8_t i;
    float onoff;
    uint8_t node,endpoint;
    for(i=0; i<max; i++)
    {
        if((Controllist.value(i)).Onoff == Btn)
            break;
    }
    if(i == max)
        return;
    if(Btn->text()=="ON")
        onoff = 0;
    else
        onoff = 1;

    node = Controllist.value(i).Node;
    endpoint = Controllist.value(i).Endpoint;
    max = DataList.size();
    for(i=0; i<max; i++)
    {
        if((DataList.value(i).Node == node)&&(DataList.value(i).Endpoint == endpoint))
            break;
    }
    if(i == max)
        return;

    GenerateMessage(DataList[i].Node,DataList[i].Endpoint,DataList[i].Datatype,DataList[i].Datalength,&onoff);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this -> setWindowTitle(tr("Fire Detecting System"));
    this -> resize(800,600);
    Load_UI();
    this -> show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::Load_PageIR()
{
}

void MainWindow::Fresh_PageIR(DataPull *data, DataforUI *info)
{
    if(!DataList.contains(*info))
    {
        DataList.append(*info);
        emit Creat_Array(info,WidgetP1,data);
    }
    else
    {
        emit Fresh_Array(data);
    }
}

void MainWindow::Load_PageTH()
{

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

}

void MainWindow::Fresh_PageLI(DataPull *data, DataforUI *info)
{

    //check if this node/endpoint exist;if yes fresh,if no creat;
    if(!DataList.contains(*info))
    {
        DataList.append(*info);
        emit Creat_Axis(info,WidgetP3,data);
    }
    else
    {
        emit Fresh_Axis(data);
    }
}

void MainWindow::Load_PageEQ()
{
}

void MainWindow::Fresh_PageEQ(DataPull *data, DataforUI *info)
{

    //check if this node/endpoint exist;if yes fresh,if no creat;
    if(!DataList.contains(*info))
    {
        DataList.append(*info);
        emit Creat_Control(info,WidgetP4,data);
    }
    else
    {
        emit Fresh_Control(data);
    }
}

void MainWindow::Load_Status()
{
    Status = new QTextEdit(WidgetStatus);
    WidgetStatus -> setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);
    Status -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
    Status -> document() -> setMaximumBlockCount(50);
    QGridLayout *LayoutStatus = new QGridLayout(WidgetStatus);
    LayoutStatus -> addWidget(Status,0,0,1,1);
    Status -> setReadOnly(true);
    /*
    QFont *ft = new QFont;
    ft->setPointSize(10);
    Status -> setFont(*ft);
    */
    Status->setFontPointSize(10);
}

void MainWindow::Load_Alarm()
{
    QVBoxLayout *layout = new QVBoxLayout(WidgetAlarm);
    QLabel *Image = new QLabel(WidgetAlarm);
    QLabel *Word = new QLabel;
    WidgetAlarm -> setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);
    Image -> setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);
    Image ->setScaledContents(true);
    layout -> addWidget(Image,Qt::AlignCenter);
    layout -> addWidget(Word,Qt::AlignBottom);

    QPixmap *pixmap = new QPixmap("risk.png");
    QPixmap fixedmap = pixmap -> scaled(Image->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    Image -> setPixmap(fixedmap);
    Word -> setText("Risk!");
    Word -> setAlignment(Qt::AlignCenter);
}

void MainWindow::Fresh_Alarm(DataPull * data)
{
    delete data;
}

void MainWindow::Load_UI()
{
    MainWidget = new QWidget;
    WidgetP1 = new QWidget;
    WidgetP2 = new QWidget;
    WidgetP3 = new QWidget;
    WidgetP4 = new QWidget;
    WidgetStatus = new QWidget;
    WidgetAlarm = new QWidget;

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
    GridLayout -> addWidget(WidgetAlarm,4,3,1,1);
    GridLayout -> addWidget(StackedWidget,0,0,4,3);

    WidgetStatus -> setStyleSheet("background-color:green;");
    WidgetAlarm -> setStyleSheet("background-color:purple;");
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
    Load_Alarm();

    connect(Infrared,SIGNAL(clicked()),this,SLOT(SwitchPage_IR()));
    connect(Temp_Humi,SIGNAL(clicked()),this,SLOT(SwitchPage_TH()));
    connect(Light,SIGNAL(clicked()),this,SLOT(SwitchPage_LI()));
    connect(Equipment,SIGNAL(clicked()),this,SLOT(SwitchPage_EQ()));
    connect(this,SIGNAL(Creat_Axis(DataforUI *,QWidget *,DataPull *)),this,SLOT(New_Axis(DataforUI *,QWidget *,DataPull *)));
    connect(this,SIGNAL(Fresh_Axis(DataPull *)),this,SLOT(Update_Axis(DataPull *)));
    connect(this,SIGNAL(Creat_Array(DataforUI *,QWidget *,DataPull *)),this,SLOT(New_Array(DataforUI *,QWidget *,DataPull *)));
    connect(this,SIGNAL(Fresh_Array(DataPull *)),this,SLOT(Update_Array(DataPull *)));
    connect(this,SIGNAL(Creat_Control(DataforUI *,QWidget *,DataPull *)),this,SLOT(New_Control(DataforUI *,QWidget *,DataPull *)));
    connect(this,SIGNAL(Fresh_Control(DataPull *)),this,SLOT(Update_Control(DataPull *)));


    WidgetP1 -> setLayout(InfraredLayout);
    WidgetP2 -> setLayout(Temp_HumiLayout);
    WidgetP3 -> setLayout(LightLayout);
    WidgetP4 -> setLayout(EquipmentLayout);



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
    uint8_t datatype = Message->Datatype;
    uint8_t datalen = Message->Datalength;
    DataforUI *Info = new DataforUI{node,endpoint,control,datatype,datalen};

    switch(cluster)
    {
    case Temperature:
        if(Message->Controllable == READWRITE)
            UI.Fresh_PageEQ(Message,Info);
        else
            UI.Fresh_PageTH(Message,Info);
        break;
    case Humidity:
        if(Message->Controllable == READWRITE)
            UI.Fresh_PageEQ(Message,Info);
        else
            UI.Fresh_PageTH(Message,Info);
        break;
    case LightStrength:
        if(Message->Controllable == READWRITE)
            UI.Fresh_PageEQ(Message,Info);
        else
            UI.Fresh_PageLI(Message,Info);
        break;

    case TemperatureArray:
        if(Message->Controllable == READWRITE)
            UI.Fresh_PageEQ(Message,Info);
        else
            UI.Fresh_PageIR(Message,Info);
        break;

    case OnOff:
        UI.Fresh_PageEQ(Message,Info);
        break;
    default:
        delete Message;
        break;
    }
}

UI_Thread::UI_Thread()
{
    //this->start();
}

UI_Thread::~UI_Thread()
{

}

void UI_Thread::run()
{
    DataPull *Message;
    qRegisterMetaType<QTextCursor>("QTextCursor");

    while(1)
    {
        Message = ProcessMessage();
        if(!Message)
        {
            msleep(2);
        }
        else
        {
            //UI.Status -> append(QDateTime::currentDateTime().toString("[yyyy-M-dd hh:mm:ss]\r\n") + "Recv:" + Message->Data.toHex(' '));
            UI.Status -> append(QDateTime::currentDateTime().toString("[yyyy-M-dd hh:mm:ss]\r\n") + "Recv:" + " Node " + QString("%1").arg(Message->Node) + " Cluster " + QString("%1").arg(Message->Cluster));
            UI.Status -> moveCursor(QTextCursor::End);
            UpdateUI(Message);
        }
    }
}
