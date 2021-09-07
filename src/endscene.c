#include <stdlib.h>
#include <string.h>
#include "endscene.h"
#include "error.h"

#define FNAME "assets/end-scene"
#define BUFFER_SIZE 63000

char* es_newBuffer()
{
        char *c = malloc(sizeof(char) * BUFFER_SIZE);
        return c;
}

void es_freeBuffer(char *c)
{
        free(c);
}

FILE* es_openFile()
{
        FILE *file = fopen(FNAME, "r");
        if (file == NULL) {
                printf("error: %s: failed to open file: %s\n",
                                __FILE__, FNAME);
                exit(0);
        }
        return file;
}

int es_closeFile(FILE *f)
{
        int i = fclose(f);
        if (i != 0) {
                printf("error: %s: failed to close file\n", __FILE__);
                exit(0);
        }
        return 0;
}

// Verification buffer for escape character comparison.
#define VBUF 7

char pushback;
int ptr;

char read(FILE* f) {
        if (ptr > 0) {
                ptr = 0;
                return pushback;
        }
        return fgetc(f);
}

void push(char c) {
        assert(ptr == 0);
        pushback = c;
        ptr = 1;
}

int es_readLine(FILE *f, char *buf, int *count)
{
        // Check for a file.
        if (f == NULL) {
                return -1;
        }

        char* ctest = "\033[22;0f";
        char* vbuf = malloc(sizeof(char) * VBUF);

        int c, n, ri, out, max;
        for (n = 0, out = 0; (c = read(f)) != EOF; n++, max++) {
                assert(n <= BUFFER_SIZE);

                buf[n] = c;

                if (c == '\033') {
                        // If this is the text after the last cursor
                        // move (\033[22;0) then push back the char and
                        // get out of here.
                        if (out > 0){
                                push(c);
                                if (max > *count) {
                                        *count = max;
                                }
                                max = 0;
                                break;
                        }
                        // reset the count.
                        ri = 0;
                }

                // If there is room in the verification buffer, write to
                // it.
                if (ri < VBUF){
                        vbuf[ri++] = c;
                }

                // The verification buffer is ready to make a
                // comparison.
                if (ri == VBUF && ((strncmp(vbuf, ctest, VBUF)) == 0 )) {
                        out = 1;
                }
        }

        return n;
}
