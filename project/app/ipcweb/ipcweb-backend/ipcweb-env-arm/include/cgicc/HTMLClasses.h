/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLClasses.h,v 1.17 2014/04/23 20:55:05 sebdiaz Exp $
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

#ifndef _HTMLCLASSES_H_
#define _HTMLCLASSES_H_ 1

/*! \file HTMLClasses.h
 * \brief The header file containing HTML output classes.
 *
 * One class is defined for each element in the HTML 4.0 standard.
 */

#include "CgiDefs.h"
#include "HTMLAtomicElement.h"
#include "HTMLBooleanElement.h"
#include "HTMLDoctype.h"

// ============================================================
// Macros defining types of elements
// ============================================================

/*!
 * \brief Create an HTML element rendering class
 *
 * \param name The name of the class to define
 * \param tag The text to output when this tag is rendered
 */
#define TAG(name, tag) \
class name##Tag   \
{ public: inline static const char* getName() { return tag; } }

/*!
 * \brief Create an atomic HTML element
 *
 * Atomic HTML elements maintain no internal on/off state.  For
 * example, \c br and \c meta are atomic elements.
 * \param name The name of the class to define
 * \param tag The text to output when this tag is rendered
 */
#define ATOMIC_ELEMENT(name, tag) \
TAG(name, tag); typedef HTMLAtomicElement<name##Tag> name

/*!
 * \brief An HTML element maintaining an internal on/off state
 *
 * Boolean HTML elements maintain an internal state, and the output
 * rendered depends on the current state. For example, \c h1 and \c
 * title are boolean elements.
 * \param name The name of the class to define
 * \param tag The text to output when this tag is rendered
 */
#define BOOLEAN_ELEMENT(name, tag) \
TAG(name, tag); typedef HTMLBooleanElement<name##Tag> name


// ============================================================
// HTML 4.0 elements - for details see http://www.w3.org/
// ============================================================

namespace cgicc {

  // ============================================================
  // Class comment - needs special render function
  // ============================================================
  
  class nullTag
  { public: inline static const char* getName() { return 0; } };
  
  /*! \class comment HTMLClasses.h cgicc/HTMLClasses.h
   * \brief An HTML comment
   */
  class comment : public HTMLBooleanElement<nullTag>
  {
    virtual void render(std::ostream& out) 	const
    {
      if(getData().empty() && false == dataSpecified()) {
	swapState();
	out << (getState() ? "<!-- " : " -->");
      }
      else
	out << "<!-- " << getData() << " -->";
    }
  };
  
  BOOLEAN_ELEMENT (html,       "html");       // HTML document
  BOOLEAN_ELEMENT (head,       "head");       // document head
  BOOLEAN_ELEMENT (title,      "title");      // document title
  ATOMIC_ELEMENT  (meta,       "meta");       // meta data
  BOOLEAN_ELEMENT (style,      "style");      // style sheet
  BOOLEAN_ELEMENT (body,       "body");       // document body
  BOOLEAN_ELEMENT (div,        "div");        // block-level grouping
  BOOLEAN_ELEMENT (span,       "span");       // inline grouping
  BOOLEAN_ELEMENT (h1,         "h1");         // level 1 heading
  BOOLEAN_ELEMENT (h2,         "h2");         // level 2 heading
  BOOLEAN_ELEMENT (h3,         "h3");         // level 3 heading
  BOOLEAN_ELEMENT (h4,         "h4");         // level 4 heading
  BOOLEAN_ELEMENT (h5,         "h5");         // level 5 heading
  BOOLEAN_ELEMENT (h6,         "h6");         // level 6 heading
  BOOLEAN_ELEMENT (address,    "address");    // contact information
  
  // text markup
  
  BOOLEAN_ELEMENT (em,         "em");         // emphasis
  BOOLEAN_ELEMENT (strong,     "strong");     // stronger emphasis
  BOOLEAN_ELEMENT (cite,       "cite");       // citation/reference
  BOOLEAN_ELEMENT (dfn,        "dfn");        // defining instance
  BOOLEAN_ELEMENT (code,       "code");       // computer code
  BOOLEAN_ELEMENT (samp,       "samp");       // sample output
  BOOLEAN_ELEMENT (kbd,        "kbd");        // user input
  BOOLEAN_ELEMENT (var,        "var");        // variable/argument
  BOOLEAN_ELEMENT (abbr,       "abbr");       // abbreviated form
  BOOLEAN_ELEMENT (acronym,    "acronym");    // acronym
  BOOLEAN_ELEMENT (blockquote, "blockquote"); // block-level quotation
  BOOLEAN_ELEMENT (q,          "q");          // inline quotation
  BOOLEAN_ELEMENT (sub,        "sub");        // subscript
  BOOLEAN_ELEMENT (sup,        "sup");        // superscript
  BOOLEAN_ELEMENT (p,          "p");          // paragraph
  ATOMIC_ELEMENT  (br,         "br");         // line break
  BOOLEAN_ELEMENT (pre,        "pre");        // preformatted text
  BOOLEAN_ELEMENT (ins,        "ins");        // inserted text
  BOOLEAN_ELEMENT (del,        "del");        // deleted text
  BOOLEAN_ELEMENT (bdo,        "bdo");        // overriding direction
  
