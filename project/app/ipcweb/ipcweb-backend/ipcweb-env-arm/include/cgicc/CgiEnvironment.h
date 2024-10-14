/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: CgiEnvironment.h,v 1.21 2014/04/23 20:55:03 sebdiaz Exp $
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

#ifndef _CGIENVIRONMENT_H_
#define _CGIENVIRONMENT_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file CgiEnvironment.h
 * \brief Class encapsulating the CGI runtime environment
 *
 * The \c CgiEnvironment class encapsulates the  environment of 
 * the CGI application as described by the HTTP server.  \c CgiEnvironment
 * contains the \c GET or \c POST data along with all environment variables 
 * set by the HTTP server specified in the CGI specification.
 */

#include <string>
#include <vector>
#include <cstdlib>

#include "CgiDefs.h"
#include "CgiUtils.h"
#include "CgiInput.h"
#include "HTTPCookie.h"

namespace cgicc {

#ifdef WIN32
  template class CGICC_API std::vector<HTTPCookie>;
#endif
  
  // ============================================================
  // Iterator typedefs
  // ============================================================
  
  //! A vector of HTTPCookie objects
  typedef std::vector<HTTPCookie>::iterator 	cookie_iterator;
  //! A vector of \c const HTTPCookie objects
  typedef std::vector<HTTPCookie>::const_iterator const_cookie_iterator;
  
  // ============================================================
  // Class CgiEnvironment
  // ============================================================
  
  /*! \class CgiEnvironment CgiEnvironment.h cgicc/CgiEnvironment.h 
   * \brief Class encapsulating the CGI runtime environment
   *
   * The \c CgiEnvironment class encapsulates the  environment of 
   * the CGI application as described by the HTTP server.  \c CgiEnvironment
   * contains the \c GET or \c POST data along with all environment variables 
   * set by the HTTP server specified in the CGI specification.
   */
  class CGICC_API CgiEnvironment
  {
  public:
    
    friend class Cgicc;
    
    // ============================================================
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*! 
     * \brief Read in the CGI environment passed to the CGI application
     * by the server
     *
     * This function is not usually called directly; instead, an object of type
     * CgiEnvironment is retrieved by calling the \c getEnvironment() method
     * on Cgicc.
     * If you are using %cgicc with FastCGI, you will need to pass 
     * a \c CgiInput subclass that %cgicc will use to read input.  If
     * \c input is omitted, standard input and environment
     * variables will be used.
     * \param input A CgiInput object to use for reading input
     * \see Cgicc::getEnvironment
     */
    CgiEnvironment(CgiInput *input);
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the values of this CgiEnvironment to those of \c env.
     * \param env The CgiEnvironment to copy.
     */
    inline
    CgiEnvironment(const CgiEnvironment& env)
    { operator=(env); }

    /*!
     * \brief Destructor 
     *
     * Delete this CgiEnvironment object
     */
    ~CgiEnvironment();
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Compare two CgiEnvironments for equality.
     *
     * CgiEnvironments are equal if they have the same environment variables.
     * \param env The CgiEnvironment to compare to this one.
     * \return \c true if the two CgiEnvironments are equal, \c false otherwise.
     */
    bool 
    operator== (const CgiEnvironment& env) 		const;
    
    /*!
     * \brief Compare two CgiEnvironments for inequality.
     *
     * CgiEnvironments are equal if they have the same environment variables.
     * \param env The CgiEnvironment to compare to this one.
     * \return \c false if the two CgiEnvironments are equal, \c true otherwise.
     */
    inline bool
    operator!= (const CgiEnvironment& env) 		const
    { return ! operator==(env); }
    
#ifdef WIN32
    /* Dummy operator for MSVC++ */
    inline bool
    operator< (const CgiEnvironment& env) 		const
    { return false; }
#endif
    
    /*!
     * \brief Assign one CgiEnvironment to another.  
     *
     * Sets the environment variables in this CgiEnvironment to those of \c env.
     * \param env The CgiEnvironment to copy.
     * \return A reference to this.
     */
    CgiEnvironment& 
    operator= (const CgiEnvironment& env);
    //@}

    // ============================================================
    
    /*! \name Server Information
     * Information on the server handling the HTTP/CGI request
     */
    //@{
    
    /*!
     * \brief Get the name and version of the HTTP server software
     *
     * For example, \c Apache/1.3.4
     * \return The name of the server software
     */
    inline std::string 
    getServerSoftware() 			const
    { return fServerSoftware; }
    
    /*!
     * \brief Get the hostname, DNS name or IP address of the HTTP server
     *
     * This is \e not a URL, for example \c www.gnu.org (no leading http://)
     * \return The name of the server
     */
    inline std::string 
    getServerName() 				const
    { return fServerName; }
    
    /*!
     * \brief Get the name and version of the gateway interface.
     *
     * This is usually \c CGI/1.1
     * \return The name and version of the gateway interface
     */
    inline std::string 
    getGatewayInterface() 			const
    { return fGatewayInterface;}
    
