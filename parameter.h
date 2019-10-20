//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef PARAMETER_H
#define PARAMETER_H

#include <map>
#include <string>


class parameter
{
private:
    std::string m_description;
    std::string m_type;
    std::string m_value;

public:
     parameter(){}
     parameter(std::string description,std::string type,std::string value)
     {
         m_description = description;
         m_type = type;
         m_value = value;
     }
     parameter(const parameter& p)
     {
         if(this != &p)
         {
             this->m_type = p.m_type;
             this->m_description = p.m_description;
             this->m_value = p.m_value;
         }
     }
     std::string description() const
     {
         return m_description;
     }

     std::string type() const
     {
         return m_type;
     }

     std::string value() const
     {
         return m_value;
     }

     void setValue(std::string val)
     {
         m_value = val;
     }


};

class parameterMap : public std::map<std::string,parameter>
{
public:
    static void insertParameters(parameterMap& member, parameterMap toInsert)
    {
        //insert into local para map
        auto itPara =  toInsert.begin();
        while(itPara != toInsert.end())
        {
            member[itPara->first] = itPara->second;
            itPara++;
        }
    }

    static parameterMap setParameterValue(parameterMap map,std::string key,std::string value)
    {
        map[key].setValue(value);
        return map;
    }
};

#endif // PARAMETER_H
