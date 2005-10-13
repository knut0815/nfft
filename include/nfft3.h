/*! \file nfft3.h
 *  \brief Header file for the nfft3 library.
 */
#ifndef NFFT3_H
#define NFFT3_H

/** Include header for C99 complex datatype. */
#include <complex.h>
/** Include header for FFTW3 library. */
#include <fftw3.h>

/** Macros for public members inherited by all plan structures */
#define MACRO_MV_PLAN(float_type)                                           \
int N_total;                          /**< total number of Fourier coeffs.*/\
int M_total;                          /**< total number of samples        */\
float_type *f_hat;                    /**< Fourier coefficients           */\
float_type *f;                        /**< samples                        */\
 
/** @defgroup nfft NFFT
 * Direct and fast computation of the
 * discrete Fourier transform at nonequispaced knots
 * @{ 
 */

/** 
 * Constant symbols for precomputation and memory usage
 */
#define PRE_PHI_HUT      (1U<< 0)
#define PRE_LIN_PSI      (1U<< 1)
#define PRE_PSI          (1U<< 2)
#define PRE_FULL_PSI     (1U<< 3)
#define MALLOC_X         (1U<< 4)
#define MALLOC_F_HAT     (1U<< 5)
#define MALLOC_F         (1U<< 6)
#define FFT_OUT_OF_PLACE (1U<< 7)
#define FFTW_INIT        (1U<< 8)

#define MALLOC_V         (1U<< 9)

#define SNDFT            (1U<< 10)

typedef struct nfft_plan_ 
{
  /** api */
  MACRO_MV_PLAN(complex);          

  int d;                                /**< dimension, rank                 */
  int *N;                               /**< cut-off-frequencies             */
  double *sigma;	                       /**< oversampling-factor             */
  int *n;                               /**< fftw-length = sigma*N           */
  int n_total;                          /**< total size of fftw              */
  int m;                                /**< cut-off parameter in time-domain*/
  double *b;                            /**< shape parameters                */
  int K;                                /**< number of precomp. uniform psi  */

  unsigned nfft_flags;                  /**< flags for precomputation, malloc*/
  unsigned fftw_flags;                  /**< flags for the fftw              */

  double *x;                            /**< nodes (in time/spatial domain)  */

  /** internal*/
  fftw_plan  my_fftw_plan1;             /**< fftw_plan forward               */
  fftw_plan  my_fftw_plan2;		           /**< fftw_plan backward              */

  double **c_phi_inv;                   /**< precomputed data, matrix D      */
  double *psi;				                      /**< precomputed data, matrix B      */
  int *psi_index_g;                     /**< only for PRE_FULL_PSI           */
  int *psi_index_f;                     /**< only for PRE_FULL_PSI           */

  complex *g;
  complex *g_hat;
  complex *g1;                          /**< input of fftw                   */
  complex *g2;                          /**< output of fftw                  */

  double *spline_coeffs;          	     /**< input for de Boor algorithm, if  
					                                        B_SPLINE or SINC_2m is defined  */
} nfft_plan;


/** see formula (1.1), computes
 * for j=0,...,M-1                                                             
 *  f[j] = sum_{k in I_N^d} f_hat[k] * exp(-2 (pi) k x[j])
 */
void ndft_trafo(nfft_plan *ths);

/** see formula (1.1), computes
 * for j=0,...,M-1                                                             
 *  f[j] = sum_{k in I_N^d} f_hat[k] * exp(+2 (pi) k x[j])
 */
void ndft_conjugated(nfft_plan *ths);

/** see formula (1.2), computes
 * for k in I_N^d
 *  f_hat[k] = sum_{j=0}^{M-1} f[j] * exp(+2(pi) k x[j])
 */
void ndft_adjoint(nfft_plan *ths);

/** see formula (1.2), computes
 * for k in I_N^d
 *  f_hat[k] = sum_{j=0}^{M-1} f[j] * exp(-2(pi) k x[j])
 */
void ndft_transposed(nfft_plan *ths);

/** see formula (1.1), computes in a fast and approximative way
 * for j=0,...,M-1                                                             
 *  f[j] = sum_{k in I_N^d} f_hat[k] * exp(-2 (pi) k x[j])
 */
void nfft_trafo(nfft_plan *ths);

/** see formula (1.1), computes in a fast and approximative way
 * for j=0,...,M-1                                                             
 *  f[j] = sum_{k in I_N^d} f_hat[k] * exp(+2 (pi) k x[j])
 */
void nfft_conjugated(nfft_plan *ths);

/** see formula (1.2), computes in a fast and approximative way
 * for k in I_N^d
 *  f_hat[k] = sum_{j=0}^{M-1} f[j] * exp(+2(pi) k x[j])
 */
void nfft_adjoint(nfft_plan *ths);

/** see formula (1.2), computes in a fast and approximative way
 * for k in I_N^d
 *  f_hat[k] = sum_{j=0}^{M-1} f[j] * exp(+2(pi) k x[j])
 */
void nfft_transposed(nfft_plan *ths);

/** wrapper for nfft_init and d=1
 */
void nfft_init_1d(nfft_plan *ths, int N1, int M);

/** wrapper for nfft_init and d=2
 */
void nfft_init_2d(nfft_plan *ths, int N1, int N2, int M);

/** wrapper for nfft_init and d=3
 */
void nfft_init_3d(nfft_plan *ths, int N1, int N2, int N3, int M);

/** initialisation of transform, simple interface
 */
void nfft_init(nfft_plan *ths, int d, int *N, int M);

/** initialisation of transform, advanced interfacegoogle.de/
 */
void nfft_init_advanced(nfft_plan *ths, int d, int *N, int M,
			unsigned nfft_flags_on, unsigned nfft_flags_off);

/** initialisation of transform, guru interface
 */
void nfft_init_guru(nfft_plan *ths, int d, int *N, int M, int *n,
	            int m, unsigned nfft_flags, unsigned fftw_flags);

/** precomputes equally spaced values of psi
 *  if PRE_LIN_PSI is set the application program has to call this routine
 */
void nfft_precompute_lin_psi(nfft_plan *ths);

/** precomputes the values psi in a tensor product form
 *  if PRE_PSI is set the application program has to call this routine
 *  after setting the nodes x
 */
void nfft_precompute_psi(nfft_plan *ths);

/** precomputes the values psi and their indices in non tensor product form
 *  if PRE_FULL_PSI is set the application program has to call this routine
 */
void nfft_precompute_full_psi(nfft_plan *ths);

/** finalisation of transform
 */
void nfft_finalize(nfft_plan *ths);

/** @} 
 */



/** @defgroup nfct NFCT
 * direct and fast computation of the
 * discrete cosine transform at nonequispaced knots in time/spatial domain
 * @{ 
 */

