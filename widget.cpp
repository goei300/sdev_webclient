#include "widget.h"
#include "ui_widget.h"
#include <string>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QFile file("settings.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // 화면 위치 불러오기
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 2) {
            int x = parts.at(0).toInt();
            int y = parts.at(1).toInt();
            this->move(x, y);
        }

        // LineEdit 내용 불러오기
        if (!in.atEnd()) {
            ui->leHost->setText(in.readLine());
        }

        // CheckBox 상태 불러오기
        if (!in.atEnd()) {
            QString checked = in.readLine();
            ui->cbTls->setChecked(checked == "true");
        }
    }
    file.close();

    //socket_.connect();

    //socket_.readyRead();
    QObject::connect(&socket_t, &QAbstractSocket::connected,this, &Widget::doConnected);
    QObject::connect(&socket_t, &QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&socket_t, &QAbstractSocket::readyRead,this,&Widget::doReadyRead);

    QObject::connect(&socket_s, &QAbstractSocket::connected,this, &Widget::doConnected);
    QObject::connect(&socket_s, &QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&socket_s, &QAbstractSocket::readyRead,this,&Widget::doReadyRead);
    init();
}

Widget::~Widget()
{
    QFile file("settings.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // 화면 위치 저장
        out << this->pos().x() << "," << this->pos().y() << "\n";

        // LineEdit 내용 저장
        out << ui->leHost->text() << "\n";

        // CheckBox 상태 저장 (체크되었는지)
        out << (ui->cbTls->isChecked() ? "true" : "false") << "\n";
    }
    file.close();

    delete ui;
}

void Widget::init(){
    ui->pbDisconnect->setEnabled(st);
    ui->pbConnect->setEnabled(!st);
    ui->pbSend->setEnabled(st);
    ui->cbTls->setEnabled(!st);
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
            ui->pteMessage->insertPlainText("TCP ON! \n");
            socket_t.connectToHost(ui->leHost->text(),80);
            break;
        case TLS:
            ui->pteMessage->insertPlainText("TLS ON! \n");
            socket_s.connectToHostEncrypted(ui->leHost->text(), 443);
            break;
    }

}


void Widget::on_pbDisconnect_clicked()
{
    if (socket_t.state() == QAbstractSocket::ConnectedState) {
            socket_t.close();
            ui->pteMessage->insertPlainText("TCP Disconnected\n");
    }

    if (socket_s.state() == QAbstractSocket::ConnectedState) {
            socket_s.close();
            ui->pteMessage->insertPlainText("TLS Disconnected\n");
    }
}

void Widget::on_pbSend_clicked()
{
    switch(port)
    {
    case TCP:
            if (socket_t.state() == QAbstractSocket::ConnectedState) {
                socket_t.write(ui->pteSend->toPlainText().toUtf8());
            }
            break;
    case TLS:
            if (socket_s.state() == QAbstractSocket::ConnectedState) {
                socket_s.write(ui->pteSend->toPlainText().toUtf8());
            }
            break;
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
