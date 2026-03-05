/******************************************************/
/* Program to run and test the integration functions. */
/* Creation date: 31 July, 2025                       */
/******************************************************/
#include <math.h>
#include "integration.h"

/* Code here everything you need to test the integration methods and show your numericzal results */

double f(double x)
{
	return exp(x);
}

int main()
{
	double a = 1;
	double b = 5;
	double N = 100;

	QuadFormula qf;
	double integration_res;

	char *arr[] = {"left", "right", "middle", "trapezes", "simpson", "gauss2", "gauss3"};

	printf("\nFonction: f(x)=exp(x)\n\n");

	for (int i = 0; i < 7; i++)
	{
		setQuadFormula(&qf, arr[i]);
		printQuadFormula(&qf);
		integration_res = integrate(f, a, b, N, &qf);
		printf("\nResult: %f\n\n", integration_res);
	}

	return 0;
}