/** Structure for a transform plan */
typedef struct nfct_plan_  
{ 
  /** api */ 
  MACRO_MV_PLAN(double);

  int d;                                /**< dimension, rank                  */
  int *N;                               /**< cut-off-frequencies (kernel)     */
  int *n;                               /**< length of dct-i                  */ 
  double *sigma;                        /**< oversampling-factor              */
  int m;                                /**< cut-off parameter in time-domain */
 
  double nfct_full_psi_eps; 
  double *b;                            /**< shape parameters                 */ 
 
  unsigned nfct_flags;                  /**< flags for precomputation, malloc */ 
  unsigned fftw_flags;                  /**< flags for the fftw               */ 
 
  double *x;                            /**< nodes (in time/spatial domain)   */ 
 
  /** internal */ 
  fftw_plan  my_fftw_r2r_plan;          /**< fftw_plan                        */ 
  fftw_r2r_kind *r2r_kind;              /**< r2r transform type (dct-i)       */
 
  double **c_phi_inv;                   /**< precomputed data, matrix D       */ 
  double *psi;                          /**< precomputed data, matrix B       */ 
  int size_psi;                         /**< only for thin B                  */ 
  int *psi_index_g;                     /**< only for thin B                  */ 
  int *psi_index_f;                     /**< only for thin B                  */ 
 
  double *g; 
  double *g_hat; 
  double *g1;                           /**< input of fftw                    */ 
  double *g2;                           /**< output of fftw                   */ 
 
  double *spline_coeffs;                /**< input for de Boor algorithm, if   
                                             B_SPLINE or SINC_2m is defined   */ 
} nfct_plan; 


/**
 * Creates a 1-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg N0 The bandwidth \f$N\f$
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */
void nfct_init_1d( nfct_plan *ths_plan, int N0, int M_total); 

/**
 * Creates a 3-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg N0 The bandwidth of dimension 1
 * \arg N1 The bandwidth of dimension 2
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */
void nfct_init_2d( nfct_plan *ths_plan, int N0, int N1, int M_total); 

/**
 * Creates a 3-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg N0 The bandwidth of dimension 1
 * \arg N1 The bandwidth of dimension 2
 * \arg N2 The bandwidth of dimension 3
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */ 
void nfct_init_3d( nfct_plan *ths_plan, int N0, int N1, int N2, int M_total); 

/**
 * Creates a d-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg d the dimension
 * \arg N The bandwidths
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */ 
void nfct_init( nfct_plan *ths_plan, int d, int *N, int M_total); 

/**
 * Creates a d-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg d the dimension
 * \arg N The bandwidths
 * \arg M_total The number of nodes \f$x\f$
 * \arg n The oversampled bandwidths
 * \arg m The cut-off parameter
 * \arg nfct_flags The flags known to nfct
 * \arg fftw_flags The flags known to fftw
 *
 * \author Steffen Klatt
 */ 
void nfct_init_guru( nfct_plan *ths_plan, int d, int *N, int M_total, int *n,
                         int m, unsigned nfct_flags, unsigned fftw_flags);
 
/** 
 * precomputes the values psi 
 * if the PRE_PSI is set the application program has to call this routine 
 * after setting the nodes this_plan->x 
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */ 
void nfct_precompute_psi( nfct_plan *ths_plan); 


/** 
 * executes a NFCT (approximate,fast), computes for \f$j=0,...,M\_total-1\f$
 * \f$f_j^C(x_j) = sum_{k \in I_0^{N,d}} \hat{f}_k^C * cos(2 \pi k x_j)\f$
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */ 
void nfct_trafo( nfct_plan *ths_plan); 

/** 
 * executes a NDCT (exact,slow), computes for \f$j=0,...,M\_total-1\f$
 * \f$f_j^C(x_j) = sum_{k \in I_0^{N,d}} \hat{f}_k^C * cos(2 \pi k x_j)\f$
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void ndct_trafo( nfct_plan *ths_plan);
 
/** 
 * executes a transposed NFCT (approximate,fast), computes for \f$k \in I_0^{N,d}\f$
 * \f$h^C(k) = sum_{j \in I_0^{(M\_total,1)}} f_j^C * cos(2 \pi k x_j)\f$
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void nfct_transposed( nfct_plan *ths_plan); 

/** 
 * executes a direct transposed NDCT (exact,slow), computes for \f$k \in I_0^{N,d}\f$
 * \f$h^C(k) = sum_{j \in I_0^{(M\_total,1)}} f_j^C * cos(2 \pi k x_j)\f$
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void ndct_transposed( nfct_plan *ths_plan); 
 
/**
 * Destroys a plan.
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt 
 */
void nfct_finalize( nfct_plan *ths_plan);

/**
 * do some adjustments (N,n) then compute PHI_HUT
 * 
 * \arg ths_plan the plan for the transform
 * \arg k        index of c_phi
 * \arg d        dimension
 *
 * \author Steffen Klatt
 */
double nfct_phi_hut( nfct_plan *ths_plan, int k, int d);

/**
 * do some adjustments (N,n) then compute PHI
 * 
 * \arg ths_plan the plan for the transform
 * \arg x        node \f$x\f$
 * \arg d        dimension
 *
 * \author Steffen Klatt
 */
double nfct_phi ( nfct_plan *ths_plan, double x, int d);

/**
 * returns 2(n-1),  fftw related issue
 *
 * \arg n       i.e. length of dct-1
 *
 * \author Steffen Klatt
 */
int nfct_fftw_2N( int n);

/**
 * returns 0.5n+1,  fftw related issue
 *
 * \arg n       i.e. length of dct-1
 *
 * \author Steffen Klatt
 */
int nfct_fftw_2N_rev( int n);

/** @}  
 */


/** @defgroup nfst NFST
 * direct and fast computation of the
 * discrete sine transform at nonequispaced knots in time/spatial domain
 * @{ 
 */

/** Structure for a transform plan */
typedef struct nfst_plan_ 
{
  /** api */
  MACRO_MV_PLAN(double);

  int d;                                /**< dimension, rank                  */
  int *N;                               /**< bandwidth                        */
  int *n;                               /**< length of dst-1                  */
  double *sigma;                        /**< oversampling-factor              */
  int m;                                /**< cut-off parameter in time-domain */

  double nfst_full_psi_eps;
  double *b;                            /**< shape parameters                 */

  unsigned nfst_flags;                  /**< flags for precomputation, malloc */
  unsigned fftw_flags;                  /**< flags for the fftw               */

  double *x;                            /**< nodes (in time/spatial domain)   */

  /** internal */
  fftw_plan  my_fftw_r2r_plan;         /**< fftw_plan forward                */
  fftw_r2r_kind *r2r_kind;              /**< r2r transform type (dct-i)       */

  double **c_phi_inv;                   /**< precomputed data, matrix D       */
  double *psi;                          /**< precomputed data, matrix B       */
  int size_psi;                         /**< only for thin B                  */
  int *psi_index_g;                     /**< only for thin B                  */
  int *psi_index_f;                     /**< only for thin B                  */


  double *g;
  double *g_hat;
  double *g1;                           /**< input of fftw                    */
  double *g2;                           /**< output of fftw                   */

  double *spline_coeffs;                /**< input for de Boor algorithm, if  
                                             B_SPLINE or SINC_2m is defined   */
} nfst_plan;


/**
 * Creates a 1-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg N0 The bandwidth \f$N\f$
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */
void nfst_init_1d( nfst_plan *ths_plan, int N0, int M_total);

/**
 * Creates a 3-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg N0 The bandwidth of dimension 1
 * \arg N1 The bandwidth of dimension 2
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */
void nfst_init_2d( nfst_plan *ths_plan, int N0, int N1, int M_total);

