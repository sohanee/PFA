#include <math.h>

#define INTEGRATION_C

#include "integration.h"

bool setQuadFormula(QuadFormula *qf, char *name)
{
	if (!strcmp(name, "left"))
	{
		strcpy(qf->name, name);
		qf->n = 0;
		qf->size_w = 1;
		qf->w = calloc(1, sizeof(double));
		qf->w[0] = 1.0;
		qf->size_x = 1;
		qf->x = calloc(1, sizeof(double));
		qf->x[0] = 0.0;
		return true;
	}
	else if (!strcmp(name, "right"))
	{
		strcpy(qf->name, name);
		qf->n = 0;
		qf->size_w = 1;
		qf->w = calloc(1, sizeof(double));
		qf->w[0] = 1;
		qf->size_x = 1;
		qf->x = calloc(1, sizeof(double));
		qf->x[0] = 1;
		return true;
	}
	else if (!strcmp(name, "middle"))
	{
		strcpy(qf->name, name);
		qf->n = 0;
		qf->size_w = 1;
		qf->w = calloc(1, sizeof(double));
		qf->w[0] = 1;
		qf->size_x = 1;
		qf->x = calloc(1, sizeof(double));
		qf->x[0] = 0.5;
		return true;
	}
	else if (!strcmp(name, "trapezes"))
	{
		strcpy(qf->name, name);
		qf->n = 0;
		qf->size_w = 2;
		qf->w = calloc(2, sizeof(double));
		qf->w[0] = 0.5;
		qf->w[1] = 0.5;
		qf->size_x = 2;
		qf->x = calloc(2, sizeof(double));
		qf->x[0] = 0;
		qf->x[1] = 1;
		return true;
	}
	else if (!strcmp(name, "simpson"))
	{
		strcpy(qf->name, name);
		qf->n = 2;
		qf->size_w = 3;
		qf->w = calloc(3, sizeof(double));
		qf->w[0] = (double)1 / 6;
		qf->w[1] = (double)2 / 3;
		qf->w[2] = (double)1 / 6;
		qf->size_x = 3;
		qf->x = calloc(3, sizeof(double));
		qf->x[0] = 0;
		qf->x[1] = 0.5;
		qf->x[2] = 1;
		return true;
	}
	else if (!strcmp(name, "gauss2"))
	{
		strcpy(qf->name, name);
		qf->n = 2;
		qf->size_w = 2;
		qf->w = calloc(2, sizeof(double));
		qf->w[0] = (double)1 / 2;
		qf->w[1] = (double)1 / 2;
		qf->size_x = 2;
		qf->x = calloc(2, sizeof(double));
		qf->x[0] = (double)(1 / 2 - 1 / (2 * sqrt(3)));
		qf->x[1] = (double)(1 / 2 + 1 / (2 * sqrt(3)));
		return true;
	}
	else if (!strcmp(name, "gauss3"))
	{
		strcpy(qf->name, name);
		qf->n = 3;
		qf->size_w = 3;
		qf->w = calloc(3, sizeof(double));
		qf->w[0] = (double)5 / 18;
		qf->w[1] = (double)4 / 9;
		qf->w[2] = (double)5 / 18;
		qf->size_x = 3;
		qf->x = calloc(3, sizeof(double));
		qf->x[0] = (double)1 / 2 * (1 - sqrt(3 / 5));
		qf->x[1] = (double)1 / 2;
		qf->x[2] = (double)1 / 2 * (1 + sqrt(3 / 5));
		return true;
	}
	return false;
}

/* This function is not required ,but it may useful to debug */
void printQuadFormula(QuadFormula *qf)
{
	printf("║ Quadratic formula: %s\n", qf->name);
	printf("║ n: %lu\n", qf->n);
	printf("║ size_w: %lu\n", qf->size_w);
	for (size_t i = 0; i < qf->size_w; i++)
	{
		printf("║ w%li: %f\n", i, qf->w[i]);
	}
	for (size_t i = 0; i < qf->size_x; i++)
	{
		printf("║ x%li: %f\n", i, qf->x[i]);
	}
	/* Print everything else that may be useful */
}

/* Approximate the integral of function f from a to b.
	 - f is a pointer to a function pointer
	 - a and b are the integration bounds
	 - Interval [a,b] is split in N subdivisions [ai,bi]
	 - Integral of f on each subdivision [ai,bi] is approximated by the quadrature formula qf.
	 */
double integrate(double (*f)(double), double a, double b, int N, QuadFormula *qf)
{
	double sign = 1;
	if (b < a)
	{
		double t = a;
		a = b;
		b = t;
		sign = -1;
	}

	double res = 0.0;
	for (int i = 0; i < N; i++)
	{
		double ai = a + i * (b - a) / N;
		double bi = a + ((b - a) * (i + 1)) / N;
		double sum = 0.0;
		for (size_t k = 0; k <= qf->n; k++)
		{
			sum += qf->w[k] * f(ai + qf->x[k] * (bi - ai));
		}
		res += (bi - ai) * sum;
	}
	return sign * res;
}

double integrate_dx(double (*f)(double), double a, double b, double dx, QuadFormula *qf)
{
	int N = (int)round(abs(b - a) / dx);
	if (N <= 0)
		N = 1;
	return integrate(f, a, b, N, qf);
}

double get_erreur(double (*f)(double), double a, double b, size_t N, QuadFormula *qf, double true_value)
{
	return abs(integrate(f, a, b, N, qf) - true_value);
}

struct error_param get_error_param(double (*f)(double), double a, double b, size_t base_N, size_t pas, size_t N, QuadFormula *qf, double true_value)
{
	struct error_param values;

	double xB = 0;
	double yB = 0;
	double *x = calloc(N, sizeof(double));
	double *y = calloc(N, sizeof(double));

	for (size_t i = 0; i < N; i++)
	{
		double curN = base_N + pas * i;
		x[i] = -log(curN);
		y[i] = log(get_erreur(f, a, b, curN, qf, true_value));
		xB += x[i] / N;
		yB += y[i] / N;
	}

	double alpha = 0;
	double top = 0;
	double bottom = 0;
	for (size_t i = 0; i < N; i++)
	{
		top += ((x[i] - xB) * (y[i] - yB));
		bottom += pow((x[i] - xB), 2);
	}
	alpha = top / bottom;
	double beta = yB - alpha * xB;

	values.alpha = alpha;
	values.beta = exp(beta);

	free(x);
	free(y);

	return values;
}
