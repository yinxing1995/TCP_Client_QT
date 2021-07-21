#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;


class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
private:
    QLabel *usrLabel;
    QLabel *pwdLabel;
    QLineEdit *usrLineEdit;
    QLineEdit *pwdLineEdit;
    QPushButton *loginBtn;
    QPushButton *exitBtn;
private slots:
    void login();
};

#endif // LOGINDIALOG_H