    /*!
     * \brief Get the name and revision of the protocol used for this request.
     *
     * This is usually \c HTTP/1.0 or \c HTTP/1.1
     * \return The protocol in use
     */
    inline std::string 
    getServerProtocol() 			const
    { return fServerProtocol; }
    
    /*!
     * \brief Get the port number on the server to which this request was sent.
     *
     * This will usually be 80.
     * \return The port number
     */
    inline unsigned long
    getServerPort() 				const
    { return fServerPort; }
    
    /*!
     * \brief Determine if this is a secure request
     * 
     * A secure request is usually made using SSL via HTTPS
     * \return \c true if this connection is via https, \c false otherwise
     */
    inline bool
    usingHTTPS() 				const
    { return fUsingHTTPS; }
    //@}
    
    // ============================================================
    
    /*! \name CGI Query Information
     * Information specific to this CGI query
     */
    //@{
    
    /*!
     * \brief Get the HTTP cookies associated with this query, if any.  
     *
     * The string returned by this method may contain multiple cookies; it is
     * recommended to use the method getCookieList() instead, which returns
     * a \c vector<HTTPCookie>.
     * \return The HTTP cookies
     * \see getCookieList
     */
    inline std::string 
    getCookies() 				const
    { return fCookie; }
    
    /*!
     * \brief Get a \c vector containing the HTTP cookies 
     * associated with this query
     *
     * This vector may be empty
     * \return A \c vector containing the HTTP cookies associated with this
     *  query
     * \see HTTPCookie
     */
    inline const std::vector<HTTPCookie>& 
    getCookieList() 				const
    { return fCookies; }
    
    /*!
     * \brief Get the request method used for this query.
     *
     * This is usually one of \c GET or \c POST
     * \return The request method
     */
    inline std::string 
    getRequestMethod()   			const
    { return fRequestMethod; }
    
    /*!
     * \brief Get the extra path information for this request, given by the 
     * client.
     *
     * For example, in the string \c foo.cgi/cgicc the path information is
     * \c cgicc.
     * \return The absolute path info
     */
    inline std::string
    getPathInfo() 				const
    { return fPathInfo; }
    
    /*!
     * \brief Get the translated path information (virtual to physical mapping).
     *
     * For example, \c www.gnu.org may be translated to \c /htdocs/index.html
     * \return The translated path info
     */
    inline std::string 
    getPathTranslated() 			const
    { return fPathTranslated; }
    
    /*!
     * \brief Get the full path to this CGI application
     *
     * This is useful for self-referencing URIs
     * \return The full path of this application
     */
    inline std::string 
    getScriptName() 				const
    { return fScriptName; }
    
    /*!
     * \brief Get the query string for this request.
     *
     * The query string follows the <tt>?</tt> in the URI which called this
     * application. This is usually only valid for scripts called with 
     * the \c GET method. For example, in the string \c foo.cgi?cgicc=yes 
     * the query string is \c cgicc=yes.
     * @return The query string
     */
    inline std::string 
    getQueryString()  				const
    { return fQueryString; }
    
    /*!
     * \brief Get the length of the data read from standard input, in chars.
     *
     * This is usually only valid for scripts called with the POST method.
     * \return The data length
     */
    inline unsigned long
    getContentLength() 				const
    { return fContentLength; }
    
    /*!
     * \brief Get the content type of the submitted information.
     *
     * For applications called via the GET method, this information is
     * irrelevant.  For applications called with the POST method, this is
     * specifies the MIME type of the information, 
     * usually \c application/x-www-form-urlencoded or as specified by
     * getContentType().
     * \return The content type
     * \see getContentType
     */
    inline std::string 
    getContentType() 				const
    { return fContentType; }
    
    /*!
     * \brief Get the data passed to the CGI application via standard input.
     *
     * This data is of MIME type \c getContentType().
     * \return The post data.
     */
    inline std::string
    getPostData() 				const
    { return fPostData; }
    //@}
    
    // ============================================================
    
    /*! \name Server Specific Information
     * Information dependent on the type of HTTP server in use
     */
    //@{
    
    /*!
     * \brief Get the URL of the page which called this CGI application.
     *
     * Depending on the HTTP server software, this value may not be set.
     * \return The URI which called this application.
     */
    inline std::string 
    getReferrer() 				const
    { return fReferrer; }
    //@}
    
    // ============================================================
    
    /*! \name Remote User Information
     * Information about the user making the CGI request
     */
    //@{
    
    /*!
     * \brief Get the hostname of the remote machine making this request
     *
     * This may be either an IP address or a hostname
     * \return The remote host
     */
    inline std::string 
    getRemoteHost() 				const
    { return fRemoteHost; }
    
    /*!
     * \brief Get the IP address of the remote machine making this request
     *
     * This is a standard IP address of the form \c 123.123.123.123
     * \return The remote IP address
     */
    inline std::string 
    getRemoteAddr() 				const
    { return fRemoteAddr; }
    
