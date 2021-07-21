#include "mainwindow.h"
#include "logindialog.h"
#include "tcp_thread.h"
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    LoginDialog dlg;
    //if(dlg.exec() == QDialog::Accepted)
    if(1)
    {
        UI_Thread ui_thread;
        //TCP_Thread net_thread;
        ui_thread.UI.show();
        return a.exec();
    }
     else
        return a.exec();
}
