# argpe-html-parser
Library for parsing HTML, writen in C language. HTML parser without super dependencies. argpe-html-parser sources released as part of web bot Argiope.

## Examples

See [example] directory

**Simple example**

```c
#include <stdio.h>
#include "argpe_html.h"

int main (int argc, char* argv[])
{
	FILE *f;

	if (argc != 2) {
		fprintf(stderr, "usage: printthree <file.html>\n");
		return 1;
	}

	if (!(f = fopen(argv[1], "r"))) {
		fprintf(stderr, "Error: cannot open file %s\n", argv[1]);
		return 1;
	}

	html_doc doc = argpe_parser_html(f);
	argpe_html_print_three(doc);
	argpe_html_close(doc);

	fclose(f);

	return 0;
}
```

## Got questions?
For questions and discussion, or anything else, please shoot me a message on github or send me an email. You can find me at emilioschi@gmail.com . I will try to get back to you asap.
