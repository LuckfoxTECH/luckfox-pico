/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLElement.h,v 1.9 2014/04/23 20:55:06 sebdiaz Exp $
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

#ifndef _HTMLELEMENT_H_
#define _HTMLELEMENT_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file HTMLElement.h
 * \brief Class dealing with HTML elements
 * 
 * For example, \c a, \c img, \c html, and \c body, are all HTML elements.
 */

#include <string>

#include "CgiDefs.h"
#include "MStreamable.h"
#include "HTMLAttributeList.h"

namespace cgicc {

  class HTMLElementList;
  
  // ============================================================
  // Class HTMLElement
  // ============================================================
  
  /*! \class HTMLElement HTMLElement.h cgicc/HTMLElement.h
   * \brief Class representing an HTML element.
   *
   * An HTML element is any entity enclosed in angle brackets (\< and \>)
   * interpreted as HTML, for example \c a, \c img, \c html, and \c body.
   *
   * This class is an abstract base class that defines the interface
   * for all HTMLElement subclasses.
   */
  class CGICC_API HTMLElement : public MStreamable 
  {
  public:
    
    /*! 
     * \brief Possible types of HTMLElements 
     *
     * An HTMLElement is either atomic, meaning it has no corresponding
     * closing tag (elements such as \c hr and \c br are atomic) or
     * boolean (elements such as \c a and \c ol are boolean)
     */
    enum EElementType {
      /*! Atomic element, such as \c hr */
      eAtomic,
      /*! Boolean element, such as \c strong */
      eBoolean
    };
    
    // ============================================================
    
    /*! \name Constructors and Destructor */
    //@{
    
    /*!
     * \brief Copy constructor.
     *
     * Sets the name and internal state of this element to those of \c element
     * \param element The HTMLElement to copy.
     */
    HTMLElement(const HTMLElement& element);
    
    /*!
     * \brief Destructor 
     *
     * Delete this HTMLElement object
     */
    virtual ~HTMLElement();
    //@}
    
    // ============================================================
    
    /*! \name Overloaded Operators */
    //@{
    
    /*!
     * \brief Compare two HTMLElements for equality.
     *
     * HTMLElements are equal if they have the same name
     * \param element The HTMLElement to compare to this one.
     * \return \c true if the two HTMLElements are equal, \c false otherwise.
     */
    bool 
    operator== (const HTMLElement& element) 		const;
    
    /*!
     * \brief Compare two HTMLElements for inequality.
     *
     * HTMLElements are equal if they have the same name
     * \param element The HTMLElement to compare to this one.
     * \return \c false if the two HTMLElements are equal, \c true otherwise.
     */
    inline bool 
    operator!= (const HTMLElement& element) 		const
    { return ! operator==(element); }
    
#ifdef WIN32
    /* Dummy operator for MSVC++ */
    inline bool
    operator< (const HTMLElement& element)		const
    { return false; }
#endif
    
    /*!
     * \brief Assignment operator 
     *
     * Sets the name and internal state of this element to those of \c element
     * \param element The HTMLElement to copy
     * \return A reference to \c this
     */
    HTMLElement&
    operator= (const HTMLElement& element);
    //@}
    
    // ============================================================
    
    /*! \name Accessor Methods 
     * Information on the element
     */
    //@{
    
    /*!
     * \brief Get the name of this element.
     *
     * For example, \c html or \c body.
     * \return The name of this element.
     */
    virtual const char*
    getName() 					const = 0;
    
    /*!
     * \brief Get the data contained in this element, if any.
     *
     * This is only applicable for boolean elements
     * \return The data contained in this element.
     */
    inline std::string
    getData()  					const
    { return fData; }
    
    /*!
     * \brief Get the type of this element 
     *
     * Most HTMLElements are boolean
     * \return The type of this element
     */
    inline EElementType
    getType() 					const
    { return fType; }
    //@}
    
    // ============================================================
    
    /*! \name Mutator Methods 
     * Set properties of the element
     */
    //@{
    
    /*!
     * \brief Set the data contained in this element.
     *
     * The data is the text contained between the opening and closing tags
     * \param data The data to store in this element.
     */
    inline void
    setData(const std::string& data)
    { fData = data; }
    //@}
    
    // ============================================================
    
    /*!
     * \brief Clone this HTMLElement 
     *
     * This performs a deep copy of the element
     * \return A pointer to a newly-allocated copy of \c this.
     */
    virtual HTMLElement*
    clone() 					const = 0;
    
