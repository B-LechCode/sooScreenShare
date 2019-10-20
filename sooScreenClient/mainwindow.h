//SooScreenServer by Simon Wezstein (B-LechCode), 2019
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

class MainWindow : public QMainWindow,private IdrawObserver
{
    Q_OBJECT
    enum section{
        transport,decomp
    };
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void workerInitialize();
    void treeviewInitialize();
    void readData();
    void writeData();
    ~MainWindow();
protected:
    virtual void drawWidgetClosing();
    virtual void closeEvent(QCloseEvent *event);
private slots:
    void on_qComboBoxCurrentIndexChanged(int idx);
    void on_qLineEditEditingFinished();
private:
    void addSettingsCat(QTreeWidget* tree, const std::vector<std::string> transBackends, parameterMap transParam, QString sectionName, section sec, int currentIndex = 0);
    /*QTreeWidgetItem* addToplevelWidgetItem(QTreeWidget* wid,QString text);
    QTreeWidgetItem* addWidgetItem(QTreeWidget* wid,QTreeWidgetItem* itm,QString text,QWidget* cwid,QString textDescr);
    QTreeWidgetItem* addWidgetItem(QTreeWidgetItem* itm,QString text,QString text2,QString textDescr);*/
    Ui::MainWindow *ui; 
    drawWidget* m_draw;
    mainWorker m_work;
    const std::string filePath = "parameters.json";
    std::vector<std::string> m_decompressBackends;
    size_t m_selectedDecompressBackend = 0;
    std::vector<std::string> m_transportBackends;
    size_t m_selectedTransportBackend = 0;
    parameterSerialization m_serialize;
    bool m_transistion = false; //This flag is used to supress ghost signals!

};

#endif // MAINWINDOW_H
