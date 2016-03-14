CFLAGS=-Wall --std=c99
LDFLAGS=
CC=gcc
SRCDIR="../src"
LIBDIR="../lib"

SOURCES1= $(SRCDIR)/argpe_html.c $(SRCDIR)/argpe_html_api.c $(LIBDIR)/argpe_common.c prettify.c
SOURCES2= $(SRCDIR)/argpe_html.c $(SRCDIR)/argpe_html_api.c $(LIBDIR)/argpe_common.c printthree.c
SOURCES3= $(SRCDIR)/argpe_html.c $(SRCDIR)/argpe_html_api.c $(LIBDIR)/argpe_common.c find_tag.c
SOURCES4= $(SRCDIR)/argpe_html.c $(SRCDIR)/argpe_html_api.c $(LIBDIR)/argpe_common.c find_attribute.c
SOURCES5= $(SRCDIR)/argpe_html.c $(SRCDIR)/argpe_html_api.c $(LIBDIR)/argpe_common.c print_text.c

#OBJECTS=$(SOURCES:.c=.o)

all:
	$(CC) $(CFLAGS) $(SOURCES1) -o prettify
	$(CC) $(CFLAGS) $(SOURCES2) -o printthree
	$(CC) $(CFLAGS) $(SOURCES3) -o findtag
	$(CC) $(CFLAGS) $(SOURCES4) -o findattr
	$(CC) $(CFLAGS) $(SOURCES5) -o printtext
