#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "factories.h"
#include "./../header.h"
#include <algorithm>
#include <QComboBox>
#include <QString>
#include <QLineEdit>

#include <QPushButton>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define PARAMETER_PROPERTY "parameter"
#define SECTION_PROPERTY "sec"

QTreeWidgetItem* MainWindow::addToplevelWidgetItem(QTreeWidget* wid,QString text)
{
    QTreeWidgetItem* itm = new QTreeWidgetItem(wid);
    itm->setText(0,text);
    return itm;
}

QTreeWidgetItem* MainWindow::addWidgetItem(QTreeWidgetItem* itm,QString text,QString text2,QString textDescr)
{
    QTreeWidgetItem* itm2 = new QTreeWidgetItem(itm);
    itm2->setText(0,text);
    itm2->setText(1,text2);
    itm2->setText(2,textDescr);


    return itm2;
}

QTreeWidgetItem* MainWindow::addWidgetItem(QTreeWidget* wid,QTreeWidgetItem* itm,QString text,QWidget* cwid,QString textDescr)
{
    QTreeWidgetItem* itm2 = new QTreeWidgetItem(itm);
    itm2->setText(0,text);
    wid->setItemWidget(itm2,1,cwid);
    itm2->setText(2,textDescr);

    return itm2;
}

void MainWindow::addSettingsCat(QTreeWidget* tree,const std::vector<std::string> transBackends,parameterMap transParam,QString sectionName,section sec,int currentIndex)
{
    QTreeWidgetItem* itmBackendSection = addToplevelWidgetItem(tree,sectionName);

    //Backend selector
    QComboBox* cboxTrans = new QComboBox();

    for(uint32_t i = 0;i<transBackends.size();++i)
        cboxTrans->addItem(QString(transBackends[i].c_str()));

    cboxTrans->setProperty(SECTION_PROPERTY,QVariant(sec));    
    cboxTrans->setCurrentIndex(currentIndex);

    connect(cboxTrans,SIGNAL(currentIndexChanged(int)),this,SLOT(on_qComboBoxCurrentIndexChanged(int)));

    QTreeWidgetItem* transItem = addWidgetItem(tree,itmBackendSection,sectionName,cboxTrans,"");


    for(auto i = transParam.begin();i!=transParam.end();++i)
    {
        QLineEdit* ledit = new QLineEdit();
        ledit->setText((*i).second.value().c_str());

        ledit->setProperty(PARAMETER_PROPERTY,QVariant(QString((*i).first.c_str())));
        ledit->setProperty(SECTION_PROPERTY,QVariant(sec));

        addWidgetItem(tree,transItem,QString((*i).first.c_str()),ledit,QString((*i).second.description().c_str()));
        connect(ledit,SIGNAL(editingFinished()),this,SLOT(on_qLineEditEditingFinished()));
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_draw = new drawWidget();
    m_draw->setObserver(this);
    m_draw->show();


    m_decompressBackends = imageDecompressorFactory::getAvailableBackends();
    m_transportBackends  = transportClientFactory::getAvailableBackends();

    readData();
}

void MainWindow::workerInitialize()
{
    m_work.init(m_decompressBackends[m_selectedDecompressBackend],m_transportBackends[m_selectedTransportBackend],m_draw);
    parameterMap transPortBackendParameterMap = m_serialize.getParameterMap(m_transportBackends[m_selectedTransportBackend]);
    m_work.trans()->setParameters(transPortBackendParameterMap);
    parameterMap decompBackendParameterMap = m_serialize.getParameterMap(m_decompressBackends[m_selectedDecompressBackend]);
    m_work.decomp()->setParameters(decompBackendParameterMap);
    treeviewInitialize();
}

size_t findIndex(std::vector<std::string>& vec,std::string str)
{
    for (size_t i = 0; i<vec.size();++i)
    {
       if(vec[i] == str)
           return i;
    }
    return 0;
}

void MainWindow::readData()
{
    m_serialize.readData(filePath);

    //get the selected backends
    std::string selectedDecompressBackend = m_serialize.getStringValue("Decompression");
    m_selectedDecompressBackend = findIndex(m_decompressBackends,selectedDecompressBackend);

    std::string selectedTransportBackend = m_serialize.getStringValue("Transportlayer");
    m_selectedTransportBackend = findIndex(m_transportBackends,selectedTransportBackend);

    workerInitialize();

    treeviewInitialize();
    m_work.run();


}

void MainWindow::treeviewInitialize()
{    
    ui->twSettings->clear();
    addSettingsCat(ui->twSettings,m_decompressBackends,m_work.decomp()->getParameters(),QString("Decompression"),section::decomp,static_cast<int>(m_selectedDecompressBackend));
    addSettingsCat(ui->twSettings,m_transportBackends,m_work.trans()->getParameters(),QString("Transport layer"),section::transport,static_cast<int>(m_selectedTransportBackend));
    m_transistion = false;
    ui->twSettings->expandAll();
}

void MainWindow::writeData()
{
    //TODO: pointers!
    m_serialize.appendBackendSelection("Decompression",m_decompressBackends[m_selectedDecompressBackend]);
    m_serialize.appendParameterMap(m_work.decomp()->getParameters(),m_decompressBackends[m_selectedDecompressBackend]);
    m_serialize.appendBackendSelection("Transportlayer",m_transportBackends[m_selectedTransportBackend]);
    m_serialize.appendParameterMap(m_work.trans()->getParameters(),m_transportBackends[m_selectedTransportBackend]);
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
        case decomp:
        if(m_selectedDecompressBackend != static_cast<size_t>(cb->currentIndex()))
        {
            m_transistion = true;
            writeData();
            m_work.end();
            m_selectedDecompressBackend = static_cast<size_t>(cb->currentIndex());
            workerInitialize();
            m_work.run();
        }
        break;
    }

}

parameterMap setParameterValue(parameterMap map,std::string key,std::string value)
{
    map[key].setValue(value);
    return map;
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
            auto paraMap = setParameterValue(m_work.trans()->getParameters(),key,value);
            m_work.trans()->setParameters(paraMap);
        }
        break;
        case decomp:            
        {
            auto paraMap = setParameterValue(m_work.decomp()->getParameters(),key,value);
            m_work.decomp()->setParameters(paraMap);
        }
        break;
    }
}
