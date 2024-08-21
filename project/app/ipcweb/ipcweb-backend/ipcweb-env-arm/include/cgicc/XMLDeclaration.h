/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: XMLDeclaration.h,v 1.1 2008/01/19 15:43:57 sebdiaz Exp $
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
#ifndef XMLDECLARATION_H
#define XMLDECLARATION_H
/*! \file XMLDeclaration.h
 * \brief Class that abstracts a XML Declaration
 *
 * This class add the XML Declaration feature
 */

#include "XMLPI.h"

#include <string>

namespace cgicc{
/*! \class XMLDeclaration XMLDeclaration.h cgicc/XMLDeclaration.h 
   * \brief Class that abstracts a XMLDeclaration
   *
   * This class add the XMLDeclaration feature
   */
class XMLDeclaration : public XMLPI {
	public:
		// ============================================================
    
    /*! \name Constructor and Destructor */
    //@{
    
    /*! 
     * \brief Constructor
     *
     * Create a new XMLDeclaration.h object
     * \param input The Xml version (generally 1.0 or 1.1)
     */
	XMLDeclaration(std::string version = "1.0") : XMLPI("xml") { set("version", version); }
	/*!
     * \brief Destructor 
     *
     * Delete this XMLDeclaration object
     */
	 virtual ~XMLDeclaration(){};
	//@}
};
}// namespace cgicc

#endif

