//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "iparameter.h"

#include <iostream>
Iparameter::Iparameter()
{

}

Iparameter::~Iparameter()
{

}

parameterMap Iparameter::getDefaultParameters()
{
    return m_defaultParameters;
}

parameterMap Iparameter::getParameters()
{
    return m_parameters;
}

void Iparameter::setParameterValue(std::string key, std::string value)
{
    bool found = m_parameters.find(key) != m_parameters.end(); //check for existence!
    bool equal = false;
    if(found)
    {
        equal = m_parameters[key].value() == value;
        std::cout << m_parameters[key].value() << std::endl;
        std::cout << value << std::endl;
    }

    m_parameters[key].setValue(value);

    if(!equal)
        parameterChangedEvent(key);

}

void Iparameter::setParameters(parameterMap &para)
{
    parameterMap::insertParameters(m_parameters,para);
    parameterMapChangedEvent();  
}

void Iparameter::parameterMapChangedEvent()
{

}