/**
 * Creates a 3-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg N0 The bandwidth of dimension 1
 * \arg N1 The bandwidth of dimension 2
 * \arg N2 The bandwidth of dimension 3
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */
void nfst_init_3d( nfst_plan *ths_plan, int N0, int N1, int N2, int M_total);

/**
 * Creates a d-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg d the dimension
 * \arg N The bandwidths
 * \arg M_total The number of nodes \f$x\f$
 *
 * \author Steffen Klatt
 */
void nfst_init( nfst_plan *ths_plan, int d, int *N, int M_total);

/**
 * Creates a d-dimensional transform plan with pcific m.
 * (just for convenience)
 *
 * \arg ths_plan The plan for the transform
 * \arg d the dimension
 * \arg N The bandwidths
 * \arg M_total The number of nodes \f$x\f$
 * \arg m cut-off parameter
 *
 * \author Steffen Klatt
 */
void nfst_init_m( nfst_plan *ths_plan, int d, int *N, int M_total, int m);

/**
 * Creates a d-dimensional transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg d the dimension
 * \arg N The bandwidths
 * \arg M_total The number of nodes \f$x\f$
 * \arg n The oversampled bandwidths
 * \arg m The cut-off parameter
 * \arg nfst_flags The flags known to nfst
 * \arg fftw_flags The flags known to fftw
 *
 * \author Steffen Klatt
 */
void nfst_init_guru( nfst_plan *ths_plan, int d, int *N, int M_total, int *n,
                     int m, unsigned nfst_flags, unsigned fftw_flags);

/** 
 * precomputes the values psi 
 * if the PRE_PSI is set the application program has to call this routine 
 * after setting the nodes this_plan->x 
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void nfst_precompute_psi( nfst_plan *ths_plan);

/** 
 * executes a NFST (approximate,fast), computes for \f$j=0,...,M\_total-1\f$
 * \f$f_j^S(x_j) = sum_{k \in I_1^{N,d}} \hat{f}_k^S * sin(2 \pi k x_j)\f$
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void nfst_trafo( nfst_plan *ths_plan);

/** 
 * executes a NDST (exact,slow), computes for \f$j=0,...,M\_total-1\f$
 * \f$f_j^S(x_j) = sum_{k \in I_1^{N,d}} \hat{f}_k^S * sin(2 \pi k x_j)\f$
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void ndst_trafo( nfst_plan *ths_plan);



/** 
 * executes a transposed NFST (approximate,fast), computes for \f$k \in I_1^{N,d}\f$
 * \f$h^S(k) = sum_{j \in I_0^{M\_total,1}} f_j^S * cos(2 \pi k x_j)\f$
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void nfst_transposed( nfst_plan *ths_plan);

/** 
 * executes a direct transposed NDST (exact,slow), computes for \f$k \in I_1^{N,d}\f$
 * \f$h^S(k) = sum_{j \in I_0^{M\_total,1}} f_j^S * cos(2 \pi k x_j)\f$ 
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt
 */
void ndst_transposed( nfst_plan *ths_plan);

/**
 * Destroys a plan.
 *
 * \arg ths_plan The plan for the transform
 *
 * \author Steffen Klatt 
 */
void nfst_finalize( nfst_plan *ths_plan);

/**
 * 
 * 
 * \arg ths_plan The plan for the transform
 * 
 */
void nfst_full_psi( nfst_plan *ths_plan, double eps);

/**
 * do some adjustments (N,n) then compute PHI_HUT
 * 
 * \arg ths_plan the plan for the transform
 * \arg k        index of c_phi
 * \arg d        dimension
 *
 * \author Steffen Klatt
 */
double nfst_phi_hut( nfst_plan *ths_plan, int k, int d);

/**
 * do some adjustments (N,n) then compute PHI
 * 
 * \arg ths_plan the plan for the transform
 * \arg x        node \f$x\f$
 * \arg d        dimension
 *
 * \author Steffen Klatt
 */
double nfst_phi ( nfst_plan *ths_plan, double x, int d);

/**
 * returns 2(n+1),  fftw related issue
 *
 * \arg n       i.e. length of dst-1
 *
 * \author Steffen Klatt
 */
int nfst_fftw_2N( int n);

/**
 * returns 0.5n-1,  fftw related issue
 *
 * \arg n       i.e. length of dct-1
 *
 * \author Steffen Klatt
 */
int nfst_fftw_2N_rev( int n);

/** @} 
 */ 




/** @defgroup nnfft NNFFT
 * Direct and fast computation of the
 * discrete Fourier transform at nonequispaced knots in time and Fourier domain
 * @{ 
 */

/** 
 * Structure for a transform plan
 */
typedef struct nnfft_plan_ 
{
  /** api */
  MACRO_MV_PLAN(complex);

  int d;                                /**< dimension, rank                 */
  double *sigma;                        /**< oversampling-factor 1           */
  double *a;                            /**< 1 + 2*m1/N1                     */
  int *N;                               /**< cut-off-frequencies             */
  int *N1;
  int *aN1;
  int m;                                /**< cut-off parameter in time-domain*/
  double *b;                            /**< shape parameters                */
  int K;                                /**< number of precomp. uniform psi  */

  int aN1_total;
  
  nfft_plan *direct_plan;               /**< plan for the nfft               */
  unsigned nnfft_flags;                 /**< flags for precomputation, malloc*/
  int *n;                               /**<  = a*N                          */
  
  double *x;                            /**< nodes (in time/spatial domain)  */
  double *v;                            /**< nodes (in fourier domain)       */

  double *c_phi_inv;                    /**< precomputed data, matrix D      */
  double *psi;                          /**< precomputed data, matrix B      */
  int size_psi;                         /**< only for thin B                 */
  int *psi_index_g;                     /**< only for thin B                 */
  int *psi_index_f;                     /**< only for thin B                 */
  complex *F;

  double *spline_coeffs;                /**< input for de Boor algorithm, if  
                                             B_SPLINE or SINC_2m is defined  */
} nnfft_plan;


/**
 * Creates a transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg d The dimension
 * \arg N_total The number of nodes \f$v\f$
 * \arg M_total The number of nodes \f$x\f$
 * \arg N The bandwidth \f$N\f$
 *
 * \author Tobias Knopp
 */
void nnfft_init(nnfft_plan *ths_plan, int d, int N_total, int M_total, int *N);

/**
 * Creates a transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg d The dimension
 * \arg N_total The number of nodes \f$v\f$
 * \arg M_total The number of nodes \f$x\f$
 * \arg N The bandwidth \f$N\f$
 * \arg N1 The oversampled bandwidth \f$N\f$
 * \arg m The cut-off parameter
 * \arg nnfft_flags The flags
 *
 * \author Tobias Knopp
 */
void nnfft_init_guru(nnfft_plan *ths_plan, int d, int N_total, int M_total,
                     int *N, int *N1, int m, unsigned nnfft_flags);
                     
