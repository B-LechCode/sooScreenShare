//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef PARAMETERSERIALIZATION_H
#define PARAMETERSERIALIZATION_H
#include <string>
#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "parameter.h"

/**
 * @brief The parameter serialization class based on QJson
 * The class implements project specific parameter serialization based on the json standard
 */
class parameterSerialization
{
private:
    QJsonObject m_object; /**< The json object to work on */
public:

    /**
     * @brief Appends a backend selection to the config file
     * This appends the current selection of a specific backend role/purpose.
     * For instance the currently selected backend for the purpose of the transport server.
     * @param backendPurpose The backend purpose (e.g. the transport server)
     * @param backendSelectionName The name of the selected backend for this usage/purpose
     */
    void appendBackendSelection(std::string backendPurpose,std::string backendSelectionName);

    /**
     * @brief Appends a parameter map linked to a specified backend
     * This allows the storage of parameters for all existing backends, not only for those currently selected.
     * @param map The parameter map
     * @param backendName The backend name linked with this settings (e.g. tcp server)
     */
    void appendParameterMap(parameterMap map,std::string backendName);

    /**
     * @brief Getter method for a simple string key
     * This method returns the string associated with the key.
     * In this project, the method is used to extract the selected backend for the available backend purposes.
     * @param key The key to look up
     * @return std::string The value
     */
    std::string getStringValue(std::string key);

    /**
     * @brief Getter method for a whole parameter map
     * This method returns the parameter map associated with the key.
     * In this project, the method is used to extract the parameter map which is stored for a specified backend name.
     * @param key The key to look up
     * @return parameterMap The parameter map (empty if not available)
     */
    parameterMap getParameterMap(std::string key);

    /**
     * @brief Reads data from json file to the internal object
     * Starts reading a json file from text
     * @param filename Path to the file
     * @return bool Read status (true if ok)
     */
    bool readData(std::string filename);

    /**
     * @brief Writes internal object data to file
     * Starts writing the internal object to the specified textfile.
     * Existing data will be overwritten.
     * @param filename Path to the file
     * @return bool Write status (true if ok)
     */
    bool writeData(std::string filename);
};

#endif // PARAMETERSERIALIZATION_H
