/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTTPStatusHeader.h,v 1.8 2014/04/23 20:55:09 sebdiaz Exp $
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

#ifndef _HTTPSTATUSHEADER_H_
#define _HTTPSTATUSHEADER_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTTPStatusHeader.h
 * \brief HTTP header to set a specific request status
 * 
 */

#include <string>

#include "HTTPHeader.h"

namespace cgicc {

  // ============================================================
  // Class HTTPStatusHeader
  // ============================================================
  
  /*! \class HTTPStatusHeader HTTPStatusHeader.h cgicc/HTTPStatusHeader.h
   * \brief HTTP header to set a specific request status
   *
   */
  class CGICC_API HTTPStatusHeader : public HTTPHeader 
  {
  public:
    
    /*!
     * \brief Create a new status header.
     * \param status The 3-digit status code, for example 404.
     * \param message The message associated with the status code, for example
     * "not found".
     */
    HTTPStatusHeader(int status, 
		     const std::string& message);
    
    /*! \brief Destructor */
    virtual ~HTTPStatusHeader();
    
    /*!
     * \brief Get the status code associated with this header.
     * \return The 3-digit status code of this header.
     */
    inline int 
    getStatusCode() 				const
    { return fStatus; }
    
    
    /*!
     * \brief Write this HTTPStatusHeader to a stream.  
     * \param out The ostream to which to write.
     */
    virtual void 
    render(std::ostream& out) 			const;
    
  private:
    HTTPStatusHeader();
    int fStatus;
  };
  
} // namespace cgicc

#endif /* ! _HTTPSTATUSHEADER_H_ */
