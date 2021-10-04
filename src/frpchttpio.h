/*
 * FastRPC -- Fast RPC library compatible with XML-RPC
 * Copyright (C) 2005-7  Seznam.cz, a.s.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Seznam.cz, a.s.
 * Radlicka 2, Praha 5, 15000, Czech Republic
 * http://www.seznam.cz, mailto:fastrpc@firma.seznam.cz
 *
 * FILE             $Id: frpchttpio.h,v 1.4 2007-04-02 15:28:20 vasek Exp $
 *
 * DESCRIPTION      HTTP I/O
 *
 * AUTHOR           Vasek Blazek <blazek@firma.seznam.cz>
 *                  Miroslav Talasek <miroslav.talasek@firma.seznam.cz>
 *
 * HISTORY
 *          2005-02-07
 *                  First draft.
 *          2005-02-22
 *                  Transformed for FastRPC
 */

#ifndef FRPCHTTPIO_H
#define FRPCHTTPIO_H

#include <frpcplatform.h>

#include <string>
#include <vector>

//#include <frpchttpio.h>
#include <frpchttp.h>

namespace FRPC
{

// forward declaration
//class DataSource_t;
class DataSink_t;

class FRPC_DLLEXPORT HTTPIO_t
{
public:
    inline HTTPIO_t(int fd, int readTimeout, int writeTimeout,
                    int lineSizeLimit, int bodySizeLimit)
            : fd(fd), readTimeout(readTimeout), writeTimeout(writeTimeout),
            lineSizeLimit(lineSizeLimit), bodySizeLimit(bodySizeLimit)
    {}

    ~HTTPIO_t();

    static std::vector<std::string> splitBySpace(const std::string &line,
            size_t upto);

    /**
     * @short Return mame and value of item from http header.
     * 
     * @param line item of header
     * @param name name if item
     * @param  value value of item
     * @return 0 succes, !0 error
     */
    static int getHeaderValue(const std::string &line, std::string &name,
                              std::string &value);

    /**
     * @short Read line from socket.
     *
     * @param checkLimit limit check  
     * @return read line
     */
    std::string readLine(bool checkLimit = false);

    /**
     * @short Read line from socket.
     *
     * @param checkLimit limit check
     * @param optional don't throw exception if server closes connection before sending any line.
     * @return read line
     */
    std::string readLineOpt(bool checkLimit = false, bool optional=false);

    void readHeader(HTTPHeader_t &header);
    void readHeader(HTTPHeader_t &header, bool optional);

    long int readChunkSize();

    /**
     * @short Read entity from socket
     *
     * @param header header used to determine content
     * @param data output
     * @param request we are reading: true=request, false=response.
     */
    void readContent(HTTPHeader_t &header, DataSink_t &data,
                     bool request);

    /**
     * @short Read chunked content.
     *
     * @param header header filled by trailer
     * @param data output
     */
    void readChunkedContent(HTTPHeader_t &header, DataSink_t &data);


    /**
     * @short Read block from socket.
     *
     * @param contentLength Exact block size to read (-1=read until conn close).
     * @param data output
     */
    void readBlock(long int contentLength, DataSink_t &data);

    /**
     * @short Send data to socket.
     *
     * @param data pointer to data
     * @param watchForResponse says that sender receive too
     */
    inline void sendData(const std::string &data,
                         bool watchForResponse = false)
    {
        // send string's content
        return sendData(data.data(), data.length(), watchForResponse);
    }

    /** @short Send data to socket.
     *
     * @param data pointer to data
     * @param length  data length 
     * @param watchForResponse says that sender receive too
     */
    void sendData(const char *data, size_t length,
                  bool watchForResponse = false);
    /**
    *    @brief return reference to socket
    */
    inline int& socket()
    {
        return fd;
    }
    inline void setSocket(int fd)
    {
        this->fd = fd;
    }
    /**
     *    @brief set new read timeout
     */
    inline void setReadTimeout(int timeout)
    {
        readTimeout = timeout;
    }
    /**
     *    @brief set new write timeout
     */
    inline void setWriteTimeout(int timeout)
    {
        writeTimeout = timeout;
    }

private:
    int fd;
    int readTimeout;
    int writeTimeout;
    int lineSizeLimit;
    int bodySizeLimit;
};

} // namespace HTTPStorage

#endif // HSTRIO_H
