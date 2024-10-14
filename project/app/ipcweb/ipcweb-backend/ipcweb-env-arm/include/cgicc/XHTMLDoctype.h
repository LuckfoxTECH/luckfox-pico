/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: XHTMLDoctype.h,v 1.2 2014/04/23 20:55:10 sebdiaz Exp $
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
#ifndef XXHTMLDoctype_H
#define XXHTMLDoctype_H

/*! \file XHTMLDoctype.h
 * \brief Class that abstracts a XHTML Doc Type
 *
 * This class add the XHTML Doc feature
 */

#include <string>

#include <MStreamable.h>

namespace cgicc {

/*! \class XHTMLDoctype XHTMLDoctype.h cgicc/XHTMLDoctype.h 
   * \brief Class that abstracts a XHTML Doc Type
   *
   * This class add the XHTML Doc Type feature
   */
class CGICC_API XHTMLDoctype : public MStreamable {
	public:
	enum EDocumentType {
		eStrict,
		eTransitional,
		eFrames,
		eHTML5 // 11.30.14 t.o.
	};
	// ============================================================
    
    /*! \name Constructor and Destructor */
    //@{
    
    /*! 
     * \brief Constructor
     *
     * Create a new XHTMLDoctype.h object
     * \param input The Document Type
     */
	XHTMLDoctype(EDocumentType type = eStrict);
	/*!
     * \brief Destructor 
     *
     * Delete this XHTMLDoctype.h object
     */
	virtual ~XHTMLDoctype();
	
	//@}
	
	virtual void render(std::ostream& out) const;
	
	private:
	EDocumentType fType;
};
}// namespace cgicc

/// XHMTL namespace attribute for html tag - usage: html().xmlnsXHTML()
#define xmlnsXHTML() set("xmlns", "http://www.w3.org/1999/xhtml")

#endif
