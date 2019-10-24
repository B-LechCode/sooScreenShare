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
       char headerString[16] = HEADER_STRING; //16 /**< The standard identifier  */
       char reserved[16];     //32 /**< Some reserved bytes for future use */
       int32_t  length;           //36 /**< Payload length */
       uint32_t  width;            //40 /**< The image width */
       uint32_t  height;           //44 /**< The image height */
       int32_t  cvType;           //48 /**< The open cv type of the transmitted image */
       char reserved2[16];    //64 /**< Some reserved bytes for future use */
    /**
     * @brief The standard data header structure
     *
     */
    } dHdr;
};

#endif