  // lists
  
  BOOLEAN_ELEMENT (ul,         "ul");         // unordered list
  BOOLEAN_ELEMENT (ol,         "ol");         // ordered list
  BOOLEAN_ELEMENT (li,         "li");         // list item 
  BOOLEAN_ELEMENT (dl,         "dl");         // definition list         
  BOOLEAN_ELEMENT (dt,         "dt");         // term to be defined
  BOOLEAN_ELEMENT (dd,         "dd");         // definition of term
  
  // tables
  
  BOOLEAN_ELEMENT (table,      "table");      // table element
  BOOLEAN_ELEMENT (caption,    "caption");    // table caption
  BOOLEAN_ELEMENT (thead,      "thead");      // table head section
  BOOLEAN_ELEMENT (tfoot,      "tfoot");      // table foot section
  BOOLEAN_ELEMENT (tbody,      "tbody");      // table body section
  BOOLEAN_ELEMENT (colgroup,   "colgroup");   // vertical section
  ATOMIC_ELEMENT  (col,        "col");        // column attributes
  BOOLEAN_ELEMENT (tr,         "tr");         // table row
  BOOLEAN_ELEMENT (th,         "th");         // table header cell
  BOOLEAN_ELEMENT (td,         "td");         // table data cell
  
  // links
  
  BOOLEAN_ELEMENT (a,          "a");          // anchor
  ATOMIC_ELEMENT  (link,       "link");       // document link
  ATOMIC_ELEMENT  (base,       "base");       // path information
  
  // objects
  
  ATOMIC_ELEMENT  (img,        "img");        // inline image
  BOOLEAN_ELEMENT (object,     "object");     // generic object
  ATOMIC_ELEMENT  (param,      "param");      // object parameters
  BOOLEAN_ELEMENT (map,        "map");        // client image map
  ATOMIC_ELEMENT  (area,       "area");       // image map region
  ATOMIC_ELEMENT  (hr,         "hr");         // horizontal rule
  
  // fonts - preferably use stylesheets
  
  BOOLEAN_ELEMENT (tt,         "tt");         // monospaced text
  BOOLEAN_ELEMENT (i,          "i");          // italic text style
  BOOLEAN_ELEMENT (b,          "b");          // bold text style
  BOOLEAN_ELEMENT (big,        "big");        // large font
  BOOLEAN_ELEMENT (small,      "small");      // small font
  
  // frames - not part of the strict DTD
  
  BOOLEAN_ELEMENT (frameset,   "frameset");   // frame layout
  ATOMIC_ELEMENT  (frame,      "frame");      // frame contents
  BOOLEAN_ELEMENT (noframes,   "noframes");   // alternative text
  BOOLEAN_ELEMENT (iframe,     "iframe");     // inline frame
  
  // forms
  
  BOOLEAN_ELEMENT (form,       "form");       // form element
  ATOMIC_ELEMENT  (input,      "input");      // generic input
  BOOLEAN_ELEMENT (button,     "button");     // special button
  BOOLEAN_ELEMENT (select,     "select");     // option menu
  BOOLEAN_ELEMENT (optgroup,   "optgroup");   // option group
  BOOLEAN_ELEMENT (option,     "option");     // option item
  BOOLEAN_ELEMENT (textarea,   "textarea");   // multi-line text input
  BOOLEAN_ELEMENT (label,      "label");      // input label
  BOOLEAN_ELEMENT (fieldset,   "fieldset");   // grouping input fields
  BOOLEAN_ELEMENT (legend,     "legend");     // caption for field set
  
  // scripts
  
  BOOLEAN_ELEMENT (script,     "script");     // script element
  BOOLEAN_ELEMENT (noscript,   "noscript");   // alternative text

} // namespace cgicc

#endif /* ! _HTMLCLASSES_H_ */
