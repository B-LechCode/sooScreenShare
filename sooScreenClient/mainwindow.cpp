//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "factories.h"
#include "./../header.h"
#include "./../guihelpers.h"
#include <algorithm>
#include <QComboBox>
#include <QString>
#include <QLineEdit>

#include <QPushButton>

#include "./../guihelpers.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_decompressBackends = imageDecompressorFactory::getAvailableBackends();
    m_postDecompressBackends = imagePostDecompressorFactory::getAvailableBackends();
    m_transportBackends  = transportClientFactory::getAvailableBackends();
    m_consumerBackends  = imageConsumerFactory::getAvailableBackends();

    readData();
}

void MainWindow::workerInitialize()
{
    m_work.init(m_decompressBackends[m_selectedDecompressBackend],m_postDecompressBackends[m_selectedPostDecompressBackend],m_transportBackends[m_selectedTransportBackend], m_consumerBackends[m_selectedConsumerBackend]);
    parameterMap transPortBackendParameterMap = m_serialize.getParameterMap(m_transportBackends[m_selectedTransportBackend]);
    m_work.trans()->setParameters(transPortBackendParameterMap);
    parameterMap decompBackendParameterMap = m_serialize.getParameterMap(m_decompressBackends[m_selectedDecompressBackend]);
    m_work.decomp()->setParameters(decompBackendParameterMap);
    parameterMap postDecompBackendParameterMap = m_serialize.getParameterMap(m_postDecompressBackends[m_selectedPostDecompressBackend]);
    m_work.postDecomp()->setParameters(postDecompBackendParameterMap);

    parameterMap imageConsumerParameterMap = m_serialize.getParameterMap(m_consumerBackends[m_selectedConsumerBackend]);
    m_work.imageConsumer()->setParameters(imageConsumerParameterMap);
    m_work.imageConsumer()->setObserver(this);
    treeviewInitialize();
}

void MainWindow::readData()
{
    m_serialize.readData(filePath);

    //get the selected backends
    std::string selectedDecompressBackend = m_serialize.getStringValue("Decompression");
    m_selectedDecompressBackend = GuiHelpers::findIndex(m_decompressBackends,selectedDecompressBackend);

    std::string selectedPostDecompressBackend = m_serialize.getStringValue("Postdecompression");
    m_selectedPostDecompressBackend = GuiHelpers::findIndex(m_postDecompressBackends,selectedPostDecompressBackend);

    std::string selectedTransportBackend = m_serialize.getStringValue("Transportlayer");
    m_selectedTransportBackend = GuiHelpers::findIndex(m_transportBackends,selectedTransportBackend);

    std::string selectedConsumerBackend = m_serialize.getStringValue("Consumer");
    m_selectedConsumerBackend = GuiHelpers::findIndex(m_consumerBackends,selectedConsumerBackend);

    workerInitialize();

    treeviewInitialize();
}

void MainWindow::treeviewInitialize()
{    
    ui->twSettings->clear();
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_decompressBackends,m_work.decomp()->getParameters(),QString("Decompression"),section::decomp,static_cast<int>(m_selectedDecompressBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_postDecompressBackends,m_work.postDecomp()->getParameters(),QString("Postdecompression"),section::postDecomp,static_cast<int>(m_selectedPostDecompressBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_transportBackends,m_work.trans()->getParameters(),QString("Transport layer"),section::transport,static_cast<int>(m_selectedTransportBackend));
    GuiHelpers::addSettingsCat(this,ui->twSettings,m_consumerBackends,m_work.imageConsumer()->getParameters(),QString("Consumer"),section::consumer,static_cast<int>(m_selectedConsumerBackend));
    m_transistion = false;
    ui->twSettings->expandAll();
    ui->twSettings->setColumnWidth(0, this->width()/2.5f);
    ui->twSettings->setColumnWidth(1, this->width()/12);
}

void MainWindow::writeData()
{
    //TODO: pointers!
    m_serialize.appendBackendSelection("Decompression",m_decompressBackends[m_selectedDecompressBackend]);
    m_serialize.appendParameterMap(m_work.decomp()->getParameters(),m_decompressBackends[m_selectedDecompressBackend]);
    m_serialize.appendBackendSelection("Postdecompression",m_postDecompressBackends[m_selectedPostDecompressBackend]);
    m_serialize.appendParameterMap(m_work.postDecomp()->getParameters(),m_postDecompressBackends[m_selectedPostDecompressBackend]);
    m_serialize.appendBackendSelection("Transportlayer",m_transportBackends[m_selectedTransportBackend]);
    m_serialize.appendParameterMap(m_work.trans()->getParameters(),m_transportBackends[m_selectedTransportBackend]);    
    m_serialize.appendBackendSelection("Consumer",m_consumerBackends[m_selectedConsumerBackend]);
    m_serialize.appendParameterMap(m_work.imageConsumer()->getParameters(),m_consumerBackends[m_selectedConsumerBackend]);
    m_serialize.writeData(filePath);
}

MainWindow::~MainWindow()
{
    writeData();
    delete ui;    
}

void MainWindow::drawWidgetClosing()
{
    QApplication::exit(0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::exit(0);
    event->accept();
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
        if(m_selectedTransportBackend != static_cast<size_t>(idx))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedTransportBackend = static_cast<size_t>(idx);
            workerInitialize();            
        }
        break;
        case decomp:
        if(m_selectedDecompressBackend != static_cast<size_t>(idx))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedDecompressBackend = static_cast<size_t>(idx);
            workerInitialize();            
        }
        break;
        case postDecomp:
        if(m_selectedPostDecompressBackend != static_cast<size_t>(idx))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedPostDecompressBackend = static_cast<size_t>(idx);
            workerInitialize();
        }
        break;
        case consumer:
        if(m_selectedConsumerBackend != static_cast<size_t>(idx))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedConsumerBackend = static_cast<size_t>(idx);
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
        case decomp:            
        {            
            m_work.decomp()->setParameterValue(key,value);
        }
        break;
        case postDecomp:
        {
            m_work.postDecomp()->setParameterValue(key,value);
        }
        break;
        case consumer:
        {
            m_work.imageConsumer()->setParameterValue(key,value);
        }
    }
}
