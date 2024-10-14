/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: CgiUtils.h,v 1.17 2014/04/23 20:55:03 sebdiaz Exp $
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

#ifndef _CGIUTILS_H_
#define _CGIUTILS_H_ 1

#ifdef __GNUG__
#  pragma interface
#endif

/*! \file CgiUtils.h
 * \brief A collection of utility functions.
 *
 * These utility functions are used internally by cgicc to
 * decode posted form data, and to read/write from streams.
 */

#include <new>
#include <string>
#include <fstream>

#include "CgiDefs.h"


namespace cgicc {
  
  /*!
   * \brief Compare two strings for equality, ignoring case.
   *
   * For case-sensitive comparison, use (s1 == s2);
   * \param s1 The first string to compare
   * \param s2 The second string to compare
   * \return \c true if the strings are equal, \c false if they are not
   */
  CGICC_API bool
  stringsAreEqual(const std::string& s1, 
		  const std::string& s2);
  
  /*!
   * \brief Compare two strings for equality, ignoring case.
   *
   * For case-sensitive comparison, use (s1 == s2);
   * \param s1 The first string to compare
   * \param s2 The second string to compare
   * \param n The number of characters to compare.
   * \return \c true if the strings are equal, \c false if they are not
   */
  CGICC_API bool
  stringsAreEqual(const std::string& ss1, 
		  const std::string& ss2,
		  size_t n);
  
  /*!
   * \brief Convert encoded characters in form data to normal ASCII. 
   *
   * For example, "%21" is converted to '!' and '+' is converted to a space.
   * Normally, this is called internally to decode the query string or post 
   * data.
   * \param src The src string containing the encoded characters

   * \return The converted string
   */
  CGICC_API std::string
  form_urldecode(const std::string& src);

  /*!
   * \brief Convert an ASCII string to a URL-safe string.
   *
   * For example, '!' is converted to "%21" and ' ' is converted to '+'.
   * \param src The src string containing the characters to encode
   * \return The converted string
   */
  CGICC_API std::string
  form_urlencode(const std::string& src);
  

  /*!
   * \brief Convert an ASCII character to its hexadecimal equivalent.
   *
   * For example, after the call
   * \code
   * string s = charToHex(':');
   * \endcode
   * \c s will have a value of "3A".
   * Normally, this is called internally to encode characters by
   * escapeString.
   * \param c The character to encode
   * \return A string representing the hexadecimal value of c
   */
  CGICC_API std::string
  charToHex(char c);
  
  /*!
   * \brief Convert a hex-encoded character to its ASCII equivalent.
   *
   * For example, after the call
   * \code
   * char c = hexToChar('2', '1');
   * \endcode
   * \c c will have a value of '!'.
   * Normally, this is called internally to decode encoded characters in
   * the query string or post data.
   * \param first The first hex digit
   * \param second The second hex digit
   * \return The ASCII character
   */
  CGICC_API char
  hexToChar(char first,
	    char second);
  
  /*!
   * \brief Extract a substring contained within two separators.
   *
   * For example, after the call
   * \code
   * std::string data = "11foo22";
   * std::string res;
   * res = extractBetween(data, "11", "22");
   * \endcode
   * \c res will be "foo".
   * \param data The data to search.
   * \param separator1 The first logical separator.
   * \param separator2 The second logical separator.
   * \return The substring between the separators.
   */
  std::string
  extractBetween(const std::string& data, 
		 const std::string& separator1, 
		 const std::string& separator2);
  
  /*!
   * \brief Extract a substring contained between a separator.
   *
   * This function is used internally to decode \c multipart/form-data
   * \param data The data to search.
   * \param separator The separator.
   * \return The substring between the separator.
   */
  inline std::string
  extractBetween(const std::string& datas, 
		 const std::string& separators)
  { return extractBetween(datas, separators, separators); }
  
  /*!
   * \brief Write a string to an ostream.
   *
   * This function is used internally  for saving environments.
   * \param out The ostream to which to write.
   * \param s The string to write.
   */
  void 
  writeString(std::ostream& out, 
	      const std::string& s);
  
  /*!
   * \brief Write a long to an ostream.
   *
   * This function is used internally for saving environments.
   * \param out The ostream to which to write.
   * \param l The long to write.
   */
  void 
  writeLong(std::ostream& out, 
	    unsigned long l);
  
  /*!
   * \brief Read a string from an istream.
   *
   * This function is used internally by cgicc for restoring environments.
   * \param in The istream from which to read.
   * \return The string read.
   */
  std::string
  readString(std::istream& in);
  
  /*!
   * \brief Read a long from an istream.
   *
   * This function is used internally by cgicc for restoring environments.
   * \param in The istream from which to read.
   * \return The long read.
   */
  unsigned long
  readLong(std::istream& in);
  
} // namespace cgicc

#endif /* ! _CGIUTILS_H_ */
