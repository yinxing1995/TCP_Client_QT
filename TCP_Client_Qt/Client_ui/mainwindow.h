#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QPushButton>
#include <QStackedWidget>
#include <QLayout>

#define PAGE_IR 0
#define PAGE_TH 1
#define PAGE_LI 2
#define PAGE_EQ 3

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *Status;

private:
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

    QWidget *MainWidget;
    QWidget *WidgetP1;
    QWidget *WidgetP2;
    QWidget *WidgetP3;
    QWidget *WidgetP4;

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
    void UpdateUI();
    void run();
};
#endif // MAINWINDOW_H
