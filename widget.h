#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QSslSocket>
#include <QUdpSocket>

#define TCP 0
#define TLS 1

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QTcpSocket socket_t;
    QSslSocket socket_s;
    int port = 0;
    bool st =false;


public slots:
    void init();
    void doConnected();
    void doDisconnected();
    void doReadyRead();

private slots:
    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

    void on_pbClear_clicked();

    void on_cbTls_stateChanged(int arg1);

    void on_pbSend_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
