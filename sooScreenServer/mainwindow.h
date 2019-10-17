//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    void on_timerTimeout();

private:
    Ui::MainWindow *ui;
    QTimer m_tmr;

    const int64_t m_timePreference = 30;
    uint64_t m_sumTime = 0;
    uint64_t m_couterTime = 0;
    uint64_t m_minTime = 999999999999;
    uint64_t m_maxTime = 0;
    mainWorker m_work;
};

#endif // MAINWINDOW_H