    // ============================================================
    
    /*! \name Embedded HTMLElement Management 
     * Manage elements embedded in this one
     */
    //@{
    
    /*!
     * \brief Get the HTMLElementList embedded in this element, if any.
     *
     * If this method returns 0, no elements are embedded
     * \return The embedded element list.
     */
    inline const HTMLElementList*
    getEmbedded() 				const
    { return fEmbedded; }
    
    /*!
     * \brief Set the HTMLElementList associated with this element.
     *
     * This is usually called by subclass constructors
     * \param embedded The HTMLElementList containing the HTMLElements
     * embedded in this element.
     */
    void 
    setEmbedded(const HTMLElementList& embedded);
    
    /*!
     * \brief Add an embedded HTMLElement in this one 
     *
     * 
     * \param element A reference to an HTMLElement to embed in this one
     * \return A reference to \c this
     */
    HTMLElement&
    add(const HTMLElement& element);
    
    /*!
     * \brief Add an embedded HTMLElement in this one 
     *
     * This element takes ownership of \c element, which should not be deleted.
     * \param element A pointer to an HTMLElement to embed.
     * \return A reference to \c this
     */
    HTMLElement&
    add(HTMLElement *element);
    //@}
    
    // ============================================================
    
    /*! \name HTMLAttribute Management 
     * Manage attributes embedded in this element
     */
    //@{
    
    /*!
     * \brief Get the attributes associated with this element.
     *
     * If this method returns 0, no attributes are embedded
     * \return The attribute list.
     */
    inline const HTMLAttributeList*
    getAttributes() 				const
    { return fAttributes; }
    
    /*!
     * \brief Set the attributes associated with this element.
     *
     * This is usually called by subclass constructors
     * \param attributes The HTMLAttributeList containing the HTMLAttributes 
     * belonging to this element.
     */
    void 
    setAttributes(const HTMLAttributeList& attributes);
    
    /*!
     * \brief Set an HTMLAttribute on this HTMLElement 
     *
     * 
     * \param name The name of the HTMLAttribute to set
     * \return A reference to \c this
     */
    HTMLElement&
    set(const std::string& name);
    
    /*!
     * \brief Set an HTMLAttribute on this HTMLElement 
     *
     *
     * \param name The name of the HTMLAttribute
     * \param value The value of the HTMLAttribute
     * \return A reference to \c this
     */
    HTMLElement&
    set(const std::string& name,
	const std::string& value);
    //@}
    
    // ============================================================
    
    /*! \name Boolean element methods 
     * Methods specific to boolean elements
     */
    //@{
    
    /*! 
     * \brief Swap the state of the boolean element 
     *
     * A state of \c true means the element is active
     */
    virtual void 
    swapState() 				const
    {}
    
    /*!
     * \brief Get the state of this boolean element 
     *
     * 
     * \return \c true if the element is active, \c false otherwise
     */
    virtual bool 
    getState() 					const
    { return false; }
    //@}
    
    /*!
     * \brief Render this HTMLElement to an ostream
     *
     * This is used for output
     * \param out The ostream to which to write
     */
    virtual void 
    render(std::ostream& out) 			const;
    
  protected:
    
    /*!
     * \brief Subclass constructor
     *
     * This allows the subclasses to fully specify all properties
     * \param attributes A pointer to an HTMLAttributeList containing the
     * HTMLAttributes for this HTMLElement, if any
     * \param embedded A pointer to the embedded HTMLElement, if any
     * \param data A pointer to the data, if any
     * \param type The type of element
     */
    HTMLElement(const HTMLAttributeList *attributes,
		const HTMLElement *embedded,
		const std::string *data,
		EElementType type);
    
    /*!
     * \brief For subclasses only
     *
     * Returns \c true if data was specified in the constructor.
     * \return \c true if data was specified in the constructor, \c false otherwise
     */
    inline bool
    dataSpecified() 				const
    { return fDataSpecified; }
    
  private:
    HTMLElement() {}
    
    HTMLAttributeList 	*fAttributes;
    HTMLElementList 	*fEmbedded;
    std::string 	fData;
    EElementType 	fType;
    bool 		fDataSpecified;
  };
  
} // namespace cgicc

#endif /* ! _HTMLELEMENT_H_ */
