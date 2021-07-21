#include "logindialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    this->resize(400, 300);
    this->setWindowTitle(tr("Login..."));
    Qt::WindowFlags flags=Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    usrLabel = new QLabel(this);
    usrLabel -> move(70, 80);
    usrLabel -> setText(tr("usrname"));

    usrLineEdit = new QLineEdit(this);
    usrLineEdit->move(160, 80);
    usrLineEdit->setPlaceholderText(tr("please fill user name"));

    pwdLabel = new QLabel(this);
    pwdLabel -> move(70, 130);
    pwdLabel -> setText(tr("password"));

    pwdLineEdit = new QLineEdit(this);
    pwdLineEdit -> move(160, 130);
    pwdLineEdit -> setPlaceholderText(tr("please fill password"));

    loginBtn = new QPushButton(this);
    loginBtn -> move(70, 200);
    loginBtn -> setText(tr("login"));

    exitBtn = new QPushButton(this);
    exitBtn -> move(230, 200);
    exitBtn -> setText(tr("exit"));

    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::login);
    connect(exitBtn, &QPushButton::clicked, this, &LoginDialog::close);
}

LoginDialog::~LoginDialog()
{

}

void LoginDialog::login()
{
    if(usrLineEdit -> text().trimmed() == tr("c00576189") && pwdLineEdit->text() == tr("yinxing007"))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, tr("warning"), tr("wrong usrname or password!"), QMessageBox::Yes);
        usrLineEdit -> clear();
        pwdLineEdit -> clear();
        usrLineEdit -> setFocus();
    }
}
