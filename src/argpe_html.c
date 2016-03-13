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

 * File:	Argiope/parser/src/argpe_html.c
   This file contains machine dependent code for html parsing

------------------------------------------------------------------------ */

#include "argpe_html.h"


argpe_uint	relation;
#define	ELEMENT_SIBLING	1
#define	ELEMENT_CHILD	2

html_element	parser_root;
html_element	parser_node;
html_attribute	parser_attr;

static parser_state PARSER_STATE = STATE_DATA;
int comment = 0;
int doctype = 0;
int cdata = 0;

argpe_uint parser_strlen = 0;
argpe_uint parser_newline = 0;
argpe_uint parser_spacelen = 0; 
argpe_string parser_str = NULL;


html_element argpe_parser_html (FILE *fd)
{
	parser_node = init_html_three();
	/* Buffer plus memory variables */
	size_t			argpe_parser_bplusmemory = 0;
	unsigned		plus_flag		 = 0;

	size_t			read_bplusmemory	 = 0;

	size_t			read			 = 0;
	size_t			not_overwrite		 = 0;
	size_t			len_read		 = 0;

	argpe_string		buffer			 = NULL;
	argpe_parser_balloc(buffer, argpe_parser_bplusmemory);

	size_t			token_miss		 = ARGPE_PARSER_BSIZE;
	const argpe_string	token			 = buffer;

	while (!feof(fd)) {
		if (token_miss > 0) {
			plus_flag = 0;
		} else if (token_miss == 0){
			plus_flag = 1;
			argpe_parser_bplusmemory++;
			argpe_parser_balloc(buffer, argpe_parser_bplusmemory);
		} else {
			fprintf(stderr, "[PARSER] Error: token corrupt.\n");
			return NULL;
		}

		read		 = plus_flag + token_miss;
		not_overwrite	 = ARGPE_PARSER_BSIZE + read_bplusmemory - token_miss;

		len_read = fread(buffer + (not_overwrite), 1, read, fd);
		buffer[not_overwrite + len_read] = 0;

		token = argpe_html_tokenizer(buffer + not_overwrite, buffer);

		token_miss	 = (token - buffer);
		not_overwrite	 = ARGPE_PARSER_BSIZE + argpe_parser_bplusmemory - token_miss;

		memmove(buffer, token, not_overwrite);

		read_bplusmemory = argpe_parser_bplusmemory;
	}
	if(parser_strlen > 0){
		argpe_string end = argpe_substr_space(buffer, argpe_strlen(buffer));
		set_html_text(end);
		argpe_save_free(end);
	}

	free(buffer);

	return parser_root;
}


html_element init_html_three () /* DUMMY NODE */
{
	html_element node	 = (html_element)malloc(sizeof(html_element_t));

	node->tag		 = "root";

	node->text		 = NULL;
	node->attribute		 = NULL;

	node->parent		 = NULL;
	node->child		 = NULL;
	node->sibling		 = NULL;

	relation		 = ELEMENT_CHILD;

	parser_root 		 = node;

	return node;
}

void close_html_tag (html_element node, argpe_string tag)
{
	html_element sibling;

	if(!node)
		return;
	while (node) {
		sibling = node->sibling;
		if (argpe_strcmp(tag, node->tag))
			parser_node = node;
		close_html_tag (node->child, tag);
		node = sibling;
	}
	return;
}

void set_html_tag(argpe_string tag)
{
	if (argpe_strncmp("/*", tag, 1)) { // TODO: Fix later
		tag = tag + 1; // <----------- if (tag[0] == 0) relation = ELEMENT_SIBLING;
		close_html_tag(parser_root->child, tag);
		// if (False) { relation == ELEMENT_CHILD } else
		relation = ELEMENT_SIBLING;
	}
	else {
		html_element node = (html_element)malloc(sizeof(html_element_t));
		node->tag = argpe_substr(tag, argpe_strlen(tag));
		node->text = NULL;
		node->attribute = NULL;

		node->parent = NULL;
		node->child = NULL;
		node->sibling = NULL;

		if (relation == ELEMENT_CHILD) {
			parser_node->child = node;
			node->parent = parser_node;
			parser_node = parser_node->child;
		}
		else if (relation == ELEMENT_SIBLING) {
			parser_node->sibling = node;
			node->parent = parser_node->parent;
			parser_node = parser_node->sibling;
			relation = ELEMENT_CHILD;	
		}
	}
}

