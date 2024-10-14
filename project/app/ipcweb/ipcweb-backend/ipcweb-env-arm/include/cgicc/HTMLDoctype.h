/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLDoctype.h,v 1.9 2014/04/23 20:55:05 sebdiaz Exp $
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

#ifndef _HTMLDOCTYPE_H_
#define _HTMLDOCTYPE_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTMLDoctype.h
 * \brief Class which specifies the DTD of the HTML 4 document
 * 
 */

#include <string>

#include "MStreamable.h"

namespace cgicc {
  
  // ============================================================
  // Class HTMLDoctype
  // ============================================================
  
  /*! \class HTMLDoctype HTMLDoctype.h cgicc/HTMLDoctype.h
   * \brief Specifies the DTD of the HTML 4 document
   *
   * To use this class, simply write an object of this type to an ostream:
   * \code
   * out << cgicc::HTMLDoctype();
   * \endcode
   * For more information, see \c http://www.w3.org/MarkUp/ and 
   * \c http://www.w3.org/TR/REC-html40/
   */
  class CGICC_API HTMLDoctype : public MStreamable 
  {
  public:
    
    /*! The DTD used by this document. */
    enum EDocumentType {
      /*! The HTML 4.0 strict DTD (the default) */
      eStrict,
      /*! The HTML 4.0 Transitional DTD */
      eTransitional,
      /*! The HTML 4.0 Frameset DTD */
      eFrames,
		/*! HTML5 (added 11.30.14 t.o.) */ 
		eHTML5
    };
    
    
    /*!
     * \brief Constructor.
     * \param type The version of the HTML 4.0 DTD used by this document.
     */
    HTMLDoctype(EDocumentType type = eStrict);
    
    /*!
     * \brief Destructor 
     *
     */
    virtual ~HTMLDoctype();
    
    virtual void 
    render(std::ostream& out) const;
    
  private:
    EDocumentType fType;
  };
  
} // namespace cgicc

#endif /* ! _HTMLDOCTYPE_H_ */

