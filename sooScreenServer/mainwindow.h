#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define SO QTcpSocket
#define S02 <QtNetwork/SO>

#include <QMainWindow>
#include <vector>

#include S02
#include <QtNetwork/QHostAddress>
#include <QString>
#include <QTimer>
#include "mainworker.h"
#include "iscreenshot.h"
#include "iimagecompressor.h"
#include "itransportserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow :  public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_socketConnected();
    void on_socketDisconnected();
    void on_timerTimeout();

private:
    Ui::MainWindow *ui;

    IscreenShot* m_screen = nullptr;
    IImageCompressor* m_comp = nullptr;
    ItransportServer* m_trans = nullptr;
    //std::vector<uint8_t> m_sendbuffer;
    uint8_t*    m_sendbuffer = nullptr;
    uint64_t         m_bufferSize;

    SO sock;
    //QString client = "192.168.0.2";
    QString client = "127.0.0.1";
    uint16_t port = 12345;
    QHostAddress m_clientAddr;
    QTimer m_tmr;

    void createHeader();
    void insertHeaderNumBytes(int byteCount);
    void pbc();
    const int64_t m_timePreference = 30;
    uint64_t m_sumTime = 0;
    uint64_t m_couterTime = 0;
    uint64_t m_minTime = 999999999999;
    uint64_t m_maxTime = 0;
    mainWorker m_work;
};

#endif // MAINWINDOW_H
