/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTTPContentHeader.h,v 1.8 2014/04/23 20:55:06 sebdiaz Exp $
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

#ifndef _HTTPCONTENTHEADER_H_
#define _HTTPCONTENTHEADER_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTTPContentHeader.h
 * \brief Class for sending data of a specific MIME type
 * 
 */

#include <string>

#include "HTTPHeader.h"

namespace cgicc {
  
  // ============================================================
  // Class HTTPContentHeader
  // ============================================================
  /*! \class HTTPContentHeader HTTPContentHeader.h cgicc/HTTPContentHeader.h
   * \brief HTTP header for data of a specified MIME type. 
   *
   */
  class CGICC_API HTTPContentHeader : public HTTPHeader 
  {
  public:
    
    /*! \name Constructor and Destructor */
    //@{
    
    /*!
     * \brief Create a new MIME type header.
     * \param mimeType The MIME type of the data which will be sent.
     */
    HTTPContentHeader(const std::string& mimeType);
    
    /*!
     * \brief Destructor 
     *
     */
    virtual ~HTTPContentHeader();
    //@}
    
    /*! \name Inherited Methods */
    //@{ 
    virtual void 
    render(std::ostream& out) 			const;
    //@}
    
  private:
    HTTPContentHeader();
  };
  
} // namespace cgicc

#endif /* ! _HTTPCONTENTHEADER_H_ */
