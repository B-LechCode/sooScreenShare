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
#include "drawwidget.h"
#include "mainworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,private IdrawObserver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void drawWidgetClosing();
    virtual void closeEvent(QCloseEvent *event);
private:

    Ui::MainWindow *ui; 
    drawWidget* m_draw;
    mainWorker m_work;
};

#endif // MAINWINDOW_H
