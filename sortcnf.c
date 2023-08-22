/************************************************************************************[sortcnf.c]
Copyright (c) 2023 Marijn Heule, Carnegie Mellon University
Last edit: August 22, 2023

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void usage(char *name) {
  printf("Usage: %s FILE1.cnf\n", name);
  exit(0);
}

int comparator(const void* p, const void* q) {
  int l = *(const int*)p;
  int r = *(const int*)q;

  return abs(l) > abs(r);
}

int main (int argc, char** argv) {
  if (argc < 2)
     usage(argv[0]);

  int nVar = 0, nCls = 0;
  char ignore[1024];
  FILE* cnf   = fopen (argv[1], "r");
  if (!cnf) {
      printf("Couldn't open file '%s'\n", argv[1]);
      exit(1); }

  for (;;) {
    int tmp = fscanf (cnf, " p cnf %i %i ", &nVar, &nCls);
    if (tmp == 2) break;
    fgets (ignore, sizeof (ignore), cnf);
    int j; for (j = 0; j < 1024; j++) { if (ignore[j] == '\n') break; }
    if (j == 1024) {
      printf ("c ERROR: comment longer than 1024 characters: %s\n", ignore); exit (0); } }

  printf ("p cnf %i %i\n", nVar, nCls);

  int* clause;
  clause = (int*) malloc (sizeof (int) * nVar);

  int size = 0;
  while (1) {
    int lit;
    int tmp = fscanf (cnf, " %i ", &lit);
    if (tmp == 0 || tmp == EOF) break;

    clause[size++] = lit;

    qsort((void*)clause, size - 1, sizeof(int), comparator);

    if (lit == 0) {
      for (int i = 0; i < size - 1; i++)
        printf ("%i ", clause[i]);
      printf ("0\n");
      size = 0;
    }
  }

  return 1;
}
