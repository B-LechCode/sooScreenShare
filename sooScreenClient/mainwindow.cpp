#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "factories.h"

#include <QComboBox>
#include <QString>


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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_draw = new drawWidget();
    m_draw->setObserver(this);
    m_draw->show();
    m_work.init(m_draw);




    QTreeWidgetItem* itmCompressionSection = addToplevelWidgetItem(ui->twSettings,"Image compression");

    std::vector<std::string> decBackends = imageDecompressorFactory::getAvailableBackends();

    QComboBox* cboxComp = new QComboBox();

    for(uint32_t i = 0;i<decBackends.size();++i)
        cboxComp->addItem(QString(decBackends[i].c_str()));


    addWidgetItem(ui->twSettings,itmCompressionSection,"Image decompressor",cboxComp,"QString textDescr");



    QTreeWidgetItem* itmBackendSection = addToplevelWidgetItem(ui->twSettings,"Transportlayer");

    std::vector<std::string> transBackends = transportClientFactory::getAvailableBackends();

    QComboBox* cboxTrans = new QComboBox();

    for(uint32_t i = 0;i<decBackends.size();++i)
        cboxTrans->addItem(QString(transBackends[i].c_str()));


    QTreeWidgetItem* transItem = addWidgetItem(ui->twSettings,itmBackendSection,"Transportlayer",cboxTrans,"QString textDescr");

    addWidgetItem(transItem,"Transportlayer","cboxTrans""","QString textDescr");
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
