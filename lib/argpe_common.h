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

 * File:	Argiope/lib/argpe_common.h
   Argiope Data Types and common function

------------------------------------------------------------------------ */

#ifndef __ARGPE_COMMON_H_
#define __ARGPE_COMMON_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* - Argiope Data Types - */

typedef	unsigned	argpe_uint;
typedef	int		argpe_int;
typedef	double		argpe_float;

#define	argpe_string	char*
extern const size_t argpe_char_size;

typedef	unsigned char	argpe_bool;
#define	True	1
#define	False	0

#define	argpe_save_free(X)\
	do { if((X) != NULL) {free(X); X=NULL;} } while(0)

#define	argpe_isspace(C)\
	(C == ' ' || C == '\r' || C == '\n' || C == '\t' || C == '\f' || C == '\v')

#define	argpe_isletter(C)\
	(C >= 'A' && C <= 'Z' || C >= 'a' && C <= 'z')

#define	argpe_strforeach(array, value)\
	for(value = *array; value = *array++, value != '\0';)

argpe_uint argpe_strlen (const argpe_string p);
argpe_string argpe_substr (const argpe_string p1, argpe_uint len);
argpe_string argpe_substr_space (const argpe_string p1, argpe_uint len);
argpe_bool argpe_strcmp (const argpe_string p1, const argpe_string p2);
argpe_bool argpe_strncmp (const argpe_string p1, const argpe_string p2, argpe_uint len);
argpe_int argpe_strcmp_int (const argpe_string p1, const argpe_string p2);

#endif /* __ARGPE_COMMON_H_ */
