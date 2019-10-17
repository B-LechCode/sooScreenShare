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

class parameterSerialization
{
private:
    QJsonObject m_object;
public:

    void appendBackendSelection(std::string backendName,std::string backendSelection)
    {
        m_object.insert(QString(backendName.c_str()),QString(backendSelection.c_str()));
    }

    void appendParameterMap(parameterMap map,std::string backendName)
    {
        QJsonObject  recordObject;

        //m_object.insert("backendName",QString(backendName.c_str()));

        std::map<std::string, parameter>::const_iterator i;
        for (i = map.begin(); i != map.end(); ++i)
        {
            parameter p = (*i).second;
            QJsonObject paraObject;
            paraObject.insert("value",QJsonValue::fromVariant(QString(p.value().c_str())));
            paraObject.insert("description",QJsonValue::fromVariant(QString(p.description().c_str())));
            paraObject.insert("type",QJsonValue::fromVariant(QString(p.type().c_str())));
            recordObject.insert(QString((*i).first.c_str()), paraObject);
        }

        m_object.insert(QString(backendName.c_str()),recordObject);
        /*

        QString o = QString::fromStdString(doc.toJson().toStdString());*/
    }

    std::string getStringValue(std::string key)
    {
        return m_object.value(key.c_str()).toString().toStdString();
    }

    parameterMap getParameterMap(std::string key)
    {
        QJsonObject t =  m_object.value(key.c_str()).toObject();

        parameterMap map;

        QStringList k = t.keys();

        for (int i = 0;i<k.length();++i)
        {
            QJsonObject paraObject = t.value(k[i]).toObject();

            map[k[i].toStdString()] = parameter(paraObject.value("description").toString().toStdString(),
                                                paraObject.value("type").toString().toStdString(),
                                                paraObject.value("value").toString().toStdString());
        }

        return map;
    }

    bool readData(std::string filename)
    {
        bool ok;
        QFile file(QString(filename.c_str()));
        ok = file.open(QFile::ReadOnly| QIODevice::Text);

        if(ok)
        {
            QJsonDocument doc;
            QByteArray t = file.readAll();
            file.close();
            QJsonParseError err;
            doc = QJsonDocument::fromJson(t,&err);
            ok = err.error == QJsonParseError::NoError;
            m_object = doc.object();
        }

        return ok;
    }

    bool writeData(std::string filename)
    {
        bool ok;
        QFile file(QString(filename.c_str()));
        ok = file.open(QFile::WriteOnly| QIODevice::Text);
        if(ok)
        {
            QJsonDocument doc(m_object);
            file.write(doc.toJson());
        }
        file.close();
        return ok;
    }
};

#endif // PARAMETERSERIALIZATION_H
