/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: FormFile.h,v 1.12 2014/04/23 20:55:04 sebdiaz Exp $
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

#ifndef _FORMFILE_H_
#define _FORMFILE_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file FormFile.h
 * \brief Class representing a file submitted via an HTML form.
 *
 * FormFile is an immutable class reprenting a file uploaded via
 * the HTTP file upload mechanism.  If you are going to use file upload
 * in your CGI application, remember to set the ENCTYPE of the form to
 * \c multipart/form-data.
 */

#include <iostream>
#include <string>

#include "CgiDefs.h"

namespace cgicc {
  
  // ============================================================
  // Class FormFile
  // ============================================================
  
  /*! \class FormFile FormFile.h cgicc/FormFile.h
   * \brief Class representing a file submitted via an HTML form.
   *
   * FormFile is an immutable class reprenting a file uploaded via
   * the HTTP file upload mechanism.  If you are going to use file upload
   * in your CGI application, remember to set the ENCTYPE of the form to
   * \c multipart/form-data.
   * \verbatim
   <form method="post" action="http://change_this_path/cgi-bin/upload.cgi" 
   enctype="multipart/form-data">
   \endverbatim
   * \sa FormEntry
   */
  class CGICC_API FormFile
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
    FormFile()
    {}
    
    /*!
     * \brief Create a new FormFile.
     *
     * This is usually not called directly, but by Cgicc.
     * \param name The \e name of the form element.
     * \param filename The \e filename of the file on the remote machine.
     * \param dataType The MIME content type of the data, if specified, or 0.
     * \param data The file data.
     */
    FormFile(const std::string& name, 
	     const std::string& filename, 
	     const std::string& dataType, 
	     const std::string& data);
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the name, filename, datatype, and data to those of \c file
     * @param file The FormFile to copy.
     */
    inline
    FormFile(const FormFile& file)
    { operator=(file); }
    
    /*! 
     * \brief Destructor 
     *
     * Delete this FormFile object
     */
    inline
    ~FormFile()
    {}
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Compare two FormFiles for equality.
     *
     * FormFiles are equal if they have the same filename.
     * @param file The FormFile to compare to this one.
     * @return \c true if the two FormFiles are equal, \c false otherwise.
     */
    bool 
    operator== (const FormFile& file) 			const;
    
    /*!
     * \brief Compare two FormFiles for inequality.
     *
     * FormFiles are equal if they have the same filename.
     * \param file The FormFile to compare to this one.
     * \return \c false if the two FormFiles are equal, \c true otherwise.
     */
    inline bool 
    operator!= (const FormFile& file) 			const
    { return ! operator==(file); }
    
#ifdef WIN32
    /* Dummy operator for MSVC++ */
    inline bool
    operator< (const FormFile& file) 			const
    { return false; }
#endif
    
    /*!
     * \brief Assign one FormFile to another.
     *
     * Sets the name, filename, datatype, and data to those of \c file
     * \param file The FormFile to copy.
     * \return A reference to this.
     */
    FormFile& 
    operator= (const FormFile& file);
    //@}
    
    // ============================================================
    
    /*! \name Accessor Methods 
     * Information on the uploaded file
     */
    //@{
    
    /*!
     * \brief Write this file data to the specified stream.
     *
     * This is useful for saving uploaded data to disk
     * \param out The ostream to which to write.
     */
    void 
    writeToStream(std::ostream& out) 			const;
    
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
     * \brief Get the basename of the file on the remote machine.
     *
     * The filename is stripped of all leading directory information
     * \return The basename of the file on the remote machine.
     */
    inline std::string
    getFilename() 					const
    { return fFilename; }
    
    /*!
     * \brief Get the MIME type of the file data.
     *
     * This will be of the form \c text/plain or \c image/jpeg
     * \return The MIME type of the file data.  
     */
    inline std::string 
    getDataType() 					const
    { return fDataType; }
    
    /*!
     * \brief Get the file data.  
     *
     * This returns the raw file data as a string
     * \return The file data.
     */
    inline std::string 
    getData() 					const
    { return fData; }
    
    /*!
     * \brief Get the length of the file data
     *
     * The length of the file data is usually measured in bytes.
     * \return The length of the file data, in bytes.
     */
    inline std::string::size_type
    getDataLength() 				const
    { return fData.length(); }
    //@}
    
  private:
    std::string 	fName;
    std::string 	fFilename;
    std::string 	fDataType;
    std::string 	fData;
  };
  
} // namespace cgicc

#endif /* ! _FORMFILE_H_ */
