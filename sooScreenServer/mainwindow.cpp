//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QLineEdit>
#include <QElapsedTimer>
#include "./../guihelpers.h"
#include "factories.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&m_tmr,SIGNAL(timeout()),this,SLOT(on_timerTimeout()));
    //m_tmr.setSingleShot(true);

    m_compressBackends = imageCompressorFactory::getAvailableBackends();
    m_screenshotBackends = screenShotFactory::getAvailableBackends();
    m_transportBackends = transportServerFactory::getAvailableBackends();

    //m_work.init();
    //screenShotSdl dsd;
    //dsd.operator()();

    readData();
    m_tmr.start(33);
}


void MainWindow::on_timerTimeout()
{
    QElapsedTimer tmr;
    tmr.start();
    m_work.run();

}

MainWindow::~MainWindow()
{
    writeData();
    delete ui;
}

void MainWindow::workerInitialize()
{
    //TODO: Add parameters for screenshot!
    m_work.init(m_screenshotBackends[m_selectedScreenshotBackend],m_compressBackends[m_selectedCompressBackend],m_transportBackends[m_selectedTransportBackend]);

    parameterMap screenShotBackendParameterMap = m_serialize.getParameterMap(m_screenshotBackends[m_selectedScreenshotBackend]);
    m_work.screen()->setParameters(screenShotBackendParameterMap);

    parameterMap transPortBackendParameterMap = m_serialize.getParameterMap(m_transportBackends[m_selectedTransportBackend]);
    m_work.trans()->setParameters(transPortBackendParameterMap);

    parameterMap compBackendParameterMap = m_serialize.getParameterMap(m_compressBackends[m_selectedCompressBackend]);
    m_work.comp()->setParameters(compBackendParameterMap);

    treeviewInitialize();
}

void MainWindow::readData()
{
    m_serialize.readData(filePath);

    //get the selected backends
    std::string selectedScreenshotBackend = m_serialize.getStringValue("Screenshot");
    m_selectedScreenshotBackend = GuiHelpers::findIndex(m_screenshotBackends,selectedScreenshotBackend);

    std::string selectedCompressBackend = m_serialize.getStringValue("Compression");
    m_selectedCompressBackend = GuiHelpers::findIndex(m_compressBackends,selectedCompressBackend);

    std::string selectedTransportBackend = m_serialize.getStringValue("Transportlayer");
    m_selectedTransportBackend = GuiHelpers::findIndex(m_transportBackends,selectedTransportBackend);

    workerInitialize();

    treeviewInitialize();
    m_work.run();


}

void MainWindow::treeviewInitialize()
{
    ui->twSettings->clear();
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_screenshotBackends,m_work.screen()->getParameters(),QString("Screenshot"),section::screenshot,static_cast<int>(m_selectedScreenshotBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_compressBackends,m_work.comp()->getParameters(),QString("Compression"),section::comp,static_cast<int>(m_selectedCompressBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_transportBackends,m_work.trans()->getParameters(),QString("Transport layer"),section::transport,static_cast<int>(m_selectedTransportBackend));
    m_transistion = false;
    ui->twSettings->expandAll();
}

void MainWindow::writeData()
{
    //TODO: pointers!

    m_serialize.appendBackendSelection("Compression",m_compressBackends[m_selectedCompressBackend]);
    m_serialize.appendParameterMap(m_work.comp()->getParameters(),m_compressBackends[m_selectedCompressBackend]);

    m_serialize.appendBackendSelection("Screenshot",m_screenshotBackends[m_selectedScreenshotBackend]);
    m_serialize.appendParameterMap(m_work.screen()->getParameters(),m_screenshotBackends[m_selectedScreenshotBackend]);

    m_serialize.appendBackendSelection("Transportlayer",m_transportBackends[m_selectedTransportBackend]);
    m_serialize.appendParameterMap(m_work.trans()->getParameters(),m_transportBackends[m_selectedTransportBackend]);
    m_serialize.writeData(filePath);
}

void MainWindow::on_qComboBoxCurrentIndexChanged(int idx)
{
    if(m_transistion)
        return;

    QComboBox* cb = reinterpret_cast<QComboBox*>(sender());
    std::cout << cb->currentText().toStdString() << std::endl;

    section sec = static_cast<section>(cb->property(SECTION_PROPERTY).toInt());


    switch(sec)
    {
        case transport:
        if(m_selectedTransportBackend != static_cast<size_t>(cb->currentIndex()))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedTransportBackend = static_cast<size_t>(cb->currentIndex());
            workerInitialize();
            m_work.run();
        }
        break;
        case comp:
        if(m_selectedCompressBackend != static_cast<size_t>(cb->currentIndex()))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedCompressBackend = static_cast<size_t>(cb->currentIndex());
            workerInitialize();
            m_work.run();
        }
        break;
        case screenshot:
        if(m_selectedScreenshotBackend != static_cast<size_t>(cb->currentIndex()))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedScreenshotBackend = static_cast<size_t>(cb->currentIndex());
            workerInitialize();
            m_work.run();
        }
        break;
    }

}



void MainWindow::on_qLineEditEditingFinished()
{
    if(m_transistion)
        return;

    QLineEdit* le = reinterpret_cast<QLineEdit*>(sender());
    std::cout <<  le->text().toStdString() << " " << le->property(PARAMETER_PROPERTY).toString().toStdString() << std::endl;
    std::cout <<  le->text().toStdString() << " " << le->property(SECTION_PROPERTY).toString().toStdString() << std::endl;

    section sec = static_cast<section>(le->property(SECTION_PROPERTY).toInt());
    std::string key = le->property(PARAMETER_PROPERTY).toString().toStdString();
    std::string value = le->text().toStdString();
    switch(sec)
    {
        case transport:
        {
            auto paraMap = parameterMap::setParameterValue(m_work.trans()->getParameters(),key,value);
            m_work.trans()->setParameters(paraMap);
        }
        break;
        case comp:
        {
            auto paraMap = parameterMap::setParameterValue(m_work.comp()->getParameters(),key,value);
            m_work.comp()->setParameters(paraMap);
        }
        break;
        case screenshot:
        {
            m_work.screen()->setParameterValue(key,value);            
        }
        break;
    }
}