/**
 * Executes a direct NNDFT, i.e. computes for \f$j=0,...,M_{total}-1\f$
 * \f[
 *   f(x_j) = \sum_{k = 0}^{N_{total}-1} \hat{f}(v_k) {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nndft_trafo(nnfft_plan *ths_plan);

/**
 * Executes a direct conjugated NNDFT, i.e. computes for \f$j=0,...,M_{total}-1\f$
 * \f[
 *   f(x_j) = \sum_{k = 0}^{N_{total}-1} \hat{f}(v_k) {\rm e}^{2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nndft_conjugated(nnfft_plan *ths_plan);

/**
 * Executes a direct adjoint NNDFT, i.e. computes for \f$k=0,...,N_{total}-1\f$
 * \f[
 *   \hat{f}(v_k) = \sum_{j = 0}^{M_{total}-1} f(x_j) {\rm e}^{2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nndft_adjoint(nnfft_plan *ths_plan);

/**
 * Executes a direct transposed NNDFT, i.e. computes for \f$k=0,...,N_{total}-1\f$
 * \f[
 *   \hat{f}(v_k) = \sum_{j = 0}^{M_{total}-1} f(x_j) {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nndft_transposed(nnfft_plan *ths_plan);

/**
 * Executes a NNFFT, i.e. computes for \f$j=0,...,M_{total}-1\f$
 * \f[
 *   f(x_j) = \sum_{k = 0}^{N_{total}-1} \hat{f}(v_k) {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nnfft_trafo(nnfft_plan *ths_plan);

/**
 * Executes a conjugated NNFFT, i.e. computes for \f$j=0,...,M_{total}-1\f$
 * \f[
 *   f(x_j) = \sum_{k = 0}^{N_{total}-1} \hat{f}(v_k) {\rm e}^{2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nnfft_conjugated(nnfft_plan *ths_plan);

/**
 * Executes a adjoint NNFFT, i.e. computes for \f$k=0,...,N_{total}-1\f$
 * \f[
 *   \hat{f}(v_k) = \sum_{j = 0}^{M_{tota}l-1} f(x_j) {\rm e}^{2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nnfft_adjoint(nnfft_plan *ths_plan);

/**
 * Executes a transposed NNFFT, i.e. computes for \f$k=0,...,N_{total}-1\f$
 * \f[
 *   \hat{f}(v_k) = \sum_{j = 0}^{M_{total}-1} f(x_j) {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} v_k x_j \odot N}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nnfft_transposed(nnfft_plan *ths_plan);

void nnfft_precompute_lin_psi(nnfft_plan *ths_plan);

void nnfft_precompute_psi(nnfft_plan *ths_plan);

void nnfft_precompute_full_psi(nnfft_plan *ths_plan);

void nnfft_precompute_phi_hut(nnfft_plan *ths_plan);

/**
 * Destroys a plan.
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void nnfft_finalize(nnfft_plan *ths_plan);
 
/** @} 
 */

/** @defgroup nsfft NSFFT
 * @{ 
 */
typedef struct nsfft_plan_ 
{
  MACRO_MV_PLAN(complex);

  int d;                                /**< d=2,3                           */

  int J;                      
  int sigma;

  unsigned flags;

  int *index_sparse_to_full;            /**< overflow for d=3, J=9!          */

  int r_act_nfft_plan;
  nfft_plan *act_nfft_plan;
  nfft_plan *center_nfft_plan;

  fftw_plan* set_fftw_plan1;
  fftw_plan* set_fftw_plan2;

  nfft_plan *set_nfft_plan_1d;
  nfft_plan *set_nfft_plan_2d;          /**< d=3                             */

  double *x_transposed;                 /**< d=2                             */
  double *x_102,*x_201,*x_120,*x_021;   /**< d=3                             */
  
} nsfft_plan;

void nsdft_trafo(nsfft_plan *ths);
void nsdft_adjoint(nsfft_plan *ths);

void nsfft_trafo(nsfft_plan *ths);
void nsfft_adjoint(nsfft_plan *ths);

/** Copy coefficients and knots from ths to ths_nfft */
void nsfft_cp(nsfft_plan *ths, nfft_plan *ths_nfft);
void nsfft_init_random_nodes_coeffs(nsfft_plan *ths);

void nsfft_init(nsfft_plan *ths, int d, int J, int M, int m, unsigned flags);
void nsfft_finalize(nsfft_plan *ths);

/** @} 
 */

/** @defgroup mri_inh MRI_INH 
 * @{ 
 */

/**
 * The structure for the transform plan.
 */
typedef struct mri_inh_2d1d_plan_
{
  /** api */
  MACRO_MV_PLAN(complex);
  
	nfft_plan plan;

  int N3;
	double sigma3;
  double *t;
  double *w;
} mri_inh_2d1d_plan;

/**
 * The structure for the transform plan.
 */
typedef struct mri_inh_3d_plan_
{
  /** api */
  MACRO_MV_PLAN(complex);

  nfft_plan plan;
  
  int N3;
	double sigma3;
  double *t;
  double *w;
} mri_inh_3d_plan;


/**
 * Executes a mri transformation considering field inhomogenities using the 2d1d method, 
 * i.e. computes for \f$j=0,...,M_{total}-1\f$
 * \f[
 *   f(x_j) = \sum_{k \in I_N^2} \hat{f}(k) {\rm e}^{\mbox{\rm\scriptsize i} t_j \omega(k)}
 *                      {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} k x_j}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void mri_inh_2d1d_trafo(mri_inh_2d1d_plan *ths);

/**
 * Executes an adjoint mri transformation considering field inhomogenities using the 2d1d method, 
 * i.e. computes for \f$k \in I_N^2\f$
 * \f[
 *   \hat{f}(k) = \sum_{j=0}^{M_{total}-1} f(x_j) {\rm e}^{\mbox{\rm\scriptsize i} t_j \omega(k)}
 *                      {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} k x_j}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void mri_inh_2d1d_adjoint(mri_inh_2d1d_plan *ths);

/**
 * Creates a transform plan.
 *
 * \arg ths_plan The plan for the transform
 * \arg N The bandwidth \f$N\f$
 * \arg M_total The number of nodes \f$x\f$
 * \arg n The oversampled bandwidth \f$N\f$
 * \arg m The cut-off parameter
 * \arg sigma The oversampling factor
 * \arg nnfft_flags The flags
 *
 * \author Tobias Knopp
 */
void mri_inh_2d1d_init_guru(mri_inh_2d1d_plan *ths, int *N, int M, int *n,
                    int m, double sigma, unsigned nfft_flags, unsigned fftw_flags);

/**
 * Destroys a plan.
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void mri_inh_2d1d_finalize(mri_inh_2d1d_plan *ths);

/**
 * Executes a mri transformation considering field inhomogenities using the 3d method, 
 * i.e. computes for \f$j=0,...,M_{total}-1\f$
 * \f[
 *   f(x_j) = \sum_{k \in I_N^2} \hat{f}(k) {\rm e}^{\mbox{\rm\scriptsize i} t_j \omega(k)}
 *                      {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} k x_j}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void mri_inh_3d_trafo(mri_inh_3d_plan *ths);

/**
 * Executes an adjoint mri transformation considering field inhomogenities using the 3d method, 
 * i.e. computes for \f$k \in I_N^2\f$
 * \f[
 *   \hat{f}(k) = \sum_{j=0}^{M_{total}-1} f(x_j) {\rm e}^{\mbox{\rm\scriptsize i} t_j \omega(k)}
 *                      {\rm e}^{-2 \pi \mbox{\rm\scriptsize i} k x_j}
 * \f]
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void mri_inh_3d_adjoint(mri_inh_3d_plan *ths);

void mri_inh_3d_init_guru(mri_inh_3d_plan *ths, int *N, int M, int *n,
                    int m, double sigma, unsigned nfft_flags, unsigned fftw_flags);

/**
 * Destroys a plan.
 *
 * \arg ths_plan The plan
 *
 * \author Tobias Knopp
 */
