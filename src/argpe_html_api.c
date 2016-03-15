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


void print_sort_text_algorithm (html_element root, html_element node, argpe_uint sort)
{
	html_element	sibling;
	html_text	text;

	if(!node)
		return;
	while (node) {
		sibling = node->sibling;

		text = node->text;
		while (text != NULL) {
			if (sort == text->sort){
				if (!argpe_strcmp(node->tag, "script"))
					fprintf(stderr,"%s ", text->text);
				print_sort_text_algorithm (root, root, sort + 1);
				return;
			}
			text = text->next;
		}

		print_sort_text_algorithm (root, node->child, sort);
		node = sibling;
	}
	return;
}

void print_sort_text (html_element node)
{
	print_sort_text_algorithm (node, node, 0);
}

void print_text (html_element node)
{
	html_element	sibling;
	html_text	text;

	if(!node)
		return;
	while (node) {
		sibling = node->sibling;

		if (!argpe_strcmp(node->tag, "script")) {
			text = node->text;
			while (text != NULL) {
				printf("%s ", text->text);
				text = text->next;
			}
		}

		print_text(node->child);

		node = sibling;
	}
	return;
}

void print_element(html_element node)
{
	html_attribute	attr;
	html_text	text;

	if (!node){
		printf("element: Not Found\n");
		return;
	}

	printf("element: %s", node->tag);
	printf("\n");

	text = node->text;
	while (text != NULL) {
		printf("text: %s\n", text->text);
		text = text->next;
	}

	attr = node->attribute;
	while (attr != NULL) {
		printf("attribute: %s=", attr->key_name);
		if (attr->value) 
			printf("\"%s\"", attr->value);
		else 
			printf("\"\"");
		printf("\n");
		attr = attr->next;
	}
	return;
}

html_element find_tag_recursive (html_element node, argpe_string foundtag)
{
	argpe_bool	found = False;
	html_element	foundnode = NULL;
	html_element	sibling;

	if(!node)
		return foundnode;
	while (node) {
		sibling = node->sibling;

		if (argpe_strcmp(node->tag, foundtag)) {
			foundnode = node;
			found = True;
			return foundnode;
		}

		if (!found)
			foundnode = find_tag_recursive (node->child, foundtag);

		node = sibling;
	}
	return foundnode;
}

html_element find_tag (html_element node, argpe_string tag)
{
	html_element found;
	found = find_tag_recursive(node->child, tag);
	return found;
}


html_element find_attr_recursive (html_element node, argpe_string foundattr)
{
	argpe_bool	found = False;
	html_element	foundnode = NULL;
	html_attribute	attr;
	html_element	sibling;

	if(!node)
		return foundnode;
	while (node) {
		sibling = node->sibling;

		attr = node->attribute;
		while (attr != NULL) {
			if (argpe_strcmp(attr->key_name, foundattr)) {
				foundnode = node;
				found = True;
				return foundnode;			
			}
			attr = attr->next;
		}

		if (!found)
			foundnode = find_attr_recursive (node->child, foundattr);

		node = sibling;
	}
	return foundnode;
}

html_element find_attr (html_element node, argpe_string attr)
{
	html_element found;
	found = find_attr_recursive(node->child, attr);
	return found;
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
			printf("text: \"%s\" sort: %u\n", text->text, text->sort);
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


