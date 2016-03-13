CFLAGS=-Wall --std=c99
LDFLAGS=
CC=gcc
SRCDIR="../src"
LIBDIR="../lib"

SOURCES1= $(SRCDIR)/argpe_html.c $(SRCDIR)/argpe_html_api.c $(LIBDIR)/argpe_common.c prettify.c
SOURCES2= $(SRCDIR)/argpe_html.c $(SRCDIR)/argpe_html_api.c $(LIBDIR)/argpe_common.c printthree.c
OBJECTS=$(SOURCES:.c=.o)

all:
	$(CC) $(CFLAGS) $(SOURCES1) -o prettify
	$(CC) $(CFLAGS) $(SOURCES2) -o printthree
