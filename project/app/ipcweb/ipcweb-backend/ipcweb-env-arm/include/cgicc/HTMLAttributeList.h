/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLAttributeList.h,v 1.8 2014/04/23 20:55:05 sebdiaz Exp $
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

#ifndef _HTMLATTRIBUTELIST_H_
#define _HTMLATTRIBUTELIST_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTMLAttributeList.h
 * \brief Class containing a list of HTMLAttribute objects
 *
 * The list is expandable and uses the STL vector class for storage
 */

#include <string>
#include <iostream>
#include <vector>

#include "CgiDefs.h"
#include "MStreamable.h"
#include "HTMLAttribute.h"

namespace cgicc {

  // ============================================================
  // Class HTMLAttributeList
  // ============================================================
  
#ifdef WIN32
  template class CGICC_API std::vector<HTMLAttribute>;
#endif
  
  /*! \class HTMLAttributeList HTMLAttributeList.h cgicc/HTMLAttributeList.h
   * \brief An expandable list of HTMLAttribute objects
   * 
   * An HTMLAttributeList represents any number of HTMLAttribute objects
   * which may be embedded in an HTMLElement.  To add HTMLAttribute objects
   * to an HTMLAttributeList, use the set() methods or functions.  
   * For example, 
   * \code 
   * cgicc::HTMLAttributeList list = cgicc::set("HEIGHT", "100").set("WIDTH", "100"); 
   * \endcode 
   * generates an HTMLAttributeList with two elements.  
   * \see HTMLAttribute 
   * \see HTMLElement 
   */
  class CGICC_API HTMLAttributeList 
  {
  public:
    
    // ============================================================
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*!
     * \brief Create an empty HTMLAttributeList. 
     *
     * HTMLAttributeLists are most often created with the set functions
     */
    HTMLAttributeList();
    
    /*!
     * \brief Create a new HTMLAttributeList, specifying the first element.
     *
     * The first attribute in the list is set to \c head
     * \param head The first element of the list
     */
    HTMLAttributeList(const HTMLAttribute& head);
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the elements in this list to those in \c list
     * \param list The HTMLAttributeList to copy.
     */
    HTMLAttributeList(const HTMLAttributeList& list);
    
    /*!
     * \brief Destructor 
     *
     * Delete this HTMLAttributeList object
     */
    ~HTMLAttributeList();
    //@}
    
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Assign one HTMLAttributeList to another.
     *
     * Sets the elements in this list to those in \c list
     * \param list The HTMLAttributeList to copy
     */
    HTMLAttributeList&
    operator= (const HTMLAttributeList &list);
    //@}
    
    
    // ============================================================
    
    /*! \name List Management 
     * Add attributes to the list
     */
    //@{
    
    /*! 
     * \brief Add an atomic HTMLAttribute to this list
     *
     * \c isindex is an example of an atomic attribute.
     * \param name The name of the HTMLAttribute to set.
     * \return A reference to \c this
     */
    HTMLAttributeList& 
    set(const std::string& name);
    
    /*!
     * \brief Add a HTMLAttribute to this list
     *
     * For a list of possible attributes see http://www.w3.org/TR/REC-html40/
     * \param name The name of the HTMLAttribute to set.
     * \param value The value of the HTMLAttribute to set.
     * \return A reference to \c this
     */
    HTMLAttributeList& 
    set(const std::string& name, 
	const std::string& value);
    //@}
    
    /*! \name Utility Methods */
    //@{
    
    /*! 
     * \brief Render this HTMLAttributeList to an ostream
     *
     * This is used for output
     * \param out The ostream to which to write
     */
    void 
    render(std::ostream& out) 				const;
    //@}
    
  private:
    std::vector<HTMLAttribute> fAttributes;
  };
  
  // ============================================================
  // List manipulators
  // ============================================================
  
  /*!
   * \brief Create a new HTMLAttributeList, and set an HTMLAttribute.
   *
   * This function is usually called from within the constructor of an
   * HTMLElement:
   * \code
   * out << img(set("ISINDEX")) << endl;
   * \endcode
   * \param name The name of the HTMLAttribute to set.
   * \return A reference to the list.
   */
  inline HTMLAttributeList 
  set(const std::string& name)
  { return HTMLAttributeList(HTMLAttribute(name)); }
  
  /*!
   * \brief Create a new HTMLAttributeList, and set an HTMLAttribute.
   *
   * This function is usually called from within the constructor of an
   * HTMLElement:
   * \code
   * out << a("link text", set("HREF","http://www.foo.com")) << endl;
   * \endcode
   * \param name The name of the HTMLAttribute to set.
   * \param value The value of the HTMLAttribute to set.
   * \return A reference to the list.
   */
  inline HTMLAttributeList 
  set(const std::string& name, 
      const std::string& value)
  { return HTMLAttributeList(HTMLAttribute(name, value)); }
  
} // namespace cgicc

#endif /* ! _HTMLATTRIBUTES_H_ */
