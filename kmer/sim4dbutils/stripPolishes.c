#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "sim4polish.h"

//  Reads a file of matches, strips out the defline and alignments,
//  and writes a new file.

const char *usage =
"usage: %s [-nodeflines] [-noalignments]\n"
"  -nodeflines    Strip out deflines\n"
"  -noalignments  Strip out alignments\n"
"\n";

int
main(int argc, char **argv) {
  sim4polish  *p = 0L;
  int          dumpDefs   = 0;
  int          dumpAligns = 0;
  int          i  = 0;

  int  arg = 1;
  while (arg < argc) {
    if        (strncmp(argv[arg], "-nodeflines", 4) == 0) {
      dumpDefs = 1;
    } else if (strncmp(argv[arg], "-noalignments", 4) == 0) {
      dumpAligns = 1;
    } else {
      fprintf(stderr, "unknown option: %s\n", argv[arg]);
    }
    arg++;
  }

  if ((!dumpDefs && !dumpAligns) || isatty(fileno(stdin)) || isatty(fileno(stdout))) {
    fprintf(stderr, usage, argv[0]);

    if (!dumpDefs && !dumpAligns)
      fprintf(stderr, "error: You need to specify at least one of -nodeflines\n       and -noalignments.\n\n");

    if (isatty(fileno(stdin)))
      fprintf(stderr, "error: I cannot read polishes from the terminal!\n\n");

    if (isatty(fileno(stdout)))
      fprintf(stderr, "error: Please redirect the output to a file.\n\n");

    exit(1);
  }

  while (!feof(stdin)) {
    p = s4p_readPolish(stdin);

    if (p) {
      if (dumpAligns) {
        for (i=0; i<p->numExons; i++) {
          free(p->exons[i].estAlignment);
          free(p->exons[i].genAlignment);

          p->exons[i].estAlignment = 0L;
          p->exons[i].genAlignment = 0L;
        }
      }

      if (dumpDefs) {
        free(p->estDefLine);
        free(p->genDefLine);

        p->estDefLine = 0L;
        p->genDefLine = 0L;
      }

      s4p_printPolish(stdout, p);
    }

    s4p_destroyPolish(p);
  }

  return(0);
}