void mri_inh_3d_finalize(mri_inh_3d_plan *ths);

/** @} 
 */

/** @defgroup texture Texture
 * @{
 */

/**
 * Flags controlling the texture transform.
 */

/** 
 * Constant for the max angle (representation of 2*Pi)
 */
#define TEXTURE_MAX_ANGLE (2*3.1415926535897932384)

/**
 * Constants for default values.
 */
#define TEXTURE_DEF_PRECOMPUTE_FLAGS 0U
#define TEXTURE_DEF_NFSFT_PRECOMPUTE_FLAGS 0U
#define TEXTURE_DEF_NFSFT_THRESHOLD 1000.0
#define TEXTURE_DEF_INIT_FLAGS 0U
#define TEXTURE_DEF_NFSFT_INIT_FLAGS 0U
#define TEXTURE_DEF_NFFT_CUTOFF 6

// Flag controlling wether to check for errors of usage

/**
 * The structure for the transform plan.
 */
typedef struct texture_plan_ {

	MACRO_MV_PLAN(complex);

	int N;
	int N1;
	int N2;
	const double *h_phi;
	const double *h_theta;
	const double *r;

	/*TODO*/
	unsigned int nfsft_init_flags;
	unsigned int nfft_cutoff;

	/* private */
	double *cos_h_theta;
	double *sin_h_theta;

	complex **nfsft_f_hat;
	complex *nfsft_f;
	double *nfsft_angles;
} texture_plan;

void texture_precompute(int N);

void texture_precompute_advanced(int N, unsigned int texture_precompute_flags, 
		unsigned int nfsft_precompute_flags, double nfsft_threshold);

/**
 * Simple initialisation of a plan.
 * Use texture_finalize to free allocated memory.
 */
void texture_init(texture_plan *ths, int N, int N1, int N2, complex* omega, 
		complex* x, const double* h_phi, const double* h_theta, const double* r);

/**
 * Advanced initialisation of a plan.
 */
//TODO int nfsft_flags
void texture_init_advanced(texture_plan *ths, int N, int N1, int N2,
		complex* omega, complex* x, const double* h_phi, const double* h_theta, 
		const double *r, unsigned int texture_init_flags, 
		unsigned int nfsft_init_flags, int nfft_cutoff);

/**
 * Carries out the transform.
 */
void texture_trafo(texture_plan *ths);

/**
 * The adjoint version of the transform.
 */
void texture_adjoint(texture_plan *ths);

/**
 * Frees all memory allocated by texture_init or texture_init_advanced.
 */
void texture_finalize(texture_plan *ths);

void texture_forget();

/* utiliy functions */

/**
 * Convert a non-flat index to a flat index.
 * \arg l the frequence
 * \arg m ranges from -l to l
 * \arg n ranges from -l to l
 */
inline int texture_flat_index(int l, int m, int n);

inline int texture_flat_length(int N);

inline int texture_get_omega_length(texture_plan *ths);

inline int texture_get_x_length(texture_plan *ths);

inline int texture_get_N(texture_plan *ths);

inline int texture_get_N1(texture_plan *ths);

inline int texture_get_N2(texture_plan *ths);

inline const complex *texture_get_omega(texture_plan *ths);

inline void texture_set_omega(texture_plan *ths, complex* omega);

inline const complex *texture_get_x(texture_plan *ths);

inline void texture_set_x(texture_plan *ths, complex* x);

inline const double *texture_get_h_phi(texture_plan *ths);

inline void texture_set_h_phi(texture_plan *ths, const double* h_phi);

inline const double *texture_get_h_theta(texture_plan *ths);

inline void texture_set_h_theta(texture_plan *ths, const double* h_theta);

inline const double *texture_get_r(texture_plan *ths);

inline void texture_set_r(texture_plan *ths, const double* r);

/*TODO*/
inline unsigned int texture_get_nfsft_init_flags(texture_plan *ths);

inline void texture_set_nfsft_init_flags(texture_plan *ths, 
		unsigned int nfsft_init_flags);

inline int texture_get_nfft_cutoff(texture_plan *ths);

inline void texture_set_nfft_cutoff(texture_plan *ths, int nfft_cutoff);

/* private */

/**
 * The structure for the transform plan.
 */
/*
struct texture_plan {

	MACRO_MV_PLAN(complex);

	int N;
	int N1;
	int N2;
	double *h_phi;
	double *h_theta;
	double *r;

	double *cos_h_theta;
	double *sin_h_theta;

	complex **nfsft_f_hat;
	complex *nfsft_f;
	double *nfsft_angles;

	unsigned int nfsft_init_flags;
	unsigned int nfft_cutoff;
}; // texture_plan_s;
*/
/** @}
 */

