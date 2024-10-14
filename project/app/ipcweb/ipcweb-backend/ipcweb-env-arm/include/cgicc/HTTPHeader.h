/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTTPHeader.h,v 1.10 2014/04/23 20:55:08 sebdiaz Exp $
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

#ifndef _HTTPHEADER_H_
#define _HTTPHEADER_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTTPHeader.h
 * \brief Abstract base class for simple HTTP headers
 * 
 */

#include <string>
#include <vector>

#include "MStreamable.h"
#include "HTTPCookie.h"

namespace cgicc {
  
  // ============================================================
  // Class HTTPHeader
  // ============================================================
  
  /*! \class HTTPHeader HTTPHeader.h cgicc/HTTPHeader.h
   * \brief Abstract base class for all simple HTTP response headers.
   *
   */
  class CGICC_API HTTPHeader : public MStreamable 
  {
  public:
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*!
     * \brief Constructor.
     * \param data The header data.
     */
    HTTPHeader(const std::string& data);
    
    /*!
     * \brief Copy constructor.
     * \param header The HTTPHeader to copy.
     */
    HTTPHeader(const HTTPHeader& header);
    
    /*! 
     * \brief Destructor 
     *
     */
    virtual ~HTTPHeader();
    //@}
    
    // ============================================================
    
    /*! \name Cookie Management */
    //@{
    
    /*!
     * \brief Set a cookie to go out with this HTTPResponseHeader
     * \param cookie The HTTPCookie to set
     */
    inline HTTPHeader&
    setCookie(const HTTPCookie& cookie)
    { fCookies.push_back(cookie); return *this; }
    
    /*!
     * \brief Get a list of all cookies associated with this header
     * \return All the cookies associated with this header
     */
    inline const std::vector<HTTPCookie>&
    getCookies() 					const
    { return fCookies; }
    //@}
    
    // ============================================================
    
    /*! \name Accessor Method */
    //@{
    
    /*!
     * Get the data contained in this HTTP header.
     * @return The data contained in this header.
     */
    inline std::string 
    getData() 					const
    { return fData; }
    //@}
    
    // ============================================================
    
    /*! \name Subclass Methods */
    //@{
    
    /*!
     * \brief Write this HTTPHeader to a stream.  
     *
     * Subclasses must implement this function.
     * \param out The ostream to which to write.
     */
    virtual void 
    render(std::ostream& out) 			const = 0;
    //@}
    
  private:
    HTTPHeader();
    
    std::string fData;
    std::vector<HTTPCookie> fCookies;
  };
  
} // namespace cgicc

#endif /* ! _HTTPHEADER_H_ */
