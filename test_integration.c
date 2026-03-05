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
	double I, I2;

	char *arr[] = {"left", "right", "middle", "trapezes", "simpson", "gauss2", "gauss3"};

	printf("\n\nNotre fonction est: f(x)=exp(x)\n\n\n");

	for (int i = 0; i < 7; i++)
	{
		printf("╔═══════════════ஓ๑♡๑ஓ════════════════\n");

		setQuadFormula(&qf, arr[i]);
		printQuadFormula(&qf);
		I = integrate(f, a, b, N, &qf);
		printf("║ result: %f\n", I);
		printf("╚═══════════════ஓ๑♡๑ஓ════════════════\n");
	}

	char *methodTestErreur = "left";
	double trueValue = 145.69487;
	setQuadFormula(&qf, methodTestErreur);
	struct error_param values = get_error_param(f, a, b, 10, 10, 10, &qf, trueValue);
	printf("Avec la méthode %s\nOn obtient:\nAlpha: %f\nC: %f\nErreur avec N=100: %f\n", methodTestErreur, values.alpha, values.beta, values.beta / (pow(N, values.alpha)));

	return 0;
}
