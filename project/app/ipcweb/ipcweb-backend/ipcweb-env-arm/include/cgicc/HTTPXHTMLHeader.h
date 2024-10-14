/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTTPXHTMLHeader.h,v 1.2 2014/04/23 20:55:09 sebdiaz Exp $
 *
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 David Roberts
		     2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
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
#ifndef HTTPXHTMLHEADER_H
#define HTTPXHTMLHEADER_H


#include "HTTPContentHeader.h"

/*! \file HTTPXHTMLHeader.h
 * \brief Class that abstracts a XHTML Content Header
 *
 * This class add the XHTML feature
 */

namespace cgicc {
/*! \class HTTPXHTMLHeader HTTPXHTMLHeader.h cgicc/HTTPXHTMLHeader.h 
   * \brief Class that abstracts a XHTML Content Header
   *
   * This class add the XHTML feature
   */
class CGICC_API HTTPXHTMLHeader : public HTTPContentHeader {
	public:
	// ============================================================
    
    /*! \name Constructor and Destructor */
    //@{
    
    /*! 
     * \brief Constructor
     *
     * Create a new HTTPXHTMLHeader object
     */
	HTTPXHTMLHeader();
	
	 /*!
     * \brief Destructor 
     *
     * Delete this HTTPXHTMLHeader object
     */
	virtual ~HTTPXHTMLHeader();
	//@}
};
}// namespace cgicc

#endif
