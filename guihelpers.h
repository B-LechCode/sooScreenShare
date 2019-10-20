#ifndef GUIHELPERS_H
#define GUIHELPERS_H

#define PARAMETER_PROPERTY "parameter"
#define SECTION_PROPERTY "sec"


#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QString>
#include "parameter.h"

class GuiHelpers
{
public:
    GuiHelpers();


    static QTreeWidgetItem* addToplevelWidgetItem(QTreeWidget* wid,QString text);

    static QTreeWidgetItem* addWidgetItem(QTreeWidgetItem* itm,QString text,QString text2,QString textDescr);

    static QTreeWidgetItem* addWidgetItem(QTreeWidget* wid,QTreeWidgetItem* itm,QString text,QWidget* cwid,QString textDescr);

    static size_t findIndex(std::vector<std::string>& vec,std::string str);
    static void addSettingsCat(QObject* window, QTreeWidget* tree, const std::vector<std::string> transBackends, parameterMap transParam, QString sectionName, int sec, int currentIndex);

};

#endif // GUIHELPERS_H
