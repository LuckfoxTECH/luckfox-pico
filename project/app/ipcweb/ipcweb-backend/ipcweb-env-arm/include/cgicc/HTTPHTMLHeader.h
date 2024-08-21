/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTTPHTMLHeader.h,v 1.8 2014/04/23 20:55:08 sebdiaz Exp $
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

#ifndef _HTTPHTMLHEADER_H_
#define _HTTPHTMLHEADER_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTTPHTMLHeader.h
 * \brief Shortcut to HTTPContentHeader for \c text/html
 * 
 */

#include "HTTPContentHeader.h"

namespace cgicc {

  // ============================================================
  // Class HTTPHTMLHeader
  // ============================================================
  /*! \class HTTPHTMLHeader HTTPHTMLHeader.h cgicc/HTTPHTMLHeader.h
   * \brief Shortcut to HTTPContentHeader for \c text/html
   *
   */
  class CGICC_API HTTPHTMLHeader : public HTTPContentHeader 
  {
  public:
    
    /*! \name Constructor and Destructor */
    //@{
    
    /*!
     * \brief Create a new \c text/html header 
     *
     */
    HTTPHTMLHeader();
    
    /*!
     * \brief Destructor 
     *
     */
    virtual ~HTTPHTMLHeader();
    //@}
  };
  
} // namespace cgicc

#endif /* ! _HTTPHTMLHEADER_H_ */
