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

 * File:	Argiope/parser/src/argpe_html_api.c
   This file contains api calls for html parsing

------------------------------------------------------------------------ */

#include "argpe_html.h"

void find_tag()
{

}

void find_attr()
{

}

void html_print_three_recursive (html_element node, argpe_uint level)
{
	html_element	sibling;
	html_attribute	attr;
	html_text	text;

	if(!node)
		return;
	while (node) {
		sibling = node->sibling;

		for(int i = 0; i < level; i++)
			printf("\t");
		printf("element: %s", node->tag);
		printf("\n");

		text = node->text;
		while (text != NULL) {
			for(int i = 0; i < level; i++)
				printf("\t");
			printf("text: %s\n", text->text);
			text = text->next;
		}

		attr = node->attribute;
		while (attr != NULL) {
			for(int i = 0; i < level; i++)
				printf("\t");
			printf("attribute: %s=", attr->key_name);
			if (attr->value) 
				printf("\"%s\"", attr->value);
			else 
				printf("\"\"");
			printf("\n");
			attr = attr->next;
		}

		html_print_three_recursive (node->child, level + 1);

		node = sibling;
	}
	return;
}

void argpe_html_print_three (html_element node)
{
	html_print_three_recursive(node->child, 0);
}


void html_print_recursive (html_element node, argpe_uint level)
{
	html_element	sibling;
	html_attribute	attr;
	html_text	text;

	if(!node)
		return;
	while (node) {
		sibling = node->sibling;

		for(int i = 0; i < level; i++)
			printf("\t");
		printf("<%s", node->tag);

		attr = node->attribute;
		while (attr != NULL) {
			printf(" %s=", attr->key_name);
			if (attr->value) 
				printf("\"%s\"", attr->value);
			else 
				printf("\"\"");

			attr = attr->next;
		}

		printf(">\n");


		text = node->text;
		while (text != NULL) {
			for(int i = 0; i < level+1; i++)
				printf("\t");
			printf("%s\n", text->text);
			text = text->next;
		}

		html_print_recursive (node->child, level + 1);

		for(int i = 0; i < level; i++)
			printf("\t");
		printf("</%s>\n", node->tag);

		node = sibling;
	}
	return;
}

void argpe_html_prettify (html_element node)
{
	html_print_recursive(node->child, 0);
}


