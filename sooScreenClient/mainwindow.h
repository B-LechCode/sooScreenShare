#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define SO QTcpServer
#define S02 <QtNetwork/SO>

#include <QMainWindow>
#include <QTcpSocket>
#include S02
#include <QtNetwork/QHostAddress>
#include <QString>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();
    void on_newConnection();
    void on_timerTimeout();
    void on_socketDisconnected();
private:
    Ui::MainWindow *ui;
    SO srvr;
    QString client = "127.0.0.1";
    uint16_t port = 12345;
    QHostAddress m_clientAddr;
    QTcpSocket* m_ptrSock = nullptr;
    QTimer m_tmr;
    void writeBigCock(QByteArray &dat);
    void writeBigCock(char* data,int len);
};

#endif // MAINWINDOW_H