void set_html_text(argpe_string text)
{
	html_text node1;
	html_text node2 = (html_text)malloc(sizeof(html_text_t));
	node2->text = argpe_substr(text, argpe_strlen(text));
	node2->next = NULL;

	if (relation == ELEMENT_CHILD) {
		node1 = parser_node->text;
	} 
	else /*(relation == ELEMENT_SIBLING)*/ {
		node1 = parser_node->parent->text;
	}

	if (node1 == NULL){
		parser_node->text = node2;
	}
	else {
		while (node1->next != NULL) {
			node1 =  node1->next;
		}
		node1->next = node2;
	}
	return;

}

void set_html_attr(argpe_string attr)
{
	if (relation == ELEMENT_SIBLING) return;
	html_attribute node1 = parser_node->attribute;
	html_attribute node2 = (html_attribute)malloc(sizeof(html_attrib_t));
	node2->key_name = argpe_substr(attr, argpe_strlen(attr));
	node2->value = NULL;
	node2->next = NULL;

	if (node1 == NULL){
		parser_node->attribute = node2;
	}
	else {
		while (node1->next != NULL) {
			node1 =  node1->next;
		}
		node1->next = node2;
	}
	parser_attr = node2;
	return;
}

void set_html_value(argpe_string value)
{
	parser_attr->value = argpe_substr(value, argpe_strlen(value));
}

void html_free_three_recursive (html_element node)
{
	html_element	sibling;
	html_element	freesibling;
	html_attribute	attr;
	html_attribute	freeattr;
	html_text	text;
	html_text	freetext;


	if(!node)
		return;
	while (node) {
		sibling = node->sibling;
		argpe_save_free(node->tag);
		text = node->text;
		while (text != NULL) {
			argpe_save_free(text->text);
			freetext = text;
			text = text->next;
			argpe_save_free(freetext);
		}

		attr = node->attribute;
		while (attr != NULL) {
			argpe_save_free(attr->key_name);
			if (attr->value)
				argpe_save_free(attr->value);
			freeattr = attr;
			attr = attr->next;
			argpe_save_free(freeattr);
		}

		html_free_three_recursive (node->child);

		freesibling = sibling;
		node = sibling;
		argpe_save_free(freesibling);
	}
	argpe_save_free(node);
	return;
}

void argpe_html_close (html_element node)
{
	html_free_three_recursive(node->child);
	argpe_save_free(node);
}

#define	SET_HTML_TOKEN_OFFSET	{token = stream; parser_strlen = 0;}

