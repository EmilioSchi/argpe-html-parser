#include <stdio.h>
#include "../src/argpe_html.h"

int main (int argc, char* argv[])
{
	FILE *f;

	if (argc != 2) {
		fprintf(stderr, "usage: prettify <file.html>\n");
		return 1;
	}

	if (!(f = fopen(argv[1], "r"))) {
		fprintf(stderr, "[PARSER] Error: cannot open file %s\n", argv[1]);
		return 1;
	}

	html_doc doc = argpe_parser_html(f);
	argpe_html_prettify(doc);
	argpe_html_close(doc);

	fclose(f);

	return 0;
}
