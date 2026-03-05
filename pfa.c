

// #define GRAPHIC
// je sais pas...
#define PFA_C

#include "pfa.h"

/* Initialize the integration variables.
Arguments :
- quadrature : name of the quadrature formula that will be used. It can be "left", "right",
"middle", "trapezes", "simpson", "gauss2" or "gauss3".
- dt : a positive value, which will be used to decide the number of subdivisions of an
interval [a,b], when computing the integration.
The number of subdivisions will be N such that (b-a)/N ~ dt
*/
bool init_integration(char *quadrature, double dt)
{
	if (!quadrature || dt <= 0)
		return false;
	else if (strcmp(quadrature, "left") == 0)
	{
		pfa_dt = dt;
		setQuadFormula(&pfaQF, "left");
		return true;
	}
	else if (strcmp(quadrature, "right") == 0)
	{
		pfa_dt = dt;
		setQuadFormula(&pfaQF, "right");
		return true;
	}
	else if (strcmp(quadrature, "middle") == 0)
	{
		pfa_dt = dt;
		setQuadFormula(&pfaQF, "middle");
		return true;
	}
	else if (strcmp(quadrature, "trapezes") == 0)
	{
		pfa_dt = dt;
		setQuadFormula(&pfaQF, "trapezes");
		return true;
	}
	else if (strcmp(quadrature, "simpson") == 0)
	{
		pfa_dt = dt;
		setQuadFormula(&pfaQF, "simpson");
		return true;
	}
	else if (strcmp(quadrature, "gauss2") == 0)
	{
		pfa_dt = dt;
		setQuadFormula(&pfaQF, "gauss2");
		return true;
	}
	else if (strcmp(quadrature, "gauss3") == 0)
	{
		pfa_dt = dt;
		setQuadFormula(&pfaQF, "gauss3");
		return true;
	}
	else
		return false;
}

/* Density of the normal distribution */
double phi(double x)
{
	return 0.398942280401433 * exp(-x * x / 2);
}

/* Cumulative distribution function of the normal distribution */
double PHI(double x)
{
	return 0.5 + integrate_dx(phi, 0, x, pfa_dt, &pfaQF);
}

/* =====================================
	 Finance function: price of an option
	 */
double optionPrice(Option *option)
{
	double sig = option->sig;
	double k = option->K;
	double t = option->T;
	double mu = option->mu;
	double s0 = option->S0;
	if (sig <= 0 || t < 0)
	{
		if (t == 0)
		{
			if (option->type == CALL)
				return fmax(s0 - k, 0.0);
			else if (option->type == PUT)
				return fmax(k - s0, 0.0);
		}
		return 0.0;
	}
	if (option->type == CALL)
	{
		double z = (log(k / s0) - (mu - ((sig * sig) / 2)) * t) / (sig * sqrt(t));
		double c = s0 * exp(mu * t) * PHI(-z + sig * sqrt(t)) - k * PHI(-z);
		return c;
	}
	else if (option->type == PUT)
	{
		double z = (log(k / s0) - (mu - ((sig * sig) / 2)) * t) / (sig * sqrt(t));
		double p = k * PHI(z) - s0 * exp(mu * t) * PHI(z - sig * sqrt(t));
		return p;
	}
	return 0.0;
}

/* ===============================================*/
/* Insurance functions */

/* Probability density function (PDF) of variable X.
	 X is the reimbursement in case of a claim from the client.
	 */
double clientPDF_X(InsuredClient *client, double x)
{
	if (x <= 0)
		return 0;

	double sig = client->s;
	double mu = client->m;

	return (1 / (sig * x)) * phi((log(x) - mu) / sig);
}

/* Cumulative distribution function (CDF) of variable X.
	 X is the reimbursement in case of a claim from the client.
	 */
double clientCDF_X(InsuredClient *client, double x)
{
	if (x <= 0)
		return 0;

	double sig = client->s;
	double mu = client->m;
	return PHI((log(x) - mu) / sig);
}

/* ==========================================================*/
/* Distribution of X1+X2 : static intermediate functions     */

/* The static functions localProductPDF and localPDF_X1X2 take only one
	 argument, of type double.
	 They hence can be integrated: function integrate_dx takes as argument a function pointer f,
	 where f depends only on one argument (double t).
	 The static functions below can be given as argument to integrate_dx.

	 That's why we copy other variables of the final functions (client and x) to local static variables,
	 and define these static functions depending on only one argument (double t).
	 These local functions can hence be arguments of integrate_dx.
	 */
static InsuredClient *localClient;
static double localX;

/* This function assumes that static variables localClient and localX have been set.
	 It can be an argument of integrate_dx (since it has the good signature)
	 */
static double localProductPDF(double t)
{
	return clientPDF_X(localClient, localX - t) * clientPDF_X(localClient, t);
}

/* Density of X1+X2

	 This function assumes that static variable localClient has been set.
	 It is called by clientPDF_X1X2
	 It can also be an argument of integrate_dx (since it has the good signature)
	 */
static double localPDF_X1X2(double x)
{
	localX = x;

	return integrate_dx(localProductPDF, 0, x, pfa_dt, &pfaQF);
}

/* ==========================================================*/
/* Distribution of X1+X2 : the final functions               */

/* Probability density function (PDF) of variable X1+X2.
	 X1 and X2 are the reimbursements of the two claims from the client (assuming there are
	 two claims).
	 */
double clientPDF_X1X2(InsuredClient *client, double x)
{
	if (x <= 0)
		return 0.0;

	localClient = client;
	return localPDF_X1X2(x);
}

/* Cumulative distribution function (CDF) of variable X1+X2.
	 X1 and X2 are the reimbursements of the two claims from the client (assuming there are
	 two claims).
	 */
double clientCDF_X1X2(InsuredClient *client, double x)
{
	localClient = client;

	return integrate_dx(localPDF_X1X2, 0, x, pfa_dt, &pfaQF);
}

/* Cumulative distribution function (CDF) of variable S.
	 Variable S is the sum of the reimbursements that the insurance company will pay to client.
	 */
double clientCDF_S(InsuredClient *client, double x)
{
	if (x < 0)
	{
		return 0;
	}
	else if (x == 0)
	{
		return client->p[0];
	}
	else
	{
		double *p = client->p;
		return p[0] + p[1] * clientCDF_X(client, x) + p[2] * clientCDF_X1X2(client, x);
	}
}