/* TODO: SCRIPT SECTION; CDATA SECTION; HTML ENTITY CONVERTION */
const argpe_string 
argpe_html_tokenizer (const argpe_string stream, const argpe_string token)
{
	char data;

	argpe_strforeach (stream, data) {

		switch (PARSER_STATE) {
		case STATE_DATA:
			/*if (data == '&') { //TODO: Convertire l'entità
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_ENTITY_DATA;
			}
			else */
			if (data == '<') {
				if (parser_strlen > 0) {
					parser_str = argpe_substr_space(token,
					 parser_strlen + parser_spacelen + parser_newline);
					//printf(" TEXT: '%s':%d\n", parser_str, parser_strlen);
					set_html_text(parser_str);
					parser_spacelen = 0;
					parser_newline = 0;
				}
					parser_strlen++;
					parser_spacelen = 0;
					parser_newline = 0;
					PARSER_STATE = STATE_TAG_OPEN;
			}
			else if (data == '\r' || data == '\n' || data == '\f' ){
				parser_newline++;
			}
			else if (data == ' ' || data == '\t' || data == '\v'){
				parser_spacelen++;
			}
			else{
				parser_strlen++;
			}
			break;
		case STATE_TAG_OPEN:
			if (data == '!') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_MARKUP_DECLARATION_OPEN;
			}
			else if (data == '/') {
				parser_strlen = 1;
				token = stream - 1;
				PARSER_STATE = STATE_CLOSE_TAG_OPEN;
			}
			else if (argpe_isletter(data)){
				parser_strlen = 1;
				token = stream - 1;
				PARSER_STATE = STATE_TAG_NAME;
			}
			else if (data == '>') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '?') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BOGUS_COMMENT;
			}
			else{
				parser_strlen++;
				PARSER_STATE = STATE_DATA;
			}
			break;
		case STATE_TAG_NAME:
			if (argpe_isspace(data)) {
				if (parser_strlen > 0){
					parser_str = argpe_substr(token, parser_strlen);
					//printf("  TAG: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_tag(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			else if (data == '>') {
				if (parser_strlen > 0){
					parser_str = argpe_substr(token, parser_strlen);
					//printf("  TAG: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_tag(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '/') {
				parser_strlen++;
				PARSER_STATE = STATE_SELF_CLOSING_START_TAG1;
			}
			else {
				parser_strlen++;
			}
			break;
		case STATE_CLOSE_TAG_OPEN:
			if (argpe_isletter(data)) {
				parser_strlen++;
				PARSER_STATE = STATE_TAG_NAME;
			}
			else if (data == '>') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else {
				/* data can be _'_... */
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BOGUS_COMMENT;
			}

			break;
		case STATE_BEFORE_ATTRIBUTE_NAME:
			if (argpe_isspace(data)) {
				SET_HTML_TOKEN_OFFSET;
			}
			else if (argpe_isletter(data)) {
				parser_strlen++;
				PARSER_STATE = STATE_ATTRIBUTE_NAME;
			}
			else if (data == '>') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '/') {
				parser_strlen++;
				PARSER_STATE = STATE_SELF_CLOSING_START_TAG1;
			}
			else if (data == '\'' || data == '"' ||
				 data == '=' || data == '<') {
				PARSER_STATE = STATE_ATTRIBUTE_NAME;
			}
			else {
				parser_strlen++;
				PARSER_STATE = STATE_ATTRIBUTE_NAME;
			}
			break;
		case STATE_ATTRIBUTE_NAME:
			if (data == '=') {
				parser_str = argpe_substr(token, parser_strlen);
				//printf(" ATTR: '%s':%d\n", parser_str, parser_strlen); // DEBUG
				set_html_attr(parser_str);
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_VALUE;
			}
			else if (argpe_isletter(data)) {
				parser_strlen++;
			}
			else if (data == '>') {
				parser_str = argpe_substr(token, parser_strlen);
				//printf(" ATTR: '%s':%d\n", parser_str, parser_strlen); // DEBUG
				set_html_attr(parser_str);
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (argpe_isspace(data)) {
				parser_str = argpe_substr(token, parser_strlen);
				//printf(" ATTR: '%s':%d\n", parser_str, parser_strlen); // DEBUG
				set_html_attr(parser_str);
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_AFTER_ATTRIBUTE_NAME;
			}
			else if (data == '/') {
				parser_strlen++;
				PARSER_STATE = STATE_SELF_CLOSING_START_TAG2;
			}
			else {
				parser_strlen++;
			}
			break;
		case STATE_BEFORE_ATTRIBUTE_VALUE:
			if (argpe_isspace(data)) {
				SET_HTML_TOKEN_OFFSET;
			}
			else if (data == '"') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED;
			}
			else if (data == '&') {
				//TODO: traduzione della entità. Serve?? <------
				parser_strlen++;
				PARSER_STATE = STATE_ATTRIBUTE_VALUE_UNQUOTED;
			}
			else if (data == '\'') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED;
			}
			else if (data == '>') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '=' || data == '<' || data == '`') {
				parser_strlen++;
				PARSER_STATE = STATE_ATTRIBUTE_VALUE_UNQUOTED;
			} else {
				parser_strlen++;
				PARSER_STATE = STATE_ATTRIBUTE_VALUE_UNQUOTED;
			}
			break;
		case STATE_AFTER_ATTRIBUTE_NAME:
			if (argpe_isspace(data)) {
				SET_HTML_TOKEN_OFFSET;
			}
			else if (data == '=') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_VALUE;
			}
			else if (data == '>') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (argpe_isletter(data)) {
				parser_strlen++;
				PARSER_STATE = STATE_ATTRIBUTE_NAME;
			}
			else if (data == '/') {
				parser_strlen++;
				PARSER_STATE = STATE_SELF_CLOSING_START_TAG1;
			}
			else if (data == '\'' || data == '"' || data == '<') {
				/* invalid-character-after-attribute-name */
				//parser_strlen++;
				SET_HTML_TOKEN_OFFSET; // <-------------- CHECK THIS ?!
				PARSER_STATE = STATE_ATTRIBUTE_NAME;
			} else {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_ATTRIBUTE_NAME;
			}
			break;
		case STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED:
			if (data == '"'){
				if (parser_strlen > 0){
					parser_str = argpe_substr_space(token, parser_strlen);
					//printf("VALUE: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_value(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_AFTER_ATTRIBUTE_VALUE;
			}
			/*else if (data == '&') {
				//TODO: traduzione della entità. Serve?!
			}*/
			else {
				parser_strlen++;
			}

			break;
		case STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED:
			if (data == '\'') {
				if (parser_strlen > 0){
					parser_str = argpe_substr_space(token, parser_strlen);
					//printf("VALUE: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_value(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_AFTER_ATTRIBUTE_VALUE;	
			}
			/*else if (data == '&') {
				//TODO: traduzione della entità. Serve?!
			}*/
			else {
				parser_strlen++;
			}
			break;
		case STATE_ATTRIBUTE_VALUE_UNQUOTED:
			if (argpe_isspace(data)){
				if (parser_strlen > 0){
					parser_str = argpe_substr_space(token, parser_strlen);
					//printf("VALUE: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_value(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			else if (data == '>') {
				if (parser_strlen > 0){
					parser_str = argpe_substr_space(token, parser_strlen);
					//printf("VALUE: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_value(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '=' || data == '<' || data == '`' ||
				 data == '"' || data == '\'' ) {
				parser_strlen++;
			} else {
				parser_strlen++;				
			}
			break;
		case STATE_AFTER_ATTRIBUTE_VALUE:
			if (argpe_isspace(data)) {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			else if (data == '>') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '/') {
				parser_strlen++;
				PARSER_STATE = STATE_SELF_CLOSING_START_TAG1;
			}
			else {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			break;
		case STATE_SELF_CLOSING_START_TAG1:
			if (data == '>') {
				if (parser_strlen > 0){
					parser_str = argpe_substr(token, parser_strlen);
					//printf("  TAG: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_tag(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (argpe_isspace(data)) {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			else { /* unexpected-character-after-solidus-in-tag */
				if (parser_strlen - 1 > 0){
					parser_str = argpe_substr(token, parser_strlen - 1);
					//printf("SAVETAG: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_tag(parser_str);
					token = token + parser_strlen;
					parser_strlen = 1;
				}
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			break;
		case STATE_SELF_CLOSING_START_TAG2:
			if (data == '>') {
				if (parser_strlen > 0){
					parser_str = argpe_substr(token, parser_strlen);
					//printf(" ATTR: '%s':%d\n", parser_str, parser_strlen); // DEBUG
					set_html_attr(parser_str);
				}
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (argpe_isspace(data)) {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			else { /* unexpected-character-after-solidus-in-attr */
				parser_strlen++;
				PARSER_STATE = STATE_BEFORE_ATTRIBUTE_NAME;
			}
			break;
		case STATE_MARKUP_DECLARATION_OPEN:
			if (data == '-') {
				SET_HTML_TOKEN_OFFSET;
				doctype = 0;
				cdata = 0;
				comment++;
				if (comment == 2) {
					PARSER_STATE = STATE_COMMENT_START;
					comment = 0;
				}

			}
			else if (data == 'd' || data == 'D' ||
				 data == 'o' || data == 'O' ||
				 data == 'c' || data == 'C' ||
				 data == 't' || data == 'T' ||
				 data == 'y' || data == 'Y' ||
				 data == 'p' || data == 'P' ||
				 data == 'e' || data == 'E') {
				doctype++;
				parser_strlen++;
				if (doctype == 7) {
					SET_HTML_TOKEN_OFFSET;
					PARSER_STATE = STATE_DOCTYPE;
					doctype = 0;
				}
			}
			else if (data == '[' || data == 'C' ||
				 data == 'D' || data == 'A' ||
				 data == 'T') { /* [CDATA */
				cdata++;
				parser_strlen++;
				/* TODO: FIX same character (CDATA, DOCTYPE) */
				if (cdata == 3) { /* 1 time '['; 2 times 'A';  */
					SET_HTML_TOKEN_OFFSET;
					PARSER_STATE = STATE_CDATA_SECTION;
					cdata = 0;
				}
			}
			else {
				parser_strlen++;
				comment = 0;
				doctype = 0;
				cdata = 0;
				PARSER_STATE = STATE_DATA;
			}
			break;
		case STATE_ENTITY_DATA:
			if (argpe_isspace(data))
				PARSER_STATE = STATE_DATA;
			else if (data == '<') {
				PARSER_STATE = STATE_DATA;
			}
			break;
		case STATE_BOGUS_COMMENT:
			SET_HTML_TOKEN_OFFSET;
			if (data == '>')
				PARSER_STATE = STATE_DATA;
			break;
		case STATE_COMMENT_START:
			if (data == '-'){
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_COMMENT_START_DASH;
			}
			else {
				SET_HTML_TOKEN_OFFSET;
			}

			break;
		case STATE_COMMENT_START_DASH:
			if (data == '-'){
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_COMMENT_END;
			} else {
				SET_HTML_TOKEN_OFFSET;
			}
			break;
		case STATE_COMMENT:
			if (data == '-'){
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_COMMENT_END_DASH;
			} else {
				SET_HTML_TOKEN_OFFSET;
			}
			break;
		case STATE_COMMENT_END_DASH:
			if (data == '-'){
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_COMMENT_END;
			} else {
				SET_HTML_TOKEN_OFFSET;
			}
			break;
		case STATE_COMMENT_END:
			if (data == '>'){
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '!') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_COMMENT_END_BANG;
			}
			break;
		case STATE_COMMENT_END_BANG:
			if (data == '>'){
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else if (data == '!') {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_COMMENT_END_BANG;
			} else {
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_COMMENT;
			}
			break;
		case STATE_DOCTYPE:
			if (argpe_isspace(data)){
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_DOCTYPE_NAME;
			}
			else {
				//errore
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_BEFORE_DOCTYPE_NAME;				
			}
			break;
		case STATE_BEFORE_DOCTYPE_NAME:
			if (argpe_isspace(data)) {
				SET_HTML_TOKEN_OFFSET;
			}
			else if (data == '>') {
				parser_str = argpe_substr(token, parser_strlen);
				//printf(" DOC: '%s':%d\n", parser_str, parser_strlen); // DEBUG
				SET_HTML_TOKEN_OFFSET;
				PARSER_STATE = STATE_DATA;
			}
			else {
				parser_strlen++;
			}
			break;
		case STATE_CDATA_SECTION:
			if (data == ']' || cdata > 0) {
				if (data == '>'){
					if (cdata == 1)
						PARSER_STATE = STATE_DATA;
					cdata = 0;
				}
				cdata++;
			}
		}
		argpe_save_free(parser_str);
	}
	return token;
}