    /*!
     * \brief Get the protocol-specific user authentication method used.
     *
     * This is only applicable if the server supports user authentication,
     * and the user has authenticated.
     * \return The authorization type
     */
    inline std::string 
    getAuthType() 				const
    { return fAuthType; }
    
    /*!
     * \brief Get the authenticated remote user name.
     *
     * This is only applicable if the server supports user authentication,
     * and the user has authenticated.
     * \return The remote username 
     */
    inline std::string 
    getRemoteUser() 				const
    { return fRemoteUser; }
    
    /*!
     * \brief Get the remote user name retrieved from the server.
     *
     * This is only applicable if the server supports RFC 931 
     * identification.  This variable should \e only be used
     * for logging purposes.
     * \return The remote identification
     * \see RFC 1431 at 
     * http://info.internet.isi.edu:80/in-notes/rfc/files/rfc1413.txt
     */
    inline std::string 
    getRemoteIdent()    			const
    { return fRemoteIdent; }
    
    /*!
     * \brief Get the MIME data types accepted by the client's browser.
     *
     * For example <TT>image/gif, image/x-xbitmap, image/jpeg, image/pjpeg</TT>
     * \return The accepted data types
     */
    inline std::string 
    getAccept() 				const
    { return fAccept; }
    
    /*!
     * \brief Get the name of the browser used for this CGI request.
     *
     * For example <TT>Mozilla/5.0 (X11; U; Linux 2.4.0 i686; en-US; 0.8.1) 
     * Gecko/20010421</TT>
     * \return The browser name
     */
    inline std::string 
    getUserAgent() 				const
    { return fUserAgent; }
    //@}
    
    // ============================================================
    
    /*! \name ErrorDocument Handling
     * For a tutorial on ErrorDocument handling, see 
     * http://hoohoo.ncsa.uiuc.edu/cgi/ErrorCGI.html
     */
    //@{
    
    /*!
     * \brief Get the redirect request.
     *
     * This will only be valid if you are using this script as a script
     * to use in place of the default server messages.
     * @return The redirect request.
     */
    inline std::string 
    getRedirectRequest() 			const
    { return fRedirectRequest; }
    
    /*!
     * \brief Get the redirect URL.
     *
     * This will only be valid if you are using this script as a script
     * to use in place of the default server messages.
     * \return The redirect URL.
     * \see http://hoohoo.ncsa.uiuc.edu/docs/setup/srm/ErrorDocument.html
     */
    inline std::string 
    getRedirectURL() 				const
    { return fRedirectURL; }
    
    /*!
     * \brief Get the redirect status.
     *
     * This will only be valid if you are using this script as a script
     * to use in place of the default server messages.
     * \return The redirect status.
     */
    inline std::string 
    getRedirectStatus() 			const
    { return fRedirectStatus; }
    //@}
    
  protected:
    
    // ============================================================
    
    /*! \name Saving and Restoring
     * These are implementation methods only
     */
    //@{
    
    /*!
     * \brief Implementation of save, for saving CGI environments
     *
     * This is called internally by Cgicc
     * \param filename The name of the file to which to save
     */
    void 
    save(const std::string& filename) 		const;
    
    /*!
     * \brief Implementation of restore, for restoring CGI environments
     *
     * This is called internally by Cgicc
     * \param filename The name of the file from which to restore
     */
    // Implementation of restore
    void 
    restore(const std::string& filename);
    //@}
    
    // ============================================================
    
  private:
    
    // Parse the list of cookies from a string to a vector
    void 
    parseCookies();
    
    // Parse a single cookie string (name=value) pair
    void
    parseCookie(const std::string& data);
    
    // Read in all the environment variables
    void 
    readEnvironmentVariables(CgiInput *input);
    
    unsigned long 		fServerPort;
    unsigned long 		fContentLength;
    bool			fUsingHTTPS;
    std::string 		fServerSoftware;
    std::string 		fServerName;
    std::string 		fGatewayInterface;
    std::string 		fServerProtocol;
    std::string 		fRequestMethod;
    std::string 		fPathInfo;
    std::string 		fPathTranslated;
    std::string 		fScriptName;
    std::string 		fQueryString;
    std::string 		fRemoteHost;
    std::string 		fRemoteAddr;
    std::string 		fAuthType;
    std::string 		fRemoteUser;
    std::string 		fRemoteIdent;
    std::string 		fContentType;
    std::string 		fAccept;
    std::string 		fUserAgent;
    std::string 		fPostData;
    std::string 		fRedirectRequest;
    std::string 		fRedirectURL;
    std::string 		fRedirectStatus;
    std::string 		fReferrer;
    std::string 		fCookie;
    std::vector<HTTPCookie> 	fCookies;
    std::string                 fAcceptLanguageString; 
  };
  
} // namespace cgicc

#endif /* ! _CGIENVIRONMENT_H_ */
