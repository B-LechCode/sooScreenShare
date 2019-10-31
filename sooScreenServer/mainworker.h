//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "interfaces.h"
#include <iostream>

/**
 * @brief The main worker class
 * This class handles the interaction between the worker backends as main controll class
 */
class mainWorker:private ItransportServerObserver
{    
    const uint32_t w=1920; /**< Default width for default buffer size */
    const uint32_t h=1080; /**< Default height for default buffer size */
public:
    /**
     * @brief The default constructor
     *
     */
    mainWorker();
    /**
     * @brief The destructor
     *
     */
    virtual ~mainWorker();
    /**
     * @brief (Re)Initializes the worker
     *
     * @param screenShotBackend Identifier of the screenshot backend
     * @param imageCompressorBackend Identifier of the image compression backend
     * @param transportServerBackend Identifier of the transport server backend
     */
    void init(std::string screenShotBackend,std::string imageCompressorBackend, std::string transportServerBackend);
    /**
     * @brief Runs one work cycle
     * Grabs one image, compresses and sends the image.
     */
    void run();
    /**
     * @brief The end.
     * Frees the ressources of the worker.
     */
    void end();
    /**
     * @brief Getter for the screenshot backend
     *
     * @return IscreenShot The backend
     */
    IscreenShot *screen() const;
    /**
     * @brief Getter for the image compressor backend
     *
     * @return IImageCompressor The backend
     */
    IImageCompressor *comp() const;
    /**
     * @brief Getter for the transport server backend
     *
     * @return ItransportServer The backend
     */
    ItransportServer *trans() const;

private:
    IscreenShot* m_screen = nullptr; /**< Pointer to the current screenshot backend */
    IImageCompressor* m_comp = nullptr; /**< Pointer to the current image compressor backend */
    ItransportServer* m_trans = nullptr; /**< Pointer to the current transport server backend */
    uint8_t*    m_sendbuffer = nullptr; /**< The send buffer */
    uint64_t         m_bufferSize; /**< The current send buffer size */

    /**
     * @brief Creates the data header
     *
     */
    void createHeader();
    /**
     * @brief Inserts the header info for the current frame
     *
     * @param byteCount The byte count of the compressed data
     * @param width Width of the Image
     * @param height Height of the Image
     * @param cvType The opencv data type
     */
    void insertHeaderInfo(int byteCount,int width,int height,int cvType);
    /**
     * @brief Handles new messages from the transport server
     *
     * @param str The message
     */
    virtual void transportNewMessage(std::string str)
    {
        std::cout << str << std::endl;
    }

};

#endif // MAINWORKER_H
