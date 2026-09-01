#define _POSIX_C_SOURCE 199309L
#include "mandelbrot.h"
#include <string.h>
#include <errno.h>

static int parse_pos(const char *s, int *out){
    char *end;
    errno = 0;
    long v = strtol(s, &end, 10);
    if(errno != 0){
        return 0;
    }
    if(*end != '\0'){
        return 0;
    }
    if(v <= 0){
        return 0;
    }
    *out = (int)v;
    return 1;
}

static double diff_sec(struct timespec a, struct timespec b){
    double sec = b.tv_sec - a.tv_sec;
    double nsec = b.tv_nsec - a.tv_nsec;
    return sec + nsec / 1000000000.0;
}

static int write_pgm(const char *fname, int w, int h, int *img){
    FILE *f = fopen(fname, "w");
    if(f == NULL){
        return 0;
    }
    for(int r = 0; r < h; r++){
        for(int c = 0; c < w; c++){
            fprintf(f, "%d", img[r*w+c]);
            if(c == w-1){
                fprintf(f, "\n");
            }else{
                fprintf(f," ");
            }
        }
    }
    fclose(f);
    return 1;
}

int main(int argc, char *argv[]){
    if(argc != 5){
        fprintf(stderr, "Uso: %s largura altura max_iter num_threads\n", argv[0]);
        return 1;
    }
    int W, H, MAX, NTH;
    if(parse_pos(argv[1], &W) == 0){
        fprintf(stderr, "Erro: largura invalida\n");
        return 1;
    }
    if(parse_pos(argv[2], &H) == 0){
        fprintf(stderr, "Erro: altura invalida\n");
        return 1;
    }
    if(parse_pos(argv[3], &MAX) == 0){
        fprintf(stderr, "Erro: max_iteracoes invalido\n");
        return 1;
    }
    if(parse_pos(argv[4], &NTH) == 0){
        fprintf(stderr, "Erro: num_threads invalido\n");
        return 1;
    }

    int *img = malloc(W * H * sizeof(int));
    if(img == NULL){
        fprintf(stderr, "Erro: malloc\n");
        return 1;
    }

    struct timespec t0, t1;
    double ts, to, tp1, tp2;

    clock_gettime(CLOCK_MONOTONIC, &t0);
    compute_serial(W, H, MAX, img);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    ts = diff_sec(t0, t1);
    if(write_pgm("mandelbrot_ddc_serial.pgm", W, H, img) == 0){
        fprintf(stderr, "Erro: arquivo serial\n");
        free(img);
        return 1;
    }