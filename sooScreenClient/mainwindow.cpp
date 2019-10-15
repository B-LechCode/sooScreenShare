#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_draw = new drawWidget();
    m_draw->setObserver(this);
    m_draw->show();
    m_work.init(m_draw);    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawWidgetClosing()
{
    QApplication::exit(0);
}
