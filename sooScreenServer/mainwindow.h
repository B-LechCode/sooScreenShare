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
#include "./../parameterserialization.h"

namespace Ui {
class MainWindow;
}

class MainWindow :  public QMainWindow
{
    Q_OBJECT
    enum section{
        transport,comp,screenshot
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_timerTimeout();
    void on_qLineEditEditingFinished();
    void on_qComboBoxCurrentIndexChanged(int idx);

private:
    void workerInitialize();
    void readData();
    void writeData();
    void treeviewInitialize();

    Ui::MainWindow *ui;
    QTimer m_tmr;
    const std::string filePath = "parameters.json";
    const int64_t m_timePreference = 30;
    uint64_t m_sumTime = 0;
    uint64_t m_couterTime = 0;
    uint64_t m_minTime = 999999999999;
    uint64_t m_maxTime = 0;
    mainWorker m_work;
    std::vector<std::string> m_compressBackends;
    size_t m_selectedCompressBackend = 0;
    std::vector<std::string> m_transportBackends;
    size_t m_selectedTransportBackend = 0;
    std::vector<std::string> m_screenshotBackends;
    size_t m_selectedScreenshotBackend = 0;
    parameterSerialization m_serialize;
    bool m_transistion = false; //This flag is used to supress ghost signals!
    void writeBackendParameter(std::string backType,std::string backName);
};

#endif // MAINWINDOW_H
