#include "guihelpers.h"
#include <QComboBox>
#include <QLineEdit>


GuiHelpers::GuiHelpers()
{

}

QTreeWidgetItem* GuiHelpers::addToplevelWidgetItem(QTreeWidget* wid,QString text)
{
    QTreeWidgetItem* itm = new QTreeWidgetItem(wid);
    itm->setText(0,text);
    return itm;
}

QTreeWidgetItem* GuiHelpers::addWidgetItem(QTreeWidgetItem* itm,QString text,QString text2,QString textDescr)
{
    QTreeWidgetItem* itm2 = new QTreeWidgetItem(itm);
    itm2->setText(0,text);
    itm2->setText(1,text2);
    itm2->setText(2,textDescr);


    return itm2;
}

QTreeWidgetItem* GuiHelpers::addWidgetItem(QTreeWidget* wid,QTreeWidgetItem* itm,QString text,QWidget* cwid,QString textDescr)
{
    QTreeWidgetItem* itm2 = new QTreeWidgetItem(itm);
    itm2->setText(0,text);
    wid->setItemWidget(itm2,1,cwid);
    itm2->setText(2,textDescr);

    return itm2;
}

size_t GuiHelpers::findIndex(std::vector<std::string>& vec,std::string str)
{
    for (size_t i = 0; i<vec.size();++i)
    {
       if(vec[i] == str)
           return i;
    }
    return 0;
}

void GuiHelpers::addSettingsCat(QObject* window,QTreeWidget* tree,const std::vector<std::string> transBackends,parameterMap transParam,QString sectionName,int sec,int currentIndex)
{
    QTreeWidgetItem* itmBackendSection = GuiHelpers::addToplevelWidgetItem(tree,sectionName);

    //Backend selector
    QComboBox* cboxTrans = new QComboBox();

    for(uint32_t i = 0;i<transBackends.size();++i)
        cboxTrans->addItem(QString(transBackends[i].c_str()));

    cboxTrans->setProperty(SECTION_PROPERTY,QVariant(sec));
    cboxTrans->setCurrentIndex(currentIndex);

    QObject::connect(cboxTrans,SIGNAL(currentIndexChanged(int)),window,SLOT(on_qComboBoxCurrentIndexChanged(int)));

    QTreeWidgetItem* transItem = GuiHelpers::addWidgetItem(tree,itmBackendSection,sectionName,cboxTrans,"");


    for(auto i = transParam.begin();i!=transParam.end();++i)
    {
        QLineEdit* ledit = new QLineEdit();
        ledit->setText((*i).second.value().c_str());

        ledit->setProperty(PARAMETER_PROPERTY,QVariant(QString((*i).first.c_str())));
        ledit->setProperty(SECTION_PROPERTY,QVariant(sec));

        GuiHelpers::addWidgetItem(tree,transItem,QString((*i).first.c_str()),ledit,QString((*i).second.description().c_str()));
        QObject::connect(ledit,SIGNAL(editingFinished()),window,SLOT(on_qLineEditEditingFinished()));
    }
}
