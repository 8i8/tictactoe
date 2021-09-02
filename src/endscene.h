#include <stdio.h>

char* es_newBuffer(void);
void es_freeBuffer(char* c);
FILE* es_openFile(void);
int es_closeFile(FILE* f);
int es_readLine(FILE* f, char* buf);
