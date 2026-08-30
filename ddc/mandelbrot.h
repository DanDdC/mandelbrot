#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <time.h>


int mandelbrot_iter(double cx, double cy, int max_iter);
void compute_serial(int w, int h, int max, int *out);
void compute_openmp(int w, int h, int max, int *out, int nthreads);
void compute_pthreads1(int w, int h, int max, int *out, int nthreads);
void compute_pthreads2(int w, int h, int max, int *out, int nthreads);

typedef struct {
    int id;
    int w, h, max, nthreads;
    int *out;
} threads_args_t;

#endif