/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: XMLPI.h,v 1.2 2014/04/23 20:55:10 sebdiaz Exp $
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
#ifndef XMLPI_H
#define XMLPI_H
/*! \file XMLPI.h
 * \brief Class that abstracts a XMLPI 
 *
 * This class add the XML Declaration feature
 */

#include <string>

#include "CgiDefs.h"
#include "MStreamable.h"
#include "HTMLAttributeList.h"

namespace cgicc {
/*! \class XMLPI XMLPI.h cgicc/XMLPI.h 
   * \brief Class that abstracts a XMLPI
   *
   * This class add the XMLPI feature
   */
class XMLPI : public MStreamable {
	public:
	// ============================================================
    
    /*! \name Constructor and Destructor */
    //@{
    
    /*! 
	* \brief Constructor
          *
	* Create a new XMLPI.h object
	* \param input The name of the XMLPI
	*/
	XMLPI(std::string name);
	
	/*!
	* \brief Destructor 
	*
	* Delete this XMLPI object
	*/
	virtual ~XMLPI();
	//@}
	
	
	XMLPI& operator= (const XMLPI& element);
	inline std::string getName() const { return fName; }
	inline const HTMLAttributeList* getAttributes() const { return fAttributes; }
	void setAttributes(const HTMLAttributeList& attributes);
	XMLPI& set(const std::string& name);
	XMLPI& set(const std::string& name, const std::string& value);
	virtual void render(std::ostream& out) const;
	
	private:
	HTMLAttributeList* fAttributes;
	std::string fName;
};
}// namespace cgicc

#endif
