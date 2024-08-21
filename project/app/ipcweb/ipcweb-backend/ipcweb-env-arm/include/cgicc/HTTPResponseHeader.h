/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTTPResponseHeader.h,v 1.10 2014/04/23 20:55:09 sebdiaz Exp $
 *
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *  Part of the GNU cgicc library, http://www.gnu.org/software/cgicc
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA 
 */

#ifndef _HTTPRESPONSEHEADER_H_
#define _HTTPRESPONSEHEADER_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTTPResponseHeader.h
 * \brief Class for generic, complete HTTP header responses
 * 
 * This is an class usually only used with Non-Parsed Header (NPH)
 * applications
 */

#include <string>
#include <vector>

#include "MStreamable.h"
#include "HTTPCookie.h"

namespace cgicc {

  // ============================================================
  // Class HTTPResponseHeader
  // ============================================================
  /*! \class HTTPResponseHeader HTTPResponseHeader.h cgicc/HTTPResponseHeader.h
   * \brief Generic HTTP response header
   *
   * This class represents an HTTP response header as defined in
   * section 6 of RFC 2616 (see http://www.w3.org)
   *
   * All HTTP/1.1 reponses consist of an initial status line containing
   * the HTTP version, a 3-digit status code, and a human-readable reason
   * phrase explaining the status code.
   *
   * The first digit of the Status-Code defines the class of
   * response. The last two digits do not have any categorization
   * role. There are 5 values for the first digit:
   * <ul>
   * <li>1xx: Informational - Request received, continuing process</li>
   * <li>2xx: Success - The action was successfully received,
   understood, and accepted</li>
   * <li>3xx: Redirection - Further action must be taken in order to
   * complete the request</li>
   * <li>4xx: Client Error - The request contains bad syntax or cannot
   * be fulfilled</li>
   * <li>5xx: Server Error - The server failed to fulfill an apparently
   * valid request</li></ul>
   */
  class CGICC_API HTTPResponseHeader : public MStreamable
  {
  public:
    
    /*! \name Constructor and Destructor */
    //@{
    
    /*!
     * \brief Create a new HTTP response header
     * \param http_version The HTTP version string, usually \c HTTP/1.1
     * \param status_code The 3-digit HTTP status code
     * \param reason_phrase A short textual description of the status code
     */
    HTTPResponseHeader(const std::string& http_version,
		       int status_code,
		       const std::string& reason_phrase);
    
    /*!
     * \brief Delete this HTTPResponseHeader
     * 
     */
    virtual ~HTTPResponseHeader();
    //@}
    
    // ============================================================
    
    /*! \name Additional Header Management */
    //@{
    
    /*!
     * \brief Add a general, response, or entity header to this one
     * 
     * \param header The text of the header to add
     * \return A reference to this
     */
    HTTPResponseHeader&
    addHeader(const std::string& header);
    
    /*!
     * \brief Add a general, response, or entity header to this one
     * 
     * \param name The name of the header element to add
     * \param value The value of the header element
     * \return A reference to this
     */
    HTTPResponseHeader&
    addHeader(const std::string& name,
	      const std::string& value);
    
    /*!
     * \brief Get a list of all additional headers
     *
     * \return A list of all additional headers
     */
    inline const std::vector<std::string>&
    getHeaders() 					const
    { return fHeaders; }
    //@}
    
    /*! \name Cookie Management */
    //@{
    
    /*!
     * \brief Set a cookie to go out with this HTTPResponseHeader
     * \param cookie The HTTPCookie to set
     */
    HTTPResponseHeader&
    setCookie(const HTTPCookie& cookie);
    
    /*!
     * \brief Get a list of all cookies associated with this header
     * \return All the cookies associated with this header
     */
    inline const std::vector<HTTPCookie>&
    getCookies() 					const
    { return fCookies; }
    //@}
    
    // ============================================================
    
    /*! \name Accessor methods 
     * Retrieve information on the header
     */
    //@{
    
    /*!
     * \brief Get the HTTP version
     *
     * The HTTP version is a string of the form \c HTTP/1.1
     * \return The HTTP version
     */
    inline const std::string&
    getHTTPVersion() 				const
    { return fHTTPVersion; }
    
    /*!
     * \brief Get the 3-digit status code
     *
     * The 3-digit status code indicates the disposition of the response.
     * \return The 3-digit status code
     */
    inline int
    getStatusCode() 				const
    { return fStatusCode; }
    
    /*!
     * \brief Get the reason phrase associated with the stats code
     *
     * The reason phrase is a human-readable interpretation of the status code
     * \return The reason phrase
     */
    inline std::string
    getReasonPhrase() 				const
    { return fReasonPhrase; }
    //@}
    
    // ============================================================
    
    /*! \name Mutator methods 
     * Set information on the header
     */
    //@{
    
    /*!
     * \brief Set the HTTP version
     *
     * The HTTP version is a string of the form \c HTTP/1.1
     * \param http_version The HTTP version string, usually \c HTTP/1.1
     * \return A reference to this
     */
    inline HTTPResponseHeader&
    getHTTPVersion(const std::string& http_version)
    { fHTTPVersion = http_version; return *this; }
    
    /*!
     * \brief Get the 3-digit status code
     *
     * The 3-digit status code indicates the disposition of the response.
     * \param status_code The 3-digit HTTP status code
     * \return A reference to this
     */
    inline HTTPResponseHeader&
    getStatusCode(int status_code)
    { fStatusCode = status_code; return *this; }
    
    /*!
     * \brief Get the reason phrase associated with the stats code
     *
     * The reason phrase is a human-readable interpretation of the status code
     * \param reason_phrase A short textual description of the status code
     * \return A reference to this
     */
    inline HTTPResponseHeader&
    getReasonPhrase(const std::string& reason_phrase)
    { fReasonPhrase = reason_phrase; return *this; }
    //@}
    
    // ============================================================
    
    /*! \name Inherited Methods */
    //@{
    virtual void 
    render(std::ostream& out) 			const;
    //@}
    
  private:
    HTTPResponseHeader();

    std::string 		fHTTPVersion;
    int 			fStatusCode;
    std::string 		fReasonPhrase;
    std::vector<std::string> 	fHeaders;
    std::vector<HTTPCookie> 	fCookies;
  };
  
} // namespace cgicc

#endif /* ! _HTTPRESPONSEHEADER_H_ */
