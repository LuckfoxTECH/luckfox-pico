/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: Cgicc.h,v 1.20 2014/04/23 20:55:04 sebdiaz Exp $
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

#ifndef _CGICC_H_
#define _CGICC_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file Cgicc.h
 * \brief The main header file for the GNU %cgicc library
 */

/*
 * The GNU cgicc library, by Stephen F. Booth <sbooth@gnu.org>
 * http://www.cgicc.org
 *
 * The latest version can be found on your closest GNU mirror site.
 * Please mail bug reports to <bug-cgicc@gnu.org>
 */

#include <vector>
#include <string>

#include "CgiDefs.h"
#include "FormEntry.h"
#include "FormFile.h"
#include "CgiInput.h"
#include "CgiEnvironment.h"

namespace cgicc {
  
#ifdef WIN32
  template class CGICC_API std::vector<FormEntry>;
  template class CGICC_API std::vector<FormFile>;
#endif
  
  class MultipartHeader;
  
  // ============================================================
  // Iterator typedefs
  // ============================================================
  
  //! A vector of FormEntry objects
  typedef std::vector<FormEntry>::iterator 	form_iterator;
  //! A vector of \c const FormEntry objects
  typedef std::vector<FormEntry>::const_iterator const_form_iterator;
  
  //! A vector of FormFile objects
  typedef std::vector<FormFile>::iterator 	file_iterator;
  //! A vector of \c const FormFile objects
  typedef std::vector<FormFile>::const_iterator const_file_iterator;
  
  // ============================================================
  // Class Cgicc
  // ============================================================
  
  /*! \class Cgicc Cgicc.h cgicc/Cgicc.h
   * \brief The main class of the GNU %cgicc library
   *
   * Cgicc is used to retrieve information on specific HTML form elements 
   * (such as checkboxes, radio buttons, and text fields), on uploaded files,
   * and to save, restore, and retrieve information on the CGI
   * environment.
   *
   * Normally, you will instantiate an object of this type in 
   * \c main():
   * \code
   * int
   * main(int argc, char **argv) {
   *   try {
   *     cgicc::Cgicc cgi;
   *     // do something with cgi
   *   }
   *
   *   catch(const exception& e) {
   *    //handle the error
   *   }
   * }
   * \endcode
   */
  class CGICC_API Cgicc {
  public:
    
    // ============================================================
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*! 
     * \brief Constructor 
     *
     * If you are using %cgicc with FastCGI, you will need to pass 
     * a \c CgiInput subclass that %cgicc will use to read input.  If
     * \c input is omitted, standard input and environment
     * variables will be used.
     * \param input A CgiInput object to use for reading input
     */
    Cgicc(CgiInput *input = 0);
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the values of this Cgicc to those of \c cgi.
     * \param env The Cgicc to copy.
     */
    inline
    Cgicc(const Cgicc& cgi)
      : fEnvironment(cgi.fEnvironment)
    { operator=(cgi); }

    /*! 
     * \brief Destructor 
     *
     * Delete this Cgicc object
     */
    ~Cgicc();
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Compare two Cgiccs for equality.
     *
     * Cgiccs are equal if they represent the same environment.
     * \param cgi The Cgicc to compare to this one.
     * \return \c true if the two Cgiccs are equal, \c false otherwise.
     */
    inline bool 
    operator== (const Cgicc& cgi) 		const
    { return this->fEnvironment == cgi.fEnvironment; }
    
    /*!
     * \brief Compare two Cgiccs for inequality.
     *
     * Cgiccs are equal if they represent the same environment.
     * \param cgi The Cgicc to compare to this one.
     * \return \c false if the two Cgiccs are equal, \c true otherwise.
     */
    inline bool
    operator!= (const Cgicc& cgi) 		const
    { return ! operator==(cgi); }
    
#ifdef WIN32
    /* Dummy operator for MSVC++ */
    inline bool
    operator< (const Cgicc& cgi) 		const
    { return false; }
#endif
    
    /*!
     * \brief Assign one Cgicc to another.  
     *
     * Sets the environment in this Cgicc to that of \c cgi.
     * \param cgi The Cgicc to copy.
     * \return A reference to this.
     */
    Cgicc& 
    operator= (const Cgicc& cgi);
    //@}

    // ============================================================
    
    /*! \name Library Information 
     * Information on this installation of %cgicc
     */
    //@{
    
    /*!
     * \brief Get the date on which this library was compiled.
     * 
     * This is a string of the form <TT>mmm dd yyyy</TT>.
     * \return The compile date
     */
    const char*
    getCompileDate() 					const;
    
    /*!
     * \brief Get the time at which this library was compiled.
     *
     * This is a string of the form \c hh:mm:ss in 24-hour time.
     * \return The compile time
     */
    const char*
    getCompileTime() 					const;
    
    /*!
     * \brief Get the version number of cgicc.
     *
     * The version number is a string of the form \c #.#.
     * \return The version number
     */
    const char*
    getVersion() 					const;
    
    /*! 
     * \brief Get the platform for which Cgicc was configured.
     *
     * The host is a string of the form \c processor-manufacturer-os
     * return The host triplet.
     */
    const char*
    getHost() 						const;
    //@}
    
    // ============================================================
    
    /*! \name Form Element Access 
     * Information on submitted form elements
     */
    //@{
    
    /*!
     * \brief Query whether a checkbox is checked.
     *
     * \param elementName The name of the element to query
     * \return \c true if the desired checkbox was checked, \c false if not
     */
    bool 
    queryCheckbox(const std::string& elementName) 	const;
    
