/* ------------------------------------------------------------------------

 Project
	   __    ____   ___  ____  _____  ____  ____ 
	  /__\  (  _ \ / __)(_  _)(  _  )(  _ \( ___)
	 /(__)\  )   /( (_-. _)(_  )(_)(  )___/ )__) 
	(__)(__)(_)\_) \___/(____)(_____)(__)  (____)
	             |
	             |
	       /     |     \
	       \     |     /'
	      \ \__ /_\ __/ /
	       \___|)_(|___/
	        ___|)_(|___
	       /  /\oOo/\  \`
	       \ /' ;":  \ /
	         \  ` `  /

Copyright (c) 2016, Emilio Schininà. All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution. 
 * All advertising materials mentioning features or use of this software must 
   display the following acknowledgement: This product includes software 
   developed by Emilio Schininà. 
 * Neither the name of author may be used to endorse or promote products
   derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

Written by Emilio Schininà <emilioschi@gmail.com>, March 2016

 * File:	Argiope/parser/src/argpe_html.h
   This file contains machine dependent code for html parsing

------------------------------------------------------------------------ */

#ifndef __ARGPE_HTML_H_
#define __ARGPE_HTML_H_

#include "../lib/argpe_common.h"


#define	ARGPE_PARSER_BSIZE	128

#define	argpe_parser_balloc(BUFFER, PLUS)				\
	do {								\
		BUFFER = (argpe_string)realloc(BUFFER,			\
		 (PLUS + ARGPE_PARSER_BSIZE + 1) * argpe_char_size);	\
		if (BUFFER == NULL) {					\
			fprintf(stderr, "[PARSER] Memory Error,\n");	\
			return NULL; /* TODO: Change it */		\
		}							\
		BUFFER[PLUS + ARGPE_PARSER_BSIZE] = 0;			\
	} while (0)

typedef enum parser_state parser_state;
enum parser_state {
	STATE_DATA,
	STATE_TAG_OPEN,
	STATE_TAG_NAME,
	STATE_CLOSE_TAG_OPEN,
	STATE_BEFORE_ATTRIBUTE_NAME,
	STATE_ATTRIBUTE_NAME,
	STATE_BEFORE_ATTRIBUTE_VALUE,
	STATE_AFTER_ATTRIBUTE_NAME,
	STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED,
	STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED,
	STATE_ATTRIBUTE_VALUE_UNQUOTED,
	STATE_AFTER_ATTRIBUTE_VALUE,
	STATE_SELF_CLOSING_START_TAG1,
	STATE_SELF_CLOSING_START_TAG2,
	STATE_MARKUP_DECLARATION_OPEN,
	STATE_ENTITY_DATA,
	STATE_BOGUS_COMMENT,
	STATE_COMMENT_START,
	STATE_COMMENT_START_DASH,
	STATE_COMMENT,
	STATE_COMMENT_END_DASH,
	STATE_COMMENT_END,
	STATE_COMMENT_END_BANG,
	STATE_DOCTYPE,
	STATE_BEFORE_DOCTYPE_NAME,
	STATE_CDATA_SECTION,
};

typedef struct html_attrib_s html_attrib_t;
typedef	html_attrib_t*	html_attribute;
struct html_attrib_s {
	argpe_string	key_name;
	argpe_string	value;

	html_attribute	next;
};


typedef struct html_text_s html_text_t;
typedef	html_text_t*	html_text;
struct html_text_s {
	argpe_string	text;

	html_text	next;
};


typedef	struct html_element_s	html_element_t;
typedef	html_element_t*	html_element;
typedef	html_element	html_doc;

struct html_element_s {
	argpe_string	tag;
	html_text	text;
	html_attribute	attribute;

	/* relations */
	html_element	parent;
	html_element	child;
	html_element	sibling;
};

html_element init_html_three();
html_element argpe_parser_html (FILE*);
const argpe_string argpe_html_tokenizer (const argpe_string, const argpe_string);
void html_free_three_recursive (html_element);
void argpe_html_close (html_element);

void close_html_tag (html_element, argpe_string);
void set_html_tag(argpe_string);
void set_html_text(argpe_string);
void set_html_attr(argpe_string);
void set_html_value(argpe_string);


/* API */
void argpe_html_print_three(html_element);
void html_print_three_recursive(html_element, argpe_uint);

void argpe_html_prettify(html_element);
void html_print_recursive(html_element, argpe_uint);

void print_element(html_element);

html_element find_tag_recursive (html_element, argpe_string);
html_element find_tag (html_element, argpe_string);

html_element find_attr_recursive (html_element, argpe_string);
html_element find_attr (html_element, argpe_string);


#endif /* __ARGPE_HTML_H_ */
