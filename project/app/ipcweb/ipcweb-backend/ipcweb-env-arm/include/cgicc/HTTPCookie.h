/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTTPCookie.h,v 1.10 2014/04/23 20:55:07 sebdiaz Exp $
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

#ifndef _HTTPCOOKIE_H_
#define _HTTPCOOKIE_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTTPCookie.h
 * \brief An HTTP Cookie
 */

#include <string>

#include "MStreamable.h"
#include "CgiDefs.h"

namespace cgicc {
  
  // ============================================================
  // Class HTTPCookie
  // ============================================================
  /*! \class HTTPCookie HTTPCookie.h cgicc/HTTPCookie.h
   * \brief An HTTP cookie
   *
   * An HTTP cookie is a way to maintain state between stateless HTTP
   * requests.  HTTP cookies consist of name/value pairs, with optional
   * comments, domains, and expiration dates. Usually, you will add one
   or more HTTPCookie objects to the HTTP headers your script is
   * returning.  For example, to set a cookie called \c count to \c 1 in
   * a normal HTML document:
   * \code
   *  out << HTTPHTMLHeader().setCookie(HTTPCookie("count","1"));
   * \endcode
   */
  class CGICC_API HTTPCookie : public MStreamable 
  {
  public:
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*!
     * \brief Default Constructor
     *
     * Create a new, empty HTTPCookie. 
     */
    HTTPCookie();
    
    /*!
     * \brief Create a new HTTPCookie
     *
     * This is the most commonly-used constructor.
     * \param name The name of the cookie.
     * \param value The value of the cookie.
     */
    HTTPCookie(const std::string& name, 
	       const std::string& value);
    
    /*!
     * \brief Create a new fully-spefified HTTPCookie
     *
     * 
     * \param name The name of the cookie.
     * \param value The value of the cookie.
     * \param comment Any comment associated with the cookie.
     * \param domain The domain for which this cookie is valid- an empty string
     * will use the hostname of the server which generated the cookie response.
     * If specified, the domain <em>must</em> start with a period('.'). 
     * \param maxAge A number of seconds defining the lifetime of this cookie.
     * A value of \c 0 indicates the cookie expires immediately.
     * \param path The subset of URLS in a domain for which the cookie is 
     * valid, for example \c /
     * @param secure Specifies whether this is a secure cookie.
     */
    HTTPCookie(const std::string& name, 
	       const std::string& value, 
	       const std::string& comment, 
	       const std::string& domain, 
	       unsigned long maxAge, 
	       const std::string& path,
	       bool secure);
    
    /*!
     * \brief Copy constructor
     *
     * Set the name, value, comment, domain, age and path of this cookie
     * to those of \c cookie
     * \param cookie The HTTPCookie to copy.
     */
    HTTPCookie(const HTTPCookie& cookie);
    
    /*!
     * \brief Destructor 
     *
     * Delete this HTTPCookie
     */
    virtual ~HTTPCookie();
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Compare two HTTPCookies for equality.
     *
     * Two HTTPCookie objects are equal if their names, values,
     * comments, domains, ages, and paths match.
     * \param cookie The HTTPCookie to compare to this one
     * \return true if the two HTTPCookies are equal, false otherwise.
     */
    bool 
    operator== (const HTTPCookie& cookie) 	const;
    
    /*!
     * \brief Compare two HTTPCookies for inequality.
     *
     * Two HTTPCookie objects are equal if their names, values,
     * comments, domains, ages, and paths match.
     * \param cookie The HTTPCookie to compare to this one
     * \return false if the two HTTPCookies are equal, true otherwise.
     */
    inline bool 
    operator != (const HTTPCookie& cookie) 	const
    { return ! operator==(cookie); }
    
#ifdef WIN32
    /* Dummy operator for MSVC++ */
    inline bool 
    operator< (const HTTPCookie& cookie) 	const
    { return false; }
#endif
    //@}
    
    // ============================================================
    
    /*! \name Accessor Methods */
    //@{
    
	/*!
     * \brief Mark this cookie as secure or unsecure.
     *
     */
    inline void 
    remove()
    { fRemoved = true; }

