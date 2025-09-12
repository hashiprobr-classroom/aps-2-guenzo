#include <math.h>

#include "fourier.h"

void normalize(complex s[], int n) {
    for (int k = 0; k < n; k++) {
        s[k].a /= n;
        s[k].b /= n;
    }
}

void nft(complex s[], complex t[], int n, int sign) {
    for (int k = 0; k < n; k++) {
        t[k].a = 0;
        t[k].b = 0;

        for (int j = 0; j < n; j++) {
            double x = sign * 2 * PI * k * j / n;

            double cosx = cos(x);
            double sinx = sin(x);

            t[k].a += s[j].a * cosx - s[j].b * sinx;
            t[k].b += s[j].a * sinx + s[j].b * cosx;
        }
    }
}

void nft_forward(complex s[], complex t[], int n) {
    nft(s, t, n, -1);
}

void nft_inverse(complex t[], complex s[], int n) {
    nft(t, s, n, 1);
    normalize(s, n);
}

void fft(complex s[], complex t[], int n, int sign) {
    complex s_i[n/2];
    complex s_p[n/2];
    complex t_p[n/2]; 
    complex t_i[n/2]; 
    int eh_par; 
    int index_par = 0;
    int index_impar = 0;

    // caso de parada
    if (n == 1){
        t[0] = s[0];
        return;
    }

    // implementando sp e si : 
    for (int index = 0; index < n; index++){
        eh_par = index%2 == 0;
        if(eh_par){
            s_p[index_par] = s[index];
            index_par++;
        }else{
            s_i[index_impar] = s[index];
            index_impar++;
        }

    }

    // tp recursivo, usando sp: 
    fft(s_p,t_p,n/2,sign);
    
    // ti recursivo usando si:
    fft(s_i,t_i,n/2,sign);
    
    // fixando k para uma somatoria
    for (int k = 0; k < n/2; k++){
        double x = (sign * 2 * PI * k)/n;
        double cosx = cos(x);
        double sinx = sin(x);

        t[k].a = t_p[k].a + t_i[k].a * cosx - t_i[k].b*sinx;
        t[k].b = t_p[k].b + t_i[k].a*sinx + t_i[k].b*cosx;
        
        t[k+n/2].a = t_p[k].a - t_i[k].a * cosx + t_i[k].b*sinx;
        t[k+n/2].b = t_p[k].b - t_i[k].a*sinx - t_i[k].b*cosx;
        
    }


}

void fft_forward(complex s[], complex t[], int n) {
    fft(s, t, n, -1);
}

void fft_inverse(complex t[], complex s[], int n) {
    fft(t, s, n, 1);
    normalize(s, n);
}

void fft_forward_2d(complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
}

void fft_inverse_2d(complex matrix[MAX_SIZE][MAX_SIZE], int width, int height) {
}

void filter(complex input[MAX_SIZE][MAX_SIZE], complex output[MAX_SIZE][MAX_SIZE], int width, int height, int flip) {
    int center_x = width / 2;
    int center_y = height / 2;

    double variance = -2 * SIGMA * SIGMA;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int dx = center_x - (x + center_x) % width;
            int dy = center_y - (y + center_y) % height;

            double d = dx * dx + dy * dy;
            double g = exp(d / variance);

            if (flip) {
                g = 1 - g;
            }

            output[y][x].a = g * input[y][x].a;
            output[y][x].b = g * input[y][x].b;
        }
    }
}

void filter_lp(complex input[MAX_SIZE][MAX_SIZE], complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
    filter(input, output, width, height, 0);
}

void filter_hp(complex input[MAX_SIZE][MAX_SIZE], complex output[MAX_SIZE][MAX_SIZE], int width, int height) {
    filter(input, output, width, height, 1);
}
