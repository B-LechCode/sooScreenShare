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

class Iparameter
{
protected:
    parameterMap m_defaultParameters;
    parameterMap m_parameters;
public:
    Iparameter();
    virtual ~Iparameter();
    virtual parameterMap getDefaultParameters();
    virtual parameterMap getParameters();
    virtual void setParameterValue(std::string key, std::string value);
    virtual void setParameters(parameterMap& para);
    virtual void parameterMapChangedEvent() = 0;
    virtual void parameterChangedEvent(const std::string& key) = 0;

};

#endif // IPARAMETER_H
