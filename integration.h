/*************************************/
/* Header file integration.h         */
/* Creation date: 31 July, 2025      */                                    
/*************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

#ifndef INTEGRATION_H
#define INTEGRATION_H

/* Represents a quadrature formula.
   The function integrate takes an argument of type QuadFormula *.
   Have everything in this structure that will be needed by the function integrate.
*/

typedef struct{
  char name[20]; 
  size_t n;
  double* w;
  double* x;

} QuadFormula;


#ifdef INTEGRATION_C

#else /* INTEGRATION_C */

/* Declaration of the functions that you have to code */

extern bool setQuadFormula(QuadFormula* qf, char* name);
extern void printQuadFormula(QuadFormula* qf); /* Not required but useful for debugging */

/* Returns the integral of function f from a to b. The approximation is done by splitting
   the interval [a,b] in N subdivisions, and then using the quadrature formula defined by qf */
extern double integrate(double (*f)(double), double a, double b, int N, QuadFormula* qf);

/* Same as function integrate, except that the number N of subdivision is computed using the
   argument dx: we take N = |b-a|/dx (rounded to be an integer) */
extern double integrate_dx(double (*f)(double), double a, double b, double dx, QuadFormula* qf);

double get_erreur(double (*f)(double), double a, double b, size_t N, QuadFormula* qf, double true_value);

struct error_param get_error_param(double (*f)(double), double a, double b, size_t base_N, size_t pas, size_t N, QuadFormula* qf, double true_value);

#endif /* INTEGRATION_C */

#endif /* INTEGRATION_H */