    /*!
     * \brief Mark this cookie as secure or unsecure.
    *
     * \param removed Set removed status
     */
    inline void 
    setRemoved(bool removed)
    { fRemoved = removed; }
 /*!
     * \brief Determine if this is a removed cookie.
     *
     * \return True if this cookie is removed, false if not.
     */
    inline bool 
    isRemoved()					const
    { return fRemoved; }
     /*!
     * \brief Create a new partially-spefified HTTPCookie for deletion 
     *
     * 
     * \param name The name of the cookie.
     * \param domain The domain for which this cookie is valid- an empty string
     * will use the hostname of the server which generated the cookie response.
     * If specified, the domain <em>must</em> start with a period('.'). 
     * \param path The subset of URLS in a domain for which the cookie is 
     * valid, for example \c /
     * @param secure Specifies whether this is a secure cookie.
     */
    HTTPCookie(const std::string& name, 
	       const std::string& domain, 
	       const std::string& path,
	       bool secure);
	
    /*!
     * \brief Get the name of this cookie.
     *
     * \return The name of this cookie.
     */
    inline std::string 
    getName() 					const
    { return fName; }
    
    /*!
     * \brief Get the value of this cookie.
     *
     * \return The value of this cookie.
     */
    inline std::string 
    getValue() 					const
    { return fValue; }
    
    /*!
     * \brief Get the comment of this cookie.
     *
     * \return The comment of this cookie.
     */
    inline std::string 
    getComment() 				const
    { return fComment; }
    
    /*!
     * \brief Get the domain for which this cookie is valid.
     *
     * An empty string indicates the hostname of the server which
     * generated the cookie response.
     * \return The domain of this cookie, or "" if none.
     */
    inline std::string 
    getDomain() 				const
    { return fDomain; }
    
    /*!
     * \brief Get the lifetime of this cookie, in seconds.
     *
     * \return The lifetime of this cookie, or 0 if none.
     */
    inline unsigned long
    getMaxAge() 				const
    { return fMaxAge; }
    
    /*!
     * \brief Get the path of this cookie.
     *
     * This is the subset of URLS in a domain for which the cookie is 
     * valid, for example \c /
     * \return The path of this cookie, or "" if none.
     */
    inline std::string 
    getPath() 					const
    { return fPath; }
    
    /*!
     * \brief Determine if this is a secure cookie.
     *
     * \return True if this cookie is secure, false if not.
     */
    inline bool 
    isSecure() 					const
    { return fSecure; }
    //@}
    
    // ============================================================
    
    /*! \name Mutator Methods */
    //@{
    
    /*!
     * \brief Set the name of this cookie.
     *
     * \param name The name of this cookie.
     */
    inline void 
    setName(const std::string& name)
    { fName = name; }
    
    /*!
     * \brief Set the value of this cookie.
     *
     * \param value The value of this cookie.
     */
    inline void 
    setValue(const std::string& value)
    { fValue = value; }
    
    /*!
     * \brief Set the comment of this cookie.
     *
     * \param comment The comment of this cookie.
     */
    inline void 
    setComment(const std::string& comment)
    { fComment = comment; }
    
    /*!
     * \brief Set the domain of this cookie.
     *
     * An empty string indicates the hostname of the server which
     * generated the cookie response.  If specified, the domain
     * <em>must</em> start with a period('.').
     * \param domain The domain of this cookie.
     */
    inline void 
    setDomain(const std::string& domain)
    { fDomain = domain; }
    
    /*!
     * \brief Set the lifetime of this cookie, in seconds.
     *
     * A value of \c 0 indicated the cookie expires immediately
     * \param maxAge The lifetime of this cookie, in seconds. 
     */
    inline void 
    setMaxAge(unsigned long maxAge)
    { fMaxAge = maxAge; }
    
    /*!
     * \brief Set the path of this cookie.
     *
     * This is the subset of URLS in a domain for which the cookie is 
     * valid, for example \c /
     * \param path The path of this cookie.
     */
    inline void 
    setPath(const std::string& path)
    { fPath = path; }
    
    /*!
     * \brief Mark this cookie as secure or unsecure.
     *
     * \param secure Whether this is a secure cookie.
     */
    inline void 
    setSecure(bool secure)
    { fSecure = secure; }
    //@}    
    
    // ============================================================
    
    /*! \name Inherited Methods */
    //@{
    virtual void 
    render(std::ostream& out) 			const;
    //@}
    
  private:
    std::string 	fName;
    std::string 	fValue;
    std::string 	fComment;
    std::string 	fDomain;
    unsigned long 	fMaxAge;
    std::string 	fPath;
    bool 		fSecure;
	bool		fRemoved;
  };
  
} // namespace cgicc

#endif /* ! _HTTPCOOKIE_H_ */