    /*!
     * \brief Find a radio button in a radio group, or a selected list item.
     *
     * \param name The name of the radio button or list item to find.
     * \return An iterator referring to the desired element, if found.
     */
    inline form_iterator 
    operator[] (const std::string& name)
    { return getElement(name); }

    /*!
     * \brief Find a radio button in a radio group, or a selected list item.
     *
     * \param name The name of the radio button or list item to find.
     * \return The desired element, or an empty string if not found.
     */
    std::string
    operator() (const std::string& name) 		const;
    
    /*!
     * \brief Find a radio button in a radio group, or a selected list item.
     *
     * \param name The name of the radio button or list item to find.
     * \return An iterator referring to the desired element, if found.
     */
    inline const_form_iterator 
    operator[] (const std::string& name) 		const
    { return getElement(name); }
    
    /*!
     * \brief Find a radio button in a radio group, or a selected list item.
     *
     * \param name The name of the radio button or list item to find.
     * \return An iterator referring to the desired element, if found.
     */
    form_iterator 
    getElement(const std::string& name);
    
    /*!
     * \brief Find a radio button in a radio group, or a selected list item.
     *
     * \param name The name of the radio button or list item to find.
     * \return A const_iterator referring to the desired element, if found.
     */
    const_form_iterator 
    getElement(const std::string& name) 		const;
    
    /*!
     * \brief Find multiple checkboxes in a group or selected items in a list.
     *
     * \param name The name of the checkboxes or list to find.
     * \param result A vector to hold the result.
     * \return \c true if any elements were found, \c false if not.
     */
    bool 
    getElement(const std::string& name,
	       std::vector<FormEntry>& result) 		const;
    
    /*!
     * \brief Find a radio button in a radio group, or a selected list item.
     *
     * \param value The value of the radio button or list item to find.
     * \return An iterator referring to the desired element, if found.
     */
    form_iterator 
    getElementByValue(const std::string& value);
    
    /*!
     * \brief Find a radio button in a radio group, or a selected list item.
     *
     * \param value The value of the radio button or list item to find.
     * \return A const_iterator referring to the desired element, if found.
     */
    const_form_iterator 
    getElementByValue(const std::string& value) 	const;
    
    /*!
     * \brief Find multiple checkboxes in a group or selected items in a list.
     *
     * \param value The value of the checkboxes or list to find.
     * \param result A vector to hold the result.
     * \return true if any elements were found, false if not.
     */
    bool 
    getElementByValue(const std::string& value,
		      std::vector<FormEntry>& result) 	const;
    
    /*!
     * \brief Get all the submitted form entries, excluding files.
     *
     * \return A vector containing all the submitted elements.
     */
    inline const std::vector<FormEntry>& 
    operator* () 					const
    { return fFormData; }
    
    /*!
     * \brief Get all the submitted form elements, excluding files.
     *
     * \return A vector containing all the submitted elements.
     */
    inline const std::vector<FormEntry>&
    getElements() 					const
    { return fFormData; }
    //@}
    
    // ============================================================
    
    /*! \name Uploaded File Access */
    //@{
    
    /*!
     * \brief Find an uploaded file.
     *
     * \param name The name of the file.
     * \return An iterator referring to the desired file, if found.
     */
    file_iterator 
    getFile(const std::string& name);
    
    /*!
     * \brief Find an uploaded file.
     *
     * \param name The name of the file.
     * \return An iterator referring to the desired file, if found.
     */
    const_file_iterator 
    getFile(const std::string& name) 			const;
    
    /*!
     * Get all uploaded files.
     * \return A vector containing all the uploaded files.
     */
    inline const std::vector<FormFile>&
    getFiles() 						const
    { return fFormFiles; }
    //@}
    
    // ============================================================
    
    /*! \name Environment Access */
    //@{
    
    /*!
     * Get the current runtime environment.
     * \return The current CGI environment.
     */
    inline const CgiEnvironment&
    getEnvironment() 					const
    { return fEnvironment;}
    //@}
    
    // ============================================================
    
    /*! \name Save and Restore */
    //@{
    
    /*!
     * \brief Save the current CGI environment to a file.
     *
     * This is useful for debugging CGI applications.
     * \param filename The name of the file to which to save.
     */
    void 
    save(const std::string& filename) 			const;
    
    /*!
     * \brief Restore from a previously-saved CGI environment.
     *
     * This is useful for debugging CGI applications.
     * \param filename The name of the file from which to restore.
     */
    void 
    restore(const std::string& filename);
    //@}
    
  private:
    CgiEnvironment 		fEnvironment;
    std::vector<FormEntry> 	fFormData;
    std::vector<FormFile> 	fFormFiles;

    // Convert query string into a list of FormEntries
    void 
    parseFormInput(const std::string& data, const std::string& content_type = "application/x-www-form-urlencoded");
    
    // Parse a multipart/form-data header
    MultipartHeader
    parseHeader(const std::string& data);
    
    // Parse a (name=value) form entry
    void 
    parsePair(const std::string& data);
    
    // Parse a MIME entry for ENCTYPE=""
    void
    parseMIME(const std::string& data);
    
    // Find elements in the list of entries
    bool 
    findEntries(const std::string& param, 
		bool byName,
		std::vector<FormEntry>& result) 	const;
  };
  
} // namespace cgicc

#endif /* ! _CGICC_H_ */
