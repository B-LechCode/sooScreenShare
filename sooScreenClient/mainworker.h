//SooScreenServer by Simon Wezstein (B-LechCode), 2019
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
class mainWorker:private ItransportClientObserver
{
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
     * @param decompBackend Identifier of the image decompression backend
     * @param transportBackend Identifier of the transport client backend
     * @param ptrDraw Pointer to the draw widget
     */
    void init(std::string decompBackend, std::string transportBackend, Idraw* ptrDraw);
    /**
     * @brief The end.
     * Frees the ressources of the worker.
     */
    void end();   
    /**
     * @brief Getter for the image decompressor backend
     *
     * @return IImageDecompressor The backend
     */
    IImageDecompressor *decomp() const;
    /**
     * @brief Getter for the transport client backend
     *
     * @return ItransportClient The backend
     */
    ItransportClient *trans() const;

private:
    /**
     * @brief Sets a new decompression backend
     *
     * @param decompBackend Identifier for the backend
     */
    void setDecompressionBackend(std::string decompBackend);
    /**
     * @brief Sets a new transport client backend
     *
     * @param transportBackend Identifier for the backend
     */
    void setTransportBackend(std::string transportBackend);
    IImageDecompressor* m_decomp = nullptr; /**< Pointer to the current image decompressor backend */
    ItransportClient* m_trans = nullptr; /**< Pointer to the current transport client backend */
    Idraw*      m_ptrDraw = nullptr; /**< Pointer to the draw widget */
    std::vector<uint8_t> m_receiveBuffer; /**< The receive buffer */
    bool m_initOk; /**< The initialize ok flag */


    /**
     * @brief Handles new messages from the transport client
     *
     * @param str The message
     */
    virtual void transportNewMessage(std::string str)
    {
        std::cout << str << std::endl;
    }    
    /**
     * @brief Handles new available Data
     *
     * @param dat Pointer to the data (don't delete!)
     * @param len Data length
     */
    virtual void transportDataAvailable(const char *dat, int64_t len);


};

#endif // MAINWORKER_H
