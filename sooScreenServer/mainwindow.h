//sooScreenShare by Simon Wezstein (B-LechCode), 2019
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

/**
 * @brief The mainwindow class of the server
 *
 */
class MainWindow :  public QMainWindow
{
    Q_OBJECT
    /**
     * @brief The enumeration for the backend sections
     * This is used to distinct the settings sections of the tree view
     */
    enum section{
        transport,comp,screenshot
    };

public:
    /**
     * @brief The standard constructor
     *
     * @param parent The parent widget
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /**
     * @brief The destructor
     *
     */
    ~MainWindow();

private slots:
    /**
     * @brief The timeout of the work timer
     * Everytime this timer triggers, one frame is sent over the network.
     */
    void on_timerTimeout();
    /**
     * @brief Changed event of the backend selection combo boxes
     * Every created combo box is connected to this slot, when emitted the right
     * backend section is selected.
     * @param idx The new Index
     */
    void on_qComboBoxCurrentIndexChanged(int idx);
    /**
     * @brief Changed event of the QLineEdits
     * Every created QLineEdit is linked to this slot, when emitted the right
     * backend section and parameter name is selected.
     */
    void on_qLineEditEditingFinished();
private:
    /**
     * @brief (Re)Initialize the main worker
     *
     */
    void workerInitialize();
    /**
     * @brief (Re)Initialize the tree view
     *
     */
    void treeviewInitialize();
    /**
     * @brief Read data from config
     *
     */
    void readData();
    /**
     * @brief Write data to config
     *
     */
    void writeData();
    /**
     * @brief The destructor
     *
     */

    Ui::MainWindow *ui; /**< Pointer to the own UI */
    QTimer m_tmr; /**< The work timer */
    const std::string m_filePath = "parameters.json"; /**< File path to the config file */
    const int64_t m_timePreference = 35; /**< The work timer period time */
    mainWorker m_work; /**< The main worker instance */
    std::vector<std::string> m_compressBackends; /**< The available compress backend names */
    size_t m_selectedCompressBackend = 0; /**< The index of the current selected compress backend */
    std::vector<std::string> m_transportBackends; /**< The available transport backend names */
    size_t m_selectedTransportBackend = 0; /**< The index of the current selected transport backend */
    std::vector<std::string> m_screenshotBackends; /**< The available screenshot backend names */
    size_t m_selectedScreenshotBackend = 0; /**< The index of the current screenshot backend */
    parameterSerialization m_serialize; /**< Parameter serializer */
    bool m_transistion = false; /**< This flag is used to supress ghost signals from destructed Qt-Gui widgets */
};

#endif // MAINWINDOW_H
