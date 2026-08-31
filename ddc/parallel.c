#include "mandelbrot.h"

void compute_openmp(int w, int h, int max, int *out, int nthreads){
    omp_set_num_threads(nthreads);
    #pragma omp parallel for
    for(int row=0; row<h; row++){
        for(int col=0; col<w; col++){
            double cx = -2.0 + col*3.0/w;
            double cy = 1.5 - row*3.0/h;
            int iter = mandelbrot_iter(cx,cy,max);
            out[row*w+col] = iter * 255 / max;
        }
    }
}

static void* worker_block(void *arg){
    threads_args_t *a = (threads_args_t*)arg;
    int rows_per = a->h / a->nthreads;
    int start = a->id * rows_per;
    int end = start+rows_per;
    if(a->id == a->nthreads -1){
        end = a->h;
    }
    for(int row=start; row<end; row++){
        for(int col=0; col<a->w;col++){
            double cx = -2.0 + col*3.0/a->w;
            double cy = 1.5 - row*3.0/a->h;
            int iter = mandelbrot_iter(cx,cy,a->max);
            a->out[row*a->w+col] = iter * 255 / a->max;
        }
    }
    return NULL;
}

void compute_pthreads1(int w, int h, int max, int *out, int nthreads){
    pthread_t th[nthreads];
    threads_args_t args[nthreads];
    for(int i=0; i<nthreads; i++){
        args[i]=(threads_args_t){i, w, h, max, nthreads, out};
        pthread_create(&th[i], NULL, worker_block, &args[i]);
    }
    for(int i=0;i<nthreads;i++) pthread_join(th[i], NULL);
}

static void* worker_interleaved(void *arg){
    threads_args_t *a = (threads_args_t*)arg;
    for(int row=a->id; row<a->h; row+=a->nthreads){
        for(int col=0; col<a->w; col++){
            double cx = -2.0 + col * 3.0 / a->w;
            double cy = 1.5 - row * 3.0 / a->h;
            int iter = mandelbrot_iter(cx,cy,a->max);
            a->out[row*a->w+col] = iter * 255 / a->max;
        }
    }
    return NULL;
}

void compute_pthreads2(int w, int h, int max, int *out, int nthreads){
    pthread_t th[nthreads];
    threads_args_t args[nthreads];
    for(int i=0; i<nthreads; i++){
        args[i] = (threads_args_t){i, w, h, max, nthreads, out};
        pthread_create(&th[i], NULL, worker_interleaved, &args[i]);
    }
    for(int i=0; i<nthreads; i++) pthread_join(th[i], NULL);
}