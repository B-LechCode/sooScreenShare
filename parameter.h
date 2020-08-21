//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef PARAMETER_H
#define PARAMETER_H

#include <map>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cctype>

/**
 * @brief The parameter class
 * This class encapsulates value, description and type of a single parameter.
 */
class parameter
{
private:
    std::string m_description; /**< The descripton */
    std::string m_type; /**< The type */
    std::string m_value; /**< The value */

public:
     /**
      * @brief The standard constructor
      *
      */
     parameter(){}
     /**
      * @brief The value constructor
      * This constructor creates a specific parameter
      * @param description The description to set
      * @param type The type to set
      * @param value The value to set
      */
     parameter(std::string description,std::string type,std::string value)
     {
         m_description = description;
         m_type = type;
         m_value = value;
     }
     /**
      * @brief The copy constructor
      *
      * @param p The parameter to copy
      */
     parameter(const parameter& p)
     {
         if(this != &p)
         {
             this->m_type = p.m_type;
             this->m_description = p.m_description;
             this->m_value = p.m_value;
         }
     }

     /**
      * @brief The assign operator
      *
      * @param p The parameter to assign
      * @return const parameter &operator Reference to the object
      */
     const parameter & operator=(const parameter& p)
     {
         if(this != &p)
         {
             this->m_type = p.m_type;
             this->m_description = p.m_description;
             this->m_value = p.m_value;
         }
         return  *this;
     }
     /**
      * @brief Getter method for the description
      *
      * @return std::string The description
      */
     std::string description() const
     {
         return m_description;
     }

     /**
      * @brief Getter method for the type
      *
      * @return std::string The type
      */
     std::string type() const
     {
         return m_type;
     }

     /**
      * @brief Getter method for the value
      *
      * @return std::string The value
      */
     std::string value() const
     {
         return m_value;
     }

     /**
      * @brief Getter method for the value as integer
      * @param  ok The conversion status
      * @return int The value
      */
     int valueInt(bool &ok)
     {
         int ret = -1;

         try {
             ret = std::stoi(value());
             ok = true;
         } catch (std::invalid_argument ) {
             ok = false;
         }
         return ret;
     }


     /**
      * @brief Getter method for the value as bool

      * @return bool The value
      */
     bool valueBool()
     {
         bool ret = false;
         std::string overrideStr = value();
         std::transform(overrideStr.begin(),overrideStr.end(),overrideStr.begin(),[](int c) -> int { return std::tolower(c); });
         ret = overrideStr == "true";
         return ret;
     }

     /**
      * @brief Setter method for the value
      *
      * @param val The new value
      */
     void setValue(std::string val)
     {
         m_value = val;
     }


};

/**
 * @brief The parameter map class
 * This class implements a std::map with some extensions for helping to deal with parameters
 */
class parameterMap : public std::map<std::string,parameter>
{
public:
    /**
     * @brief Static function to merge parameter maps
     *
     * @param basicMap The map to insert the parameters in
     * @param toInsert The map to insert
     */
    static void insertParameters(parameterMap& basicMap, parameterMap toInsert)
    {
        //insert into local para map
        auto itPara =  toInsert.begin();
        while(itPara != toInsert.end())
        {
            basicMap[itPara->first] = itPara->second;
            itPara++;
        }
    }

    /**
     * @brief Static function to add/change a parameter to a parameter map
     *
     * @param map The parameter map to work on
     * @param key The key of the parameter
     * @param value The value of the parameter
     * @return parameterMap The changed parameter map
     */
    static parameterMap setParameterValue(parameterMap map,std::string key,std::string value)
    {
        map[key].setValue(value);
        return map;
    }
};

#endif // PARAMETER_H
