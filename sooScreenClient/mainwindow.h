//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "drawwidget.h"
#include "mainworker.h"
#include "./../parameterserialization.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The mainwindow class of the client
 *
 */
class MainWindow : public QMainWindow,private IdrawObserver
{
    Q_OBJECT
    /**
     * @brief The enumeration for the backend sections
     * This is used to distinct the settings sections of the tree view
     */
    enum section{
        transport,decomp,postDecomp
    };

public:
    /**
     * @brief The standard constructor
     *
     * @param parent The parent widget
     */
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
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
     * @brief Close event of the draw widget
     * This will close the whole application
     */
    virtual void drawWidgetClosing();
    /**
     * @brief The close event of MainWindow
     * This will close the draw widget
     * @param event
     */
    virtual void closeEvent(QCloseEvent *event);
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
    mainWorker m_work; /**< The main worker instance */
    const std::string filePath = "parameters.json"; /**< File path to the config file */
    std::vector<std::string> m_decompressBackends; /**< The available decompress backend names */
    size_t m_selectedDecompressBackend = 0; /**< The index of the current selected decompress backend */

    std::vector<std::string> m_postDecompressBackends; /**< The available decompress backend names */
    size_t m_selectedPostDecompressBackend = 0; /**< The index of the current selected decompress backend */

    std::vector<std::string> m_transportBackends; /**< The available transport backend names */
    size_t m_selectedTransportBackend = 0; /**< The index of the current selected transport backend */

    std::vector<std::string> m_consumerBackends; /**< The available decompress backend names */
    size_t m_selectedConsumerBackend = 0; /**< The index of the current selected decompress backend */

    parameterSerialization m_serialize; /**< Parameter serializer */
    bool m_transistion = false; /**< This flag is used to supress ghost signals from destructed Qt-Gui widgets */

};

#endif // MAINWINDOW_H
