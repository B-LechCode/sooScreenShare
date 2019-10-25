//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef HEADER_H
#define HEADER_H
#include <cstdint>

#define HEADER_SIZE sizeof(dataHeaderHandling::dataHeader)
#define HEADER_STRING "ScreenImage"

/**
 * @brief Helper class for the handling of the data header
 *
 */
class dataHeaderHandling
{
private:
    /**
     * @brief The standard constructor
     *
     */
    //dataHeaderHandling();
public:
    /**
     * @brief The standard data header structure
     *
     */
    typedef struct dataHeader {
       char headerString[16] = HEADER_STRING; /**< The standard identifier  */ //16
       char reserved[16];     /**< Some reserved bytes for future use */ //32
       int32_t  length;          /**< Payload length */ //36
       uint32_t  width;           /**< The image width */ //40
       uint32_t  height;          /**< The image height */ //44
       int32_t  cvType;          /**< The open cv type of the transmitted image */ //48
       char reserved2[16];   /**< Some reserved bytes for future use */ //64
    /**
     * @brief The standard data header structure
     *
     */
    } dHdr;
};

#endif
