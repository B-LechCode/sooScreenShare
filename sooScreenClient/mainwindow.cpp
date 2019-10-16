#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "factories.h"
#include "./../header.h"
#include <QComboBox>
#include <QString>
#include <QLineEdit>


QTreeWidgetItem* addToplevelWidgetItem(QTreeWidget* wid,QString text)
{
    QTreeWidgetItem* itm = new QTreeWidgetItem(wid);
    itm->setText(0,text);
    return itm;
}

QTreeWidgetItem* addWidgetItem(QTreeWidgetItem* itm,QString text,QString text2,QString textDescr)
{
    QTreeWidgetItem* itm2 = new QTreeWidgetItem(itm);
    itm2->setText(0,text);
    itm2->setText(1,text2);
    itm2->setText(2,textDescr);
    return itm2;
}

QTreeWidgetItem* addWidgetItem(QTreeWidget* wid,QTreeWidgetItem* itm,QString text,QWidget* cwid,QString textDescr)
{
    QTreeWidgetItem* itm2 = new QTreeWidgetItem(itm);
    itm2->setText(0,text);
    wid->setItemWidget(itm2,1,cwid);
    itm2->setText(2,textDescr);
    return itm2;
}

void addSettingsCat(QTreeWidget* tree,const std::vector<std::string> transBackends,parameterMap transParam,QString sectionName)
{
    QTreeWidgetItem* itmBackendSection = addToplevelWidgetItem(tree,sectionName);

    //Backend selector
    QComboBox* cboxTrans = new QComboBox();

    for(uint32_t i = 0;i<transBackends.size();++i)
        cboxTrans->addItem(QString(transBackends[i].c_str()));


    QTreeWidgetItem* transItem = addWidgetItem(tree,itmBackendSection,sectionName,cboxTrans,"");

    for(auto i = transParam.begin();i!=transParam.end();++i)
    {
        QLineEdit* ledit = new QLineEdit();
        ledit->setText((*i).second.value().c_str());
        addWidgetItem(tree,transItem,QString((*i).first.c_str()),ledit,QString((*i).second.description().c_str()));
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
    m_work.init(compressback,qtTcpclient,m_draw);

    addSettingsCat(ui->twSettings,imageDecompressorFactory::getAvailableBackends(),m_work.decomp()->getParameters(),QString("Decompression"));
    addSettingsCat(ui->twSettings,transportClientFactory::getAvailableBackends(),m_work.trans()->getParameters(),QString("Transport layer"));
}



MainWindow::~MainWindow()
{
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
