/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLElementList.h,v 1.9 2014/04/23 20:55:06 sebdiaz Exp $
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

#ifndef _HTMLELEMENTLIST_H_
#define _HTMLELEMENTLIST_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTMLElementList.h
 * \brief Class containing a list of HTMLElement objects
 *
 * The list is expandable and uses the STL vector class for storage
 */

#include <string>
#include <vector>

#include "CgiDefs.h"
#include "MStreamable.h"
#include "HTMLAttribute.h"
#include "HTMLElement.h"

namespace cgicc {
  
  // ============================================================
  // Class HTMLElementList
  // ============================================================
  
#ifdef WIN32
  template class CGICC_API std::vector<HTMLElement*>;
#endif
  
  /*! \class HTMLElementList HTMLElementList.h cgicc/HTMLElementList.h
   * \brief An expandable list of HTMLElement objects
   *
   * An HTMLElementList represents any number of HTMLElement objects.  
   * To add HTMLElement objects to the list, use the add() methods:
   * \code
   * cgicc::HTMLElementList list;
   * list.add(br());
   * \endcode
   * \see HTMLAttribute
   * \see HTMLElement
   */
  class CGICC_API HTMLElementList
  {
  public:
    
    // ============================================================
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*!
     * \brief Create an empty HTMLElementList. 
     *
     * HTMLElementLists are most often created with the add() functions
     */
    HTMLElementList();
    
    /*!
     * \brief Create a new HTMLElementList, specifying the first element.
     *
     * The first element in the list is set to \c head
     * \param head The first element of the list
     */
    HTMLElementList(const HTMLElement& head);
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the elements in this list to those of \c list
     * \param list The HTMLElementList to copy.
     */
    HTMLElementList(const HTMLElementList& list);
    
    /*!
     * \brief Destructor 
     *
     * Deletes this HTMLElementList object
     */
    ~HTMLElementList();
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Assignment operator 
     *
     * Sets the elements in this list to those of \c list
     * \param list The HTMLElementList to copy
     * \return A reference to \c this
     */
    HTMLElementList&
    operator= (const HTMLElementList& list);
    //@}
    
    // ============================================================
    
    /*! \name List Management 
     * Manage the elements in the list
     */
    //@{
    
    /*!
     * \brief Add an HTMLElement to the list.
     *
     * \param element The HTMLElement to add.
     * \return A reference to \c this
     */
    HTMLElementList&
    add(const HTMLElement& element);
    
    /*!
     * \brief Add an HTMLElement to the list.
     *
     * \param element The HTMLElement to add.
     * \return A reference to the \c this
     */
    HTMLElementList&
    add(HTMLElement *element);
    //@}
    
    /*! Utility Methods */
    //@{
    
    /*!
     * \brief Render this HTMLElementList 
     *
     * \param out The ostream to which to write
     */
    void 
    render(std::ostream& out) 				const;
    //@}
    
  private:
    std::vector<HTMLElement*> fElements;
    // elements must be stored as pointers, otherwise polymorphism doesn't work
  };
  
} // namespace cgicc

#endif /* ! _HTMLELEMENTLIST_H_ */
