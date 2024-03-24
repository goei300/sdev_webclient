#include "widget.h"
#include "ui_widget.h"
#include <string>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //socket_.connect();

    //socket_.readyRead();
    init();
    QObject::connect(&socket_t, &QAbstractSocket::connected,this, &Widget::doConnected);
    QObject::connect(&socket_t, &QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&socket_t, &QAbstractSocket::readyRead,this,&Widget::doReadyRead);

    QObject::connect(&socket_s, &QAbstractSocket::connected,this, &Widget::doConnected);
    QObject::connect(&socket_s, &QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&socket_s, &QAbstractSocket::readyRead,this,&Widget::doReadyRead);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::init(){
    ui->pbDisconnect->setEnabled(st);
    ui->pbConnect->setEnabled(!st);
}
void Widget::doConnected(){
    ui->pteMessage->insertPlainText("conntected\n");
    st=true;
    init();
}

void Widget::doDisconnected(){
    ui->pteMessage->insertPlainText("Disconnected\n");
    st=false;
    init();
}

void Widget::doReadyRead(){
    ui->pteMessage->insertPlainText(socket_t.readAll());
}

void Widget::on_pbConnect_clicked()
{
    switch(port)
    {
    case TCP:
        socket_t.connectToHost(ui->leHost->text(),80);
    case TLS:
        socket_s.connectToHostEncrypted(ui->leHost->text(), 443);
    }

}


void Widget::on_pbDisconnect_clicked()
{
    switch(port)
    {
    case TCP:
        socket_t.close();
    case TLS:
        socket_s.close();
    }
}


void Widget::on_pbSend_clicked()
{
    switch(port)
    {
    case TCP:
        socket_t.write(ui->pteSend->toPlainText().toUtf8());
    case TLS:
        socket_s.write(ui->pteSend->toPlainText().toUtf8());
    }
}



void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}


void Widget::on_cbTls_stateChanged(int arg1)
{
    port = arg1==2 ? 1 : 0;
}



