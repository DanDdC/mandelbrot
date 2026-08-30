#include "mandelbrot.h"

int mandelbrot_iter(double cx, double cy, int max){
    double zx = 0;
    double zy = 0;
    int iter = 0;
    while(iter < max && zx*zx + zy*zy <= 4.0){
        double xt = zx*zx - zy*zy + cx;
        double yt = 2*zx*zy + cy;
        zx = xt;
        zy = yt;
        iter++;
    }
    return iter;
}

void compute_serial(int w, int h, int max, int *out){
    for(int row=0; row<h; row++){
        for(int col=0; col<w; col++){
            double cx = -2.0 + col * 3.0 / w;
            double cy = 1.5 - row*3.0 / h;
            int iter = mandelbrot_iter(cx,cy,max);
            out[row*w+col] = iter * 255 / max;
        }
    }
}