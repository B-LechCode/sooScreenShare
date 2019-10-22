#ifndef GUIHELPERS_H
#define GUIHELPERS_H

#define PARAMETER_PROPERTY "parameter"
#define SECTION_PROPERTY "sec"


#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QString>
#include "parameter.h"

/**
 * @brief The GUI helper class
 * This class is used to share common GUI methods for client and server
 */
class GuiHelpers
{
private:
    /**
     * @brief The standard constructor
     *
     */
    GuiHelpers();
public:



    /**
     * @brief Adds the top level text of an new section in the tree view
     *
     * @param wid The tree widget pointer
     * @param text The text to set
     * @return QTreeWidgetItem the new created item
     */
    static QTreeWidgetItem* addToplevelWidgetItem(QTreeWidget* wid,QString text);

    /**
     * @brief Adds a new text only widget item to a existing section
     *
     * @param itm The section item
     * @param text The text for the first column
     * @param text2 The text for the second column
     * @param textDescr The text for the third column
     * @return QTreeWidgetItem the new created item
     */
    static QTreeWidgetItem* addWidgetItem(QTreeWidgetItem* itm,QString text,QString text2,QString textDescr);

    /**
     * @brief Adds a new widget item, containing an existing widet
     *
     * @param wid The tree widget
     * @param itm The section item
     * @param text The text for the first column
     * @param cwid The widget to add
     * @param textDescr The text for the third column
     * @return QTreeWidgetItem the new created item
     */
    static QTreeWidgetItem* addWidgetItem(QTreeWidget* wid,QTreeWidgetItem* itm,QString text,QWidget* cwid,QString textDescr);

    /**
     * @brief
     *
     * @param vec
     * @param str
     * @return size_t
     */
    static size_t findIndex(std::vector<std::string>& vec,std::string str);
    /**
     * @brief Add a new section to the tree view
     *
     * @param window The window pointer
     * @param tree The tree view pointer
     * @param transBackends The vector of available backends
     * @param transParam The paramater map of the actual selected backend
     * @param sectionName The name of the section
     * @param sec The enumeration of the section
     * @param currentIndex The currend selected index in transBackends
     */
    static void addSettingsCat(QObject* window, QTreeWidget* tree, const std::vector<std::string> transBackends, parameterMap transParam, QString sectionName, int sec, int currentIndex);

};

#endif // GUIHELPERS_H
