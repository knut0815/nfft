#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define NFFT_PRECISION_DOUBLE
#include "nfft3mp.h"

nfft_plan* jnfft_alloc(void) {
	nfft_plan* p = nfft_malloc(sizeof(nfft_plan));
	return p;
}

void jnfft_init(nfft_plan* p, int d, int* N, int M, int* n, int m, unsigned int f1, unsigned int f2){
    nfft_init_guru(p,d,N,M,n,m,f1,f2);
}

double* jnfft_set_x(nfft_plan* p, double* X){
	int M = p->M_total;
	int d = p->d;
	int r,c;
	for (r = 0; r < M; r++)
		for (c = 0; c < d; c++)
			p->x[d*r+c] = X[d*r+c];
	return p->x;
}

// setting Fourier coefficients and returning pointer for access by Julia
complex double* jnfft_set_fhat(nfft_plan* p,complex double* f_hat){
	int n = p->N_total;
	int k;
	for (k=0;k<n;k++)
		p->f_hat[k] = f_hat[k];
	return p->f_hat;
}

// setting values and returning pointer for access by Julia
complex double* jnfft_set_f(nfft_plan* p,complex double* f){
	int M = p->M_total;
	int j;
	for (j=0;j<M;j++)
		p->f[j] = f[j];
	return p->f;
}

void jnfft_precompute_psi(nfft_plan* p){
	nfft_precompute_one_psi(p);
}

// nfft trafo, return pointer to values for access by Julia if pointer isn't set
complex double* jnfft_trafo(nfft_plan* p){
	nfft_trafo(p);
	return p->f;
}

// nfft adjoint, return pointer to coefficients for access by Julia if pointer isn't set
complex double* jnfft_adjoint(nfft_plan* p){
	nfft_adjoint(p);
	return p->f_hat;
}
