#include "mandelbrot.h"
#include <string.h>
#include <errno.h>

static int parse_pos(const char *s, int *out){
    char *end; errno=0;
    long v=strtol(s,&end,10);
    if(errno||*end!='\0'||v<=0||v>10000) return 0;
    *out=(int)v;
    return 1;
}

static double diff_sec(struct timespec a, struct timespec b){
    return (b.tv_sec - a.tv_sec)+(b.tv_nsec-a.tv_nsec) / 1e9;
}

static int write_pgm(const char *fname, int w, int h, int *img){
    FILE *f=fopen(fname, "w");
    for(int r=0; r<h; r++){
        for(int c=0; c<w; c++){
            fprintf(f, "%d", img[r*w+c]);
            if(c == w-1){
                fprintf(f, "\n");
            }else{
                fprintf(f," ");
            }
        }
    }
    fclose(f); return 1;
}