//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IPARAMETER_H
#define IPARAMETER_H
#include <string>
#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "parameter.h"

/**
 * @brief The parameter interface class
 * This class handles the parametrization, inherit this for parameter changed events.
 */
class Iparameter
{
protected:
    parameterMap m_defaultParameters; /**< Default parameter maps, fill this in your constructor. This lets the external parametrization instance know which parameters are used. */
    parameterMap m_parameters; /**< The current set parameters */
public:
    /**
     * @brief The standard constructor
     *
     */
    Iparameter();
    /**
     * @brief The destructor
     *
     */
    virtual ~Iparameter();
    /**
     * @brief Query the default parameters
     *
     * @return parameterMap The default parameter map
     */
    virtual parameterMap getDefaultParameters();
    /**
     * @brief Query the parameters
     *
     * @return parameterMap The parameter map
     */
    virtual parameterMap getParameters();
    /**
     * @brief Set a specific parameter
     *
     * @param key The parameter key
     * @param value The parameter value
     */
    virtual void setParameterValue(std::string key, std::string value);
    /**
     * @brief Set a whole parameter map
     * When setting a parameter map, existing parameters will be updated.
     * The non existing parameters will be added to the member parameter map.
     * The parameters existing in the member parameter map, but not existing in the parameter map to set will be kept.
     * @param para The parameter map to insert
     */
    virtual void setParameters(parameterMap& para);
    /**
     * @brief The changed event of a key/value pair
     *
     * @param key The key of the changed parameter
     */
    virtual void parameterMapChangedEvent() = 0;
    /**
     * @brief The new message event
     * Will be called when a new message is available
     * @param str The message
     */
    virtual void parameterChangedEvent(const std::string& key) = 0;

};

#endif // IPARAMETER_H
