/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: FormEntry.h,v 1.15 2014/04/23 20:55:04 sebdiaz Exp $
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

#ifndef _FORMENTRY_H_
#define _FORMENTRY_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file FormEntry.h
 * \brief Class representing a single HTML form entry.
 *
 * FormEntry is an immutable class representing a single user entry
 * in an HTML form element such as a text field, radio button, or a 
 * checkbox.  A FormEntry is essentially a name/value pair, where the
 * name is the name of the form element as specified in the HTML form
 * itself, and the value is the user-entered or user-selected value.
 */

#include <iostream>
#include <string>
#include <climits>
#include <cfloat>

#include "CgiDefs.h"
#include "CgiUtils.h"

namespace cgicc {
  
  // ============================================================
  // Class FormEntry
  // ============================================================
  
  /*! \class FormEntry FormEntry.h cgicc/FormEntry.h
   * \brief Class representing a single HTML form entry.
   *
   * FormEntry is an immutable class representing a single user entry
   * in an HTML form element such as a text field, radio button, or a 
   * checkbox.  A FormEntry is essentially a name/value pair, where the
   * name is the name of the form element as specified in the HTML form
   * itself, and the value is the user-entered or user-selected value.
   *
   * If a \c QUERY_STRING contained the fragment \c cgicc=yes the
   * corresponding FormEntry would have a name of \c cgicc and a value
   * of \c yes
   *
   * \sa FormFile
   */
  class CGICC_API FormEntry
  {
  public:
    
    // ============================================================
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*! 
     * \brief Default constructor
     *
     * Shouldn't be used.
     */
    inline
    FormEntry()
    {}
    
    /*!
     * \brief Create a new FormEntry
     *
     * This is usually not called directly, but by Cgicc.
     * \param name The name of the form element
     * \param value The value of the form element
     */
    inline
    FormEntry(const std::string& name, 
	      const std::string& value)
      : fName(name), fValue(value)
    {}
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the name and value of this FormEntry to those of \c entry.
     * \param entry The FormEntry to copy.
     */
    inline
    FormEntry(const FormEntry& entry)
    { operator=(entry); }
    
    /*! 
     * \brief Destructor.
     *
     * Delete this FormEntry object
     */
    inline
    ~FormEntry()
    {}
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Compare two FormEntrys for equality.
     *
     * FormEntrys are equal if they have the same name and value.
     * \param entry The FormEntry to compare to this one.
     * \return \c true if the two FormEntrys are equal, \c false otherwise.
     */
    inline bool 
    operator== (const FormEntry& entry) 		const
    { return stringsAreEqual(fName, entry.fName); }
    
    /*!
     * \brief Compare two FormEntrys for inequality.
     *
     * FormEntrys are equal if they have the same name and value.
     * \param entry The FormEntry to compare to this one.
     * \return \c false if the two FormEntrys are equal, \c true otherwise.
     */
    inline bool
    operator!= (const FormEntry& entry) 		const
    { return ! operator==(entry); }
    
#ifdef WIN32
    /* Dummy operator for MSVC++ */
    inline bool
    operator< (const FormEntry& entry) 			const
    { return false; }
#endif
    
    /*!
     * \brief Assign one FormEntry to another.  
     *
     * Sets the name and value of this FormEntry to those of \c entry.
     * \param entry The FormEntry to copy.
     * \return A reference to this.
     */
    FormEntry& 
    operator= (const FormEntry& entry);
    //@}
    
    // ============================================================
    
    /*! \name Accessor Methods 
     * Information on the form element
     */
    //@{
    
    /*!
     * \brief Get the name of the form element.
     *
     * The name of the form element is specified in the HTML form that
     * called the CGI application.
     * \return The name of the form element.
     */
    inline std::string
    getName() 						const
    { return fName; }
    
    /*!
     * \brief Get the value of the form element as a string
     *
     * The value returned may contain line breaks.
     * \return The value of the form element.
     */
    inline std::string
    getValue() 						const
    { return fValue; }
    
    /*!
     * \brief Get the value of the form element as a string
     *
     * The value returned may contain line breaks.
     * \return The value of the form element.
     */
    inline std::string
    operator* () 					const
    { return getValue(); }
    
    /*!
     * \brief Get the value of the form element as a string
     *
     * The value returned will be truncated to a specific length.
     * The value may contain line breaks.
     * \param maxChars The maximum number of characters to return.
     * \return The value of the form element, truncated to the specified length.
     */
    inline std::string
    getValue(std::string::size_type maxChars) 		const
    { return makeString(maxChars, true); }
    
    /*!
     * \brief Get the value of the form element as a string
     *
     * The value returned will be stripped of all line breaks.
     * \return The value of the form element, stripped of all line breaks.
     */
    inline std::string
    getStrippedValue() 					const
    { return getStrippedValue(INT_MAX); }
    
    /*!
     * \brief Get the value of the form element as a string
     *
     * The value returned will be stripped of all line breaks
     * and truncated to a specific length.
     * \param maxChars The maximum number of characters to return.
     * \return The value of the form element, stripped of all line breaks and
     * truncated to the specified length.
     */
    inline std::string
    getStrippedValue(std::string::size_type maxChars) 	const
    { return makeString(maxChars, false); }
    
    /*!
     * \brief Get the value of the form element as an integer
     *
     * No syntax checking is performed on the string value.
     * \param min The minimum value to return (optional).
     * \param max The maximum value to return (optional).
     * \return The integer value of the form element.
     */
    long
    getIntegerValue(long min = LONG_MIN, 
		    long max = LONG_MAX) 		const;
    
    /*!
     * \brief Get the value of the form element as an integer
     *
     * No syntax checking is performed on the string value.
     * \param min The minimum value to return.
     * \param max The maximum value to return.
     * \param bounded Set to \c true if the value was originally outside the 
     * limits, \c false otherwise
     * \return The integer value of the form element.
     */
    long
    getIntegerValue(long min, 
		    long max,
		    bool& bounded) 			const;
    
    /*!
     * \brief Get the value of the form element as a double
     *
     * No syntax checking is performed on the string value.
     * \param min The minimum value to return (optional).
     * \param max The maximum value to return (optional).
     * \return The double value of the form element.
     */
    double 
    getDoubleValue(double min = -DBL_MAX, 
		   double max = DBL_MAX) 		const;
    
    /*!
     * \brief Get the value of the form element as a double
     *
     * No syntax checking is performed on the string value.
     * \param min The minimum value to return.
     * \param max The maximum value to return.
     * \param bounded Set to \c true if the value was originally outside the 
     * limits, \c false otherwise
     * \return The double value of the form element.
     */
    double 
    getDoubleValue(double min, 
		   double max,
		   bool& bounded) 			const;
    
    
    /*!
     * \brief Get the number of characters in the value of the form element.
     *
     * Note that a character may or may not equal one byte.
     * \return The length of the value of the form element
     */
    inline std::string::size_type
    length() 						const
    { return fValue.length(); }
    
    /*!
     * \brief Determine if this form element is empty
     *
     * In an empty form element, length() == 0.
     * \return \c true if this form element is empty, \c false otherwise.
     */
    inline bool 
    isEmpty() 						const
    { return (0 == length()); }
    //@}
    
  private:  
    // utility function
    std::string
    makeString(std::string::size_type maxLen, 
	       bool allowNewlines) 			const;
    
    std::string fName;		// the name of this form element
    std::string fValue;		// the value of this form element
  };
  
} // namespace cgicc

#endif /* ! _FORMENTRY_H_ */