/** 
 * @defgroup nfsft NFSFT
 * @{ 
 * 
 * This module implements nonuniform fast spherical Fourier transforms (NFSFT).
 *
 * \section Preliminaries
 * This section summarises basic definitions and properties related to spherical 
 * Fourier transforms.
 *
 * \subsection sc Spherical Coordinates
 * Every point in \f$\mathbb{R}^3\f$ can be described in \e spherical \e 
 * coordinates by a vector \f$(r,\vartheta,\varphi)^{\mathrm{T}}\f$ with the 
 * radius \f$r \in \mathbb{R}^{+}\f$ and the angles \f$\vartheta \in [0,\pi]\f$, 
 * \f$\varphi \in [-\pi,\pi)\f$. 
 * We denote by \f$\mathbb{S}^2\f$ the two-dimensional unit sphere embedded 
 * into \f$\mathbb{R}^3\f$, i.e. 
 * \f[
 *   \mathbb{S}^2 := \left\{\mathbf{x} \in \mathbb{R}^{3}:\; 
 *   \|\mathbf{x}\|_2=1\right\}
 * \f] 
 * and identify a point from \f$\mathbb{S}^2\f$ with the corresponding vector 
 * \f$(\vartheta,\varphi)^{\mathrm{T}}\f$. The 
 * spherical coordinate system is illustrated in the following figure:
 * \image html sphere.png "" width=0.45\textwidth
 * For consistency with the other modules and the conventions used there, we 
 * also use \e scaled \e spherical \e coordinates \f$x_1 := 
 * \frac{\vartheta}{2\pi}\f$, \f$x_2 := \frac{\varphi}{2\pi}\f$ and identify a point 
 * from \f$\mathbb{S}^2\f$ with the vector \f$\mathbf{x} := \left(x_1,x_2\right) \in 
 * [0,\frac{1}{2}] \times [-\frac{1}{2}, \frac{1}{2})\f$.
 *
 * \subsection lp Legendre Polynomials
 * The \e Legendre \e polynomials \f$P_k : [-1,1] 
 * \rightarrow \mathbb{R}$, $k \in \mathbb{N}_{0}\f$ as \e classical \e 
 * orthogonal \e polynomials are given by their corresponding \e Rodrigues \e 
 * formula
 * \f[
 *   P_k(t) := \frac{1}{2^k k!} \frac{\text{d}^k}{\text{d} t^k} 
 *   \left(t^2-1\right)^k.
 * \f]
 * The corresponding three-term recurrence relation is
 * \f[
 *   (k+1)P_{k+1}(t) = (2k+1) x P_{k}(t) - k P_{k-1}(t) \quad (k \in 
 *   \mathbb{N}_0).
 * \f]  
 * With
 * \f[
 *   \left< f,g \right>_{\text{L}^2\left([-1,1]\right)} := 
 *   \int_{-1}^{1} f(t) g(t) \text{d} t
 * \f]  
 * being the usual \f$\text{L}^2\left([-1,1]\right)\f$ inner product,
 * the Legendre polynomials obey the orthogonality condition
 * \f[
 *   \left< P_k,P_l \right>_{\text{L}^2\left([-1,1]\right)} = \frac{2}{2k+1} 
 *   \delta_{k,l}.
 * \f]
 *
 * \remark The normalisation constant \f$ c_k := \sqrt{\frac{2k+1}{2}}\f$ 
 * renders the scaled Legendre polynomials \f$c_k P_k\f$ orthonormal with 
 * respect to the induced \f$\text{L}^2\left([-1,1]\right)\f$ norm
 * \f[
 *   \|f\|_{\text{L}^2\left([-1,1]\right)} := 
 *   \sqrt{<f,f>_{\text{L}^2\left([-1,1]\right)}} =
 *   \sqrt{\int_{-1}^{1} |f(t)|^2 \; \text{d} t}.
 * \f]
 * 
 * \subsection alf Associated Legendre Functions
 * The \a associated \a Legendre \a functions \f$P_k^n : [-1,1] \rightarrow 
 * \mathbb{R} \f$, \f$n \in \mathbb{N}_0\f$, \f$k \ge n\f$ are defined by
 * \f[ 
 *   P_k^n(t) := \left(\frac{(k-n)!}{(k+n)!}\right)^{1/2} 
 *   \left(1-t^2\right)^{n/2} \frac{\text{d}^n}{\text{d} t^n} P_k(t).
 * \f]
 * For \f$n = 0\f$ they coincide with the Legendre polynomials, i.e. 
 * \f$P_k^0 = P_k\f$. 
 * The associated Legendre functions obey the three-term recurrence relation
 * \f[  
 *   P_{k+1}^n(t) = v_{k}^n t P_k^n(t) + w_{k}^n P_{k-1}^n(t) \quad (k \ge n),
 * \f]
 * with \f$P_{n-1}^n(t) := 0\f$, \f$P_{n}^n(t) := \frac{\sqrt{(2n)!}}{2^n n!} 
 * \left(1-t^2\right)^{n/2}\f$, and
 * \f[ 
 *   v_{k}^n := \frac{2k+1}{((k-n+1)(k+n+1))^{1/2}}\; ,\qquad 
 *   w_{k}^n := - \frac{((k-n)(k+n))^{1/2}}{((k-n+1)(k+n+1))^{1/2}}.
 * \f]
 * For fixed \f$n\f$, the set \f$\left\{P_k^n:\: k 
 * \ge n\right\}\f$ forms a complete set of orthogonal functions for 
 * \f$\text{L}^2\left([-1,1]\right)\f$ 
 * with
 * \f[ 
 *   \left< P_k^n,P_l^n \right>_{\text{L}^2\left([-1,1]\right)} = \frac{2}{2k+1}
 *   \delta_{k,l} \quad (0 \le n \le k,l).
 * \f]
 * 
 * \remark The normalisation constant \f$ c_k = \sqrt{\frac{2k+1}{2}}\f$ 
 * renders the scaled associated Legendre functions \f$c_k P_k^n\f$ orthonormal 
 * with respect to the induced \f$\text{L}^2\left([-1,1]\right)\f$ norm
 * \f[
 *   \|f\|_{\text{L}^2\left([-1,1]\right)} = 
 *   \sqrt{<f,f>_{\text{L}^2\left([-1,1]\right)}} =
 *   \sqrt{\int_{-1}^{1} |f(t)|^2 \; \text{d} t}.
 * \f]
 *
 * \subsection sh Spherical Harmonics
 * The standard orthogonal basis of spherical harmonics for \f$\text{L}^2
 * \left(\mathbb{S}^2\right)\f$ with yet unnormalised basis functions 
 * \f$\tilde{Y}_k^n : \mathbb{S}^2 \rightarrow \mathbb{C}\f$ is given by
 * \f[
 *   \tilde{Y}_k^n(\vartheta,\varphi) := P_k^{|n|}(\cos\vartheta) 
 *   \mathrm{e}^{\mathrm{i} n \varphi}
 * \f]
 * with the usual \f$\text{L}^2\left(\mathbb{S}^2\right)\f$ inner product
 * \f[
 *   \left< f,g \right>_{\mathrm{L}^2\left(\mathbb{S}^2\right)} := 
 *   \int_{\mathbb{S^2}} f(\vartheta,\varphi) \overline{g(\vartheta,\varphi)} 
 *   \: \mathrm{d} \mathbf{\xi} := \int_{-\pi}^{\pi} \int_{0}^{\pi} 
 *   f(\vartheta,\varphi) \overline{g(\vartheta,\varphi)} \sin \vartheta 
 *   \; \mathrm{d} \vartheta \; \mathrm{d} \varphi.
 * \f]
 * The normalisation constant \f$c_k^n := \sqrt{\frac{2k+1}{4\pi}}\f$ renders 
 * the  scaled basis functions 
 * \f[
 *   Y_k^n(\vartheta,\varphi) := c_k^n P_k^{|n|}(\cos\vartheta) 
 *   \mathrm{e}^{\mathrm{i} n \varphi}
 * \f]
 * orthonormal with respect to the induced \f$\text{L}^2\left(\mathbb{S}^2
 * \right)\f$ norm
 * \f[
 *   \|f\|_{\text{L}^2\left(\mathbb{S}^2\right)} = 
 *   \sqrt{<f,f>_{\text{L}^2\left(\mathbb{S}^2\right)}} =
 *   \sqrt{\int_{-\pi}^{\pi} \int_{0}^{\pi} |f(\vartheta,\varphi)|^2 \sin 
 *   \vartheta \; \mathrm{d} \vartheta \; \mathrm{d} \varphi}.
 * \f]
 * A function \f$f \in \mathrm{L}^2\left(\mathbb{S}^2\right)\f$ has the 
 * orthogonal expansion
 * \f[
 *   f = \sum_{k=0}^{\infty} \sum_{n=-k}^{k} \hat{f}(k,n) Y_k^n,
 * \f]
 * where the coefficients \f$\hat{f}(k,n) := \left< f, Y_k^{n} 
 * \right>_{\mathrm{L}^2\left(\mathbb{S}^2\right)}\f$ are the \e spherical 
 * \e Fourier \e coefficients and the equivalence is understood in the 
 * \f$\mathrm{L}^2\f$-sense.
 *
 *
 * \section nfsfts Nonuniform Fast Spherical Fourier Transforms
 * 
 * \subsection ndsft Nonuniform Discrete Spherical Fourier Transform
 * The \e nonuniform \e discrete \e spherical \e Fourier \e transform (\e NDSFT)
 * is defined as follows:
 * \f[
 *     \begin{array}{rcl}
 *       \text{\textbf{Input}} & : & \text{coefficients } 
 *         \hat{f}(k,n) \in \mathbb{C} \text{ for } k=0,\ldots,N,\;n=-k,
 *         \ldots,k,\; N \in \mathbb{N}_0,\\[1ex]
 *                             &   & \text{arbitrary nodes } \mathbf{x}(m) \in 
 *         [0,\frac{1}{2}] \times [-\frac{1}{2},\frac{1}{2}] 
 *         \text{ for } m=0,\ldots,M-1, M \in \mathbb{N}. \\[1ex]
 *       \text{\textbf{Task}}  & : & \text{evaluate } f(m) := f\left(
 *       \mathbf{x}(m)\right) = \sum_{k=0}^N \sum_{n=-k}^k \hat{f}_k^n 
 *         Y_k^n\left(\mathbf{x}(m)\right) \text{ for } m=0,\ldots,M-1.
 *         \\[1ex]
 *       \text{\textbf{Output}} & : & \text{coefficients } f(m) \in 
 *         \mathbb{C} \text{ for } m=0,\ldots,M-1.\\
 *     \end{array}
 * \f]
 *
 * \subsection andsft Adjoint Nonuniform Discrete Spherical Fourier Transform
 * The \e adjoint \e nonuniform \e discrete \e spherical \e Fourier \e transform 
 * (\e adjoint \e NDSFT)
 * is defined as follows:
 * \f[
 *     \begin{array}{rcl}
 *       \text{\textbf{Input}} & : & \text{coefficients } f(m) \in 
 *         \mathbb{C} \text{ for } m=0,\ldots,M-1,\\
 *                             &   & \text{arbitrary nodes } \mathbf{x}(m) \in 
 *         [0,\frac{1}{2}] \times [-\frac{1}{2},\frac{1}{2}] \text{ for } 
 *         m=0,\ldots,M-1, M \in \mathbb{N}, N \in \mathbb{N}_0.\\[1ex]
 *       \text{\textbf{Task}}  & : & \text{evaluate } \hat{f}(k,n) 
 *         := \sum_{m=0}^{M-1} f(m) Y_k^n\left(\mathbf{x}(m)\right) 
 *         \text{ for } k=0,\ldots,N,\;n=-k,\ldots,k.\\[1ex]
 *       \text{\textbf{Output}} & : & \text{coefficients } 
 *         \hat{f}(k,n) \in \mathbb{C} \text{ for } 
 *         k=0,\ldots,N,\;n=-k,\ldots,k.\\[1ex]
 *     \end{array}
 * \f]
 * 
 * \subsection Good to know...
 * When using the routines of this module you should bear in mind the following:
 * - The bandwidth \f$N_{\text{max}}\f$ upt to which precomputation is always a 
 *   power of two.
 * - The \e NFSFT transform is \e in \e place, i.e. the coefficients 
 *   \f$\hat{f}(k,n)\f$ are destroyed.
 * - The \e adjoint \e NFSFT transform is \e out \e of \e place, i.e. the 
 *   coefficients \f$f(m)\f$ are not destroyed.
 */

