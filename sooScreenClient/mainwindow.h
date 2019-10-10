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
    void on_socketConnected();
    void on_timerTimeout();
    void on_socketDisconnected();
private:
    Ui::MainWindow *ui;

    QString client = "127.0.0.1";
    uint16_t port = 12345;
    QHostAddress m_clientAddr;
    QTcpSocket* m_ptrSock = nullptr;
    QTimer m_tmr;    
};

#endif // MAINWINDOW_H
