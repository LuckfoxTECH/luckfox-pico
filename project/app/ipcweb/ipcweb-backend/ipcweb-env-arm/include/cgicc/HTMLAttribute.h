/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLAttribute.h,v 1.9 2014/04/23 20:55:04 sebdiaz Exp $
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

#ifndef _HTMLATTRIBUTE_H_
#define _HTMLATTRIBUTE_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTMLAttribute.h
 * \brief Class dealing with HTML element attributes
 *
 * For example, in the HTML code
 \verbatim
 <br clear="all" />
 \endverbatim
 * \c clear is an attribute of the \c br element.
 */

#include <string>
#include <iostream>

#include "CgiDefs.h"
#include "MStreamable.h"

namespace cgicc {
  
  // ============================================================
  // Class HTMLAttribute
  // ============================================================
  
  /*! \class HTMLAttribute HTMLAttribute.h cgicc/HTMLAttribute.h
   * \brief Class representing a name or a single name/value pair
   *
   * An HTMLAttribute represents a single name/value
   * pair inside an HTMLElement.  For example, in the HTML code:
   \verbatim
   <a href="mailto:sbooth@gnu.org">Send mail</a>
   \endverbatim
   * The (name, value) pair <tt>(href, mailto:%sbooth@gnu.org)</tt> is
   * an HTMLAttribute.
   * HTMLAttribute objects are usually not created directly, but
   * using the set() methods.  To generate the HTML above using
   * %cgicc, write
   * \code
   * cout << cgicc::a("Send Mail").set("href", "mailto:sbooth@gnu.org");
   * \endcode
   * \sa HTMLAttributeList
   */
  class CGICC_API HTMLAttribute : public MStreamable 
  {
  public:
    
    // ============================================================
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*!
     * \brief Create an empty HTMLAttribute. 
     *
     * The name and value are set to an empty string.
     */
    HTMLAttribute();
    
    /*!
     * \brief Create an HTMLAttribute with the given name.
     *
     * This will simply set the name and value to the same value.
     * \param name The name of the attribute.
     */
    HTMLAttribute(const std::string& name);
    
    /*!
     * \brief Create an HTMLAttribute with the given name and value.
     *
     * Most attributes are of this form
     * \param name The attribute's name, for example \c href
     * \param value The attributes's alue, for exampe \c foo.html
     */
    HTMLAttribute(const std::string& name, 
		  const std::string& value);
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the name of value of this attribute to those of \c attribute
     * \param attribute The HTMLAttribute to copy.
     */
    HTMLAttribute(const HTMLAttribute& attribute);
    
    /*!
     * \brief Destructor 
     *
     * Delete this HTMLAttribute object
     */
    virtual ~HTMLAttribute();
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Compare two HTMLAttributes for equality.
     *
     * HTMLAttributes are equal if they have the same name and value.
     * \param att The HTMLAttribute to compare to this one.
     * \return \c true if the two HTMLAttributes are equal, \c false otherwise.
     */
    bool 
    operator== (const HTMLAttribute& att) 		const;
    
    /*!
     * \brief Compare two HTMLAttributes for inequality.
     *
     * HTMLAttributes are equal if they have the same name and value.
     * \param att The HTMLAttribute to compare to this one.
     * \return \c false if the two HTMLAttributes are equal, \c true otherwise.
     */
    inline bool 
    operator!= (const HTMLAttribute& att) 		const
    { return ! operator==(att); }
    
#ifdef WIN32
    /* Dummy operator for MSVC++ */
    inline bool
    operator< (const HTMLAttribute& att) const
    { return false; }
#endif
    
    /*!
     * \brief Assign one HTMLAttribute to another.
     *
     * Sets the name of value of this attribute to those of \c att
     * \param att The HTMLAttribute to copy.
     * \return A reference to this.
     */
    HTMLAttribute& 
    operator= (const HTMLAttribute& att);
    //@}  
    
    // ============================================================
    
    /*! \name Accessor Methods 
     * Information on the attribute
     */
    //@{
    
    /*!
     * \brief Get the name of this HTMLAttribute.
     *
     * For example, \c HREF
     * \return The attribute's name.
     */
    inline std::string 
    getName() 						const
    { return fName; }
    
    /*!
     * \brief Get the value of this HTMLAttribute.
     *
     * For example, \c http://www.gnu.org
     * \return The attribute's value.
     */
    inline std::string
    getValue() 						const
    { return fValue; }
    //@}
  
    // ============================================================
    
    /*! \name Mutator Methods 
     * Set properties of the attribute
     */
    //@{
    
    /*!
     * \brief Set the name of this HTMLAttribute.
     *
     * Use this method if the name wasn't specified in the constructor
     * \param name The new name of the attribute.
     */
    inline void 
    setName(const std::string& name)
    { fName = name; }
    
    /*!
     * \brief Set the value of this HTMLAttribute.
     *
     * Use this method if the value wasn't specified in the constructor
     * \param value The new value of the attribute.
     */
    inline void 
    setValue(const std::string& value)
    { fValue = value; }
    //@}
    
    /*!
     * \brief Render this attribute to an ostream
     *
     * This is used for output purposes
     * \param out The ostream to which to write
     */
    virtual void 
    render(std::ostream& out) 				const;
    
  private:
    std::string fName;
    std::string fValue;
  };
  
} // namespace cgicc

#endif /* ! _HTMLATTRIBUTE_H_ */