/* Planner flags */

/** 
 * By default, all computations are performed with respect to the unnormalized basis
 * functions
 * \f[
 *   \tilde{Y}_k^n(\vartheta,\varphi) = P_k^{|n|}(\cos\vartheta) 
 *   \mathrm{e}^{\mathrm{i} n \varphi}.
 * \f]
 * If this flag is set, all computations are carried out using the \f$L_2\f$-
 * normalized basis functions
 * \f[
 *   Y_k^n(\vartheta,\varphi) = \sqrt{\frac{2k+1}{4\pi}} P_k^{|n|}(\cos\vartheta) 
 *   \mathrm{e}^{\mathrm{i} n \varphi}.
 * \f]  
 * 
 * \see nfsft_init
 * \author Jens Keiner
 */
#define NFSFT_NORMALIZED   (1U<<0)

/**
 * If this flag is set, the \e direct \e NDFT algorithm will be used internally 
 * instead of the fast approximative \e NFFT algorithm.
 *
 * \see nfsft_init
 * \author Jens Keiner
 */
#define NFSFT_USE_NDFT     (1U<<1)

#define NFSFT_MALLOC_X     (1U<< 2)
#define NFSFT_MALLOC_F_HAT (1U<< 3)
#define NFSFT_MALLOC_F     (1U<< 4)


/* Precomputation flags */

/**
 * If this flag is set, the algorithms \e direct \e NDSFT and \e adjoint \e direct 
 * \e NDSFT do not work. Setting this flag saves some memory for precomputed data.
 * 
 * \see nfsft_precompute
 * \see ndsft_trafo
 * \see ndsft_adjoint
 * \author Jens Keiner
 */
#define NFSFT_NO_DIRECT  (1U<<0)

/**
 * If this flag is set, the fast algorithms \e NFSFT and \e adjoint \e NFSFT only 
 * work in a defined bandwidth window. If \f$N_{\text{max}}\f$ is the power of two 
 * up to which precomputation is performed, only fast transformations for bandwidths 
 * \f$N\f$ with \f$N_{\text{max}}/2 < N \le N_{\text{max}}\f$ will work. The slow 
 * algorithms \e direct \e NDSFT and \e adjoint \e direct \e NDSFT are unaffected. 
 * Using this flag saves memory for precomputed data.
 *
 * \see nfsft_precompute
 * \see nfsft_trafo
 * \see nfsft_adjoint
 * \author Jens Keiner
 */
#define NFSFT_BW_WINDOW  (1U<<1)

/** Structure for a transform plan */
typedef struct nfsft_plan_
{
  /** Inherited public members */
  MACRO_MV_PLAN(complex);          

  /* Public members */
  int N;                                  /**< The bandwidth \f$N\f$              */
  double *x;                              /**< The nodes \f$\mathbf{x}(m) \in 
                                               [0,\frac{1}{2}] \times 
                                               [-\frac{1}{2},\frac{1}{2}]\f$ for 
                                               \f$m=0,\ldots,M-1\f$,\f$M \in 
                                               \mathbb{N}\f$                      */
  
  /* Private members */
  int NPT;                                /**< Next greater power of two with
                                               respect to \f$N\f$                 */
  int t;                                  /**< The logaritm of NPT with 
                                               respect to the basis 2             */  
  unsigned int flags;                     /**< The planner flags                  */
  nfft_plan plan_nfft;                    /**< The internal NFFT plan             */
} nfsft_plan;

/**
 * Creates a transform plan.
 *
 * \arg plan a pointer to a \verbatim nfsft_plan \endverbatim structure
 * \arg N The bandwidth \f$N \in \mathbb{N}_0\f$
 * \arg M The number of nodes \f$M \in \mathbb{N}\f$
 *
 * \author Jens Keiner
 */
void nfsft_init(nfsft_plan *plan, int N, int M);

/**
 * Creates a transform plan.
 *
 * \arg plan a pointer to a \verbatim nfsft_plan \endverbatim structure
 * \arg N The bandwidth \f$N \in \mathbb{N}_0\f$
 * \arg M The number of nodes \f$M \in \mathbb{N}\f$
 * \arg nfsft_flags The NFSFT flags 
 *
 * \author Jens Keiner
 */
void nfsft_init_advanced(nfsft_plan* plan, int N, int M, unsigned int nfsft_flags);

