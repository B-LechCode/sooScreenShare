//sooScreenShare by Simon Wezstein (B-LechCode), 2019
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

    m_preCompressBackends = imagePreCompressorFactory::getAvailableBackends();
    m_compressBackends = imageCompressorFactory::getAvailableBackends();
    m_screenshotBackends = screenShotFactory::getAvailableBackends();
    m_transportBackends = transportServerFactory::getAvailableBackends();

    readData();
    m_tmr.setSingleShot(true);
    m_tmr.start(m_timePreference);
}


void MainWindow::on_timerTimeout()
{
    QElapsedTimer tmr;
    tmr.start();
    m_work.run();
    int64_t eleapsed = tmr.elapsed();
    int64_t newWaitTime = m_timePreference-eleapsed;
    if(newWaitTime<0)
        newWaitTime = 0;
    std::cout << eleapsed << std::endl;
    m_tmr.start(newWaitTime);
}

MainWindow::~MainWindow()
{
    m_tmr.stop();
    writeData();
    delete ui;
}

void MainWindow::workerInitialize()
{    
    m_work.init(m_screenshotBackends[m_selectedScreenshotBackend],m_preCompressBackends[m_selectedPreCompressBackend],m_compressBackends[m_selectedCompressBackend],m_transportBackends[m_selectedTransportBackend]);

    parameterMap screenShotBackendParameterMap = m_serialize.getParameterMap(m_screenshotBackends[m_selectedScreenshotBackend]);
    m_work.screen()->setParameters(screenShotBackendParameterMap);

    parameterMap transPortBackendParameterMap = m_serialize.getParameterMap(m_transportBackends[m_selectedTransportBackend]);
    m_work.trans()->setParameters(transPortBackendParameterMap);

    parameterMap preCompBackendParameterMap = m_serialize.getParameterMap(m_preCompressBackends[m_selectedPreCompressBackend]);
    m_work.preComp()->setParameters(preCompBackendParameterMap);

    parameterMap compBackendParameterMap = m_serialize.getParameterMap(m_compressBackends[m_selectedCompressBackend]);
    m_work.comp()->setParameters(compBackendParameterMap);

    treeviewInitialize();
}

void MainWindow::readData()
{
    m_serialize.readData(m_filePath);

    //get the selected backends
    std::string selectedScreenshotBackend = m_serialize.getStringValue("Screenshot");
    m_selectedScreenshotBackend = GuiHelpers::findIndex(m_screenshotBackends,selectedScreenshotBackend);

    std::string selectedPreCompressBackend = m_serialize.getStringValue("Precompression");
    m_selectedPreCompressBackend = GuiHelpers::findIndex(m_preCompressBackends,selectedPreCompressBackend);

    std::string selectedCompressBackend = m_serialize.getStringValue("Compression");
    m_selectedCompressBackend = GuiHelpers::findIndex(m_compressBackends,selectedCompressBackend);

    std::string selectedTransportBackend = m_serialize.getStringValue("Transportlayer");
    m_selectedTransportBackend = GuiHelpers::findIndex(m_transportBackends,selectedTransportBackend);

    workerInitialize();

    treeviewInitialize();

}

void MainWindow::treeviewInitialize()
{
    ui->twSettings->clear();
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_screenshotBackends,m_work.screen()->getParameters(),QString("Screenshot"),section::screenshot,static_cast<int>(m_selectedScreenshotBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_preCompressBackends,m_work.preComp()->getParameters(),QString("Precompression"),section::preComp,static_cast<int>(m_selectedPreCompressBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_compressBackends,m_work.comp()->getParameters(),QString("Compression"),section::comp,static_cast<int>(m_selectedCompressBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_transportBackends,m_work.trans()->getParameters(),QString("Transport layer"),section::transport,static_cast<int>(m_selectedTransportBackend));
    m_transistion = false;
    ui->twSettings->expandAll();
    ui->twSettings->setColumnWidth(0, this->width()/2.5f);
    ui->twSettings->setColumnWidth(1, this->width()/12);
}

void MainWindow::writeData()
{
    //TODO: pointers!

    m_serialize.appendBackendSelection("Precompression",m_preCompressBackends[m_selectedPreCompressBackend]);
    m_serialize.appendParameterMap(m_work.preComp()->getParameters(),m_preCompressBackends[m_selectedPreCompressBackend]);

    m_serialize.appendBackendSelection("Compression",m_compressBackends[m_selectedCompressBackend]);
    m_serialize.appendParameterMap(m_work.comp()->getParameters(),m_compressBackends[m_selectedCompressBackend]);

    m_serialize.appendBackendSelection("Screenshot",m_screenshotBackends[m_selectedScreenshotBackend]);
    m_serialize.appendParameterMap(m_work.screen()->getParameters(),m_screenshotBackends[m_selectedScreenshotBackend]);

    m_serialize.appendBackendSelection("Transportlayer",m_transportBackends[m_selectedTransportBackend]);
    m_serialize.appendParameterMap(m_work.trans()->getParameters(),m_transportBackends[m_selectedTransportBackend]);
    m_serialize.writeData(m_filePath);
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
        }
        break;
        case preComp:
        if(m_selectedPreCompressBackend != static_cast<size_t>(cb->currentIndex()))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedPreCompressBackend = static_cast<size_t>(cb->currentIndex());
            workerInitialize();            
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
            m_work.trans()->setParameterValue(key,value);
        }
        break;
        case comp:
        {            
            m_work.comp()->setParameterValue(key,value);
        }
        break;
        case preComp:
        {
            m_work.preComp()->setParameterValue(key,value);
        }
        break;
        case screenshot:
        {
            m_work.screen()->setParameterValue(key,value);            
        }
        break;
    }
}