/**
 * Creates a transform plan.
 *
 * \arg plan a pointer to a \verbatim nfsft_plan \endverbatim structure
 * \arg N The bandwidth \f$N \in \mathbb{N}_0\f$
 * \arg M The number of nodes \f$M \in \mathbb{N}\f$
 * \arg nfsft_flags The NFSFT flags 
 * \arg nfft_cutoff The NFFT cutoff parameter
 *
 * \author Jens Keiner
 */
void nfsft_init_guru(nfsft_plan* plan, int N, int M, unsigned int flags, 
                     int nfft_cutoff);

/**
 * Performes precomputation up to the next power of two with respect to a given 
 * bandwidth \f$N \in \mathbb{N}_2\f$. The threshold parameter \f$\kappa \in 
 * \mathbb{R}^{+}\f$ determines the number of stabilization steps computed in the 
 * discrete polynomial transform and thereby its accuracy.
 *
 * \arg N The bandwidth \f$N \in \mathbb{N}_2\f$
 * \arg threshold The threshold \f$\kappa \in \mathbb{R}^{+}\f$
 * \arg flags The flags
 *
 * \author Jens Keiner
 */
void nfsft_precompute(int M, double kappa, unsigned int flags);

/**
 * Forgets all precomputed data.
 *
 * \author Jens Keiner
 */
void nfsft_forget();

/**
 * Executes a direct NDSFT, i.e. computes for \f$m = 0,\ldots,M-1\f$
 * \f[
 *   f(m) = \sum_{k=0}^N \sum_{n=-k}^k \hat{f}(k,n) Y_k^n\left(2\pi x_1(m),
 *   2\pi x_2(m)\right).  
 * \f]
 *
 * \arg plan The plan
 *
 * \author Jens Keiner
 */
void ndsft_trafo(nfsft_plan* plan);

/**
 * Executes a direct adjoint NDSFT, i.e. computes for \f$k=0,\ldots,N;
 * n=-k,\ldots,k\f$
 * \f[
 *   \hat{f}(k,n) = \sum_{m = 0}^{M-1} f(m) Y_k^n\left(2\pi x_1(m), 
 *   2\pi x_2(m)\right).  
 * \f]
 *
 * \arg plan The plan
 *
 * \author Jens Keiner
 */
void ndsft_adjoint(nfsft_plan* plan);

/**
 * Executes a NFSFT, i.e. computes for \f$m = 0,\ldots,M-1\f$
 * \f[
 *   f(m) = \sum_{k=0}^N \sum_{n=-k}^k \hat{f}(k,n) Y_k^n\left(2\pi x_1(m),
 *   2\pi x_2(m)\right).  
 * \f]
 *
 * \arg plan The plan
 *
 * \author Jens Keiner
 */
void nfsft_trafo(nfsft_plan* plan);

/**
 * Executes an adjoint NFSFT, i.e. computes for \f$k=0,\ldots,N;
 * n=-k,\ldots,k\f$
 * \f[
 *   \hat{f}(k,n) = \sum_{m = 0}^{M-1} f(m) Y_k^n\left(2\pi x_1(m), 
 *   2\pi x_2(m)\right).  
 * \f]
 *
 * \arg plan The plan
 *
 * \author Jens Keiner
 */
void nfsft_adjoint(nfsft_plan* plan);

/**
 * Destroys a plan.
 *
 * \arg plan The plan to be destroyed
 *
 * \author Jens Keiner
 */
void nfsft_finalize(nfsft_plan plan);

/** @}
 */

/** @defgroup solver Group
 * @{ 
 */

/** 
 * Constant symbols for precomputation and memory usage (inverse problem)
 */
#define LANDWEBER           	(1U<< 0)
#define STEEPEST_DESCENT    	(1U<< 1)
#define CGNR                    (1U<< 2)
#define CGNE 		    	(1U<< 3)
#define NORMS_FOR_LANDWEBER 	(1U<< 4)
#define PRECOMPUTE_WEIGHT   	(1U<< 5)
#define PRECOMPUTE_DAMP     	(1U<< 6)
/** will come in again, as method by their own, together with some new method for
    the minimal seminorm interpolation
    #define REGULARIZE_CGNR     	(1U<< 7)
    #define REGULARIZE_CGNR_R_HAT   (1U<< 8)
 double *r_hat;                        regularisation weights           
  double lambda;                        regularisation parameter         
*/

/** 
 * \brief function mangling macro
 *  
 * \arg MV matrix vector multiplication type
 * \arg FLT float type, i.e., double resp. complex
 * \arg name name of the function
 * \arg ...  argument list of the function
 */
#define F(MV, FLT, name, ...) void i ## MV ## _ ## name(__VA_ARGS__)

#define MACRO_SOLVER_PLAN(MV, FLT) \
typedef struct i ## MV ## _plan_ \
{	\
  MV ## _plan *mv;                          /**< matrix vector multiplication   */\
  unsigned flags;			                     /**< iteration type, ...            */\
 \
  double *w;                         	      /**< weighting factors              */\
  double *w_hat;                     	      /**< damping factors                */\
 \
  FLT *y;                                   /**< right hand side, samples       */\
 \
  FLT *f_hat_iter;                          /**< iterative solution             */\
									      \
  FLT *r_iter;			        /**< iterated residual vector       */\
  FLT *z_hat_iter;             	        /**< residual vector of normal eq.1 */\
  FLT *p_hat_iter;             	        /**< search direction               */\
  FLT *v_iter;                 	        /**< residual vector update         */\
									      \
  double alpha_iter;                    /**< step size for search direction */\
  double beta_iter;                     /**< step size for search correction*/\
									      \
  double dot_r_iter;                    /**< dotproductc{_w}(r_iter)        */\
  double dot_r_iter_old;                /**< old dotproductc{_w}(r_iter)    */\
  double dot_z_hat_iter;                /**< dotproductc{_w}(z_hat_iter)    */\
  double dot_z_hat_iter_old;            /**< old dotproductc{_w}(z_hat_iter)*/\
  double dot_p_hat_iter;                /**< dotproductc{_w}(p_hat_iter)    */\
  double dot_v_iter;                    /**< dotproductc{_w}(v_iter)        */\
} i ## MV ## _plan;                                                           \
									      \
F(MV, FLT, init,	  i ## MV ## _plan *ths, MV ## _plan *mv);            \
F(MV, FLT, init_advanced, i ## MV ## _plan *ths, MV ## _plan *mv,             \
		     	  unsigned i ## MV ## _flags);		              \
F(MV, FLT, before_loop,   i ## MV ## _plan *ths);                             \
F(MV, FLT, loop_one_step, i ## MV ## _plan *ths);                             \
F(MV, FLT, finalize,      i ## MV ## _plan *ths);                             \


MACRO_SOLVER_PLAN(nfft, complex)
MACRO_SOLVER_PLAN(nfct, double)
MACRO_SOLVER_PLAN(nfst, double)
MACRO_SOLVER_PLAN(nnfft, complex)
MACRO_SOLVER_PLAN(mri_inh_2d1d, complex)
MACRO_SOLVER_PLAN(mri_inh_3d, complex)
MACRO_SOLVER_PLAN(texture, complex)
/** @} 
 */

#endif
/* nfft3.h */
